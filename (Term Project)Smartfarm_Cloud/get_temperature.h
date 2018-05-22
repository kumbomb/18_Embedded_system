#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

#define DHTPIN 7 
#define MAXTIMINGS 85

static int ret_temp;
static int dht22_dat[5] = {0,0,0,0,0};

int get_temp();
