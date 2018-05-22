#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include <string.h>
#define CS_MCP3208 8
#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

int read_mcp3208_adc(unsigned char adcChannel);
int get_light();

