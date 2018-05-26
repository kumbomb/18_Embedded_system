#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <mysql/mysql.h>
#include <time.h>
#include "fanon.h"
#include "getlightsensor.h"
#include "get_temperature.h"
#include "rgbtest.h"

#define MAX 1000

#define DBHOST "localhost"
#define DBUSER "root"
#define DBPASS "root"
#define DBNAME "term_project"

pthread_cond_t empty, fill, fanon, ledon = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

MYSQL *connector;
MYSQL_RES *result;
MYSQL_ROW row;

int fill_ptr = 0;
int use_ptr = 0;
int count = 0;
int isLed = 0;
int isFan = 0;
int num = 0;
 
char query[1024];

typedef struct _buffer_data{
	int temp;
	int lightness;
}buffer_data;

buffer_data buff[MAX];
buffer_data tmp[MAX];

void sig_handler(int signo){	
	fan_off();
	printf("fan off\n");
	led_off();
	printf("led off\n");
	mysql_close(connector);
	printf("close mysql\n");
	exit(0);
}
void put(int t, int l)
{
	buff[fill_ptr].temp = t;
	buff[fill_ptr].lightness = l;
	printf("put : %d, %u\n",t,l);
	fill_ptr = (fill_ptr + 1) % MAX;
	count++;
}
buffer_data get()
{
	buffer_data g = buff[use_ptr];
	if(g.temp > 0 && g.lightness > 0)
	{
		printf("get : %d, %u\n",g.temp,g.lightness);
	}
	use_ptr = (use_ptr + 1) % MAX;
	count--;
	return g;
}
void *led_func(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(isLed==0){
			led_off();
			pthread_cond_wait(&ledon,&mutex);
		}
		pthread_mutex_unlock(&mutex);
		led_on();
		delay(500);
		isLed=0;
	}
}
void *fan_func(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(isFan==0){
			fan_off();
			pthread_cond_wait(&fanon,&mutex);
		}
		pthread_mutex_unlock(&mutex);
		fan_on();
		delay(5000);
		isFan = 0;
	}
}
void *monitor_data(void *arg)
{
	int temp, lightness;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(count==MAX)
		{
			pthread_cond_wait(&empty,&mutex);
		}
		temp = get_temp();
		lightness = get_light();
		if(temp > 0 && lightness > 0){ put(temp,lightness);}
		if(temp >= 20){ isFan = 1; pthread_cond_signal(&fanon);}
		if(lightness >= 3000){ isLed = 1; pthread_cond_signal(&ledon);}    
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
		delay(1);
	}
}
void *send_data(void *arg)
{
	int i, len;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(count == 0)
		{
			pthread_cond_wait(&fill,&mutex);
		}
		pthread_mutex_unlock(&mutex);
		len = sizeof(buff)/sizeof(buffer_data);
		for(i = 0; i<len;i++)
		{
			tmp[i]=get();
			if(tmp[i].temp <= 0 && tmp[i].lightness <= 0)
			{
				continue;
			}
			sprintf(query,"insert into term values (%d,now(),%d,%d)",num++,tmp[i].lightness, tmp[i].temp);	
			if(mysql_query(connector, query))
			{
				fprintf(stderr,"%s\n",mysql_error(connector));
				printf("wrtie DB ERROR\n");
			}
		}
		memset(tmp, 0, sizeof(tmp));
		pthread_cond_signal(&empty);
		delay(10000);
	}
}
void sensor_init()
{
	pinMode(CS_MCP3208,OUTPUT);
	pinMode(FAN, OUTPUT);
	pinMode(DHTPIN,OUTPUT);
	pinMode(RED,OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
}

int main(void)
{
    pthread_t p1, p2, p3,p4;
	signal(SIGINT,sig_handler);
	if(wiringPiSetupGpio() == -1)
	{
		fprintf(stdout,"unable to start wiringpi : %s\n",strerror(errno));
	}
	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
	{
		return 1;
	}
	sensor_init();
	printf("init sensor\n");
	connector = mysql_init(NULL);
	if(!mysql_real_connect(connector, DBHOST,DBUSER,DBPASS, DBNAME, 3306,NULL,0))
	{ 
	       fprintf(stderr,"%s\n",mysql_error(connector));
	       return 0;
	}	       
	printf("MySQL(term_project) opened.\n");
	
	pthread_create(&p1,NULL,monitor_data,NULL);
	pthread_create(&p2,NULL,send_data,NULL);
	pthread_create(&p3,NULL,led_func,NULL);
	pthread_create(&p4,NULL,fan_func,NULL);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(p3,NULL);
	pthread_join(p4,NULL);
	
	mysql_close(connector);
	return 0;
}

