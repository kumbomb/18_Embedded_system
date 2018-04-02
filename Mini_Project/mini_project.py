import pygame.mixer
import RPi.GPIO as GPIO
from sys import exit
import time

#set Broadcom chip-specific pin numbers mode as GPIO
GPIO.setmode(GPIO.BCM)

#GPIO NUM
SERVO=17
LED_1=18
LED_2=25
LED_3=8
SET_BUTTON=21
FREE_BUTTON=20
TRIG=23
ECHO=24

#gpio set
GPIO.setup(TRIG,GPIO.OUT)
GPIO.setup(ECHO,GPIO.IN)
GPIO.setup(LED_1,GPIO.OUT)
GPIO.setup(LED_2,GPIO.OUT)
GPIO.setup(LED_3,GPIO.OUT)
GPIO.setup(SERVO,GPIO.OUT)
GPIO.setup(SET_BUTTON,GPIO.IN)
GPIO.setup(FREE_BUTTON,GPIO.IN)

#sound
pygame.mixer.init(48000,-16,1,1024)

#set music you want to play
soundA = pygame.mixer.Sound("police_s.wav")
soundChannelA = pygame.mixer.Channel(1)


GPIO.output(TRIG,GPIO.LOW)
GPIO.output(LED_1,GPIO.LOW)
GPIO.output(LED_2,GPIO.LOW)
GPIO.output(LED_3,GPIO.LOW)

#set for using servo motor
motor = GPIO.PWM(SERVO,50)
time.sleep(0.3)
time_count = 0
flag = False

try:
    while(1): #when you push button, run below procedure
        time.sleep(0.3)
        if GPIO.input(SET_BUTTON) == True or flag == True:
            flag = True
            GPIO.output(TRIG,True)
            time.sleep(0.00001)
            GPIO.output(TRIG,False)

            while GPIO.input(ECHO) == 0:
                pulse_start = time.time()
            while GPIO.input(ECHO) == 1:
                pulse_end = time.time()

            #operate distance
            pulse_duration = pulse_end - pulse_start
            distance = pulse_duration * 17150
            distance = round(distance, 2)

            if(time_count <= 20 and distance <= 30):
                if(time_count >= 0 and time_count <= 10):
                    GPIO.output(LED_1,True)
                    time.sleep(0.5)
                    print "Now Secure level : green"
                    time_count = time_count + 1;
                if(time_count > 10 and time_count <= 20):
                    GPIO.output(LED_1,False)
                    time.sleep(0.3)
                    GPIO.output(LED_2,True)
                    time_count = time_count + 1;
                    print "Now secure level : yellow"
            elif(time_count > 20):
                    GPIO.output(LED_2,False)
                    time.sleep(0.3)
                    GPIO.output(LED_3,True)
                    print "Now secure level : red"
                    soundChannelA.play(soundA)
                    time.sleep(0.2)

        if GPIO.input(FREE_BUTTON) == True :
            motor.start(2.5)
            time.sleep(0.5)
            print "Revoke Your Security"
            print "Good Bye"
            GPIO.cleanup()
            exit()

except KeyboardInterrupt:
    GPIO.cleanup()
    exit()
