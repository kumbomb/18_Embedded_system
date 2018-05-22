smartfarm : main.cpp getlight.o get_temp.o rgb.o fan.o
	g++ -o smartfarm main.cpp getlight.o get_temp.o fan.o rgb.o -lpthread -lwiringPi -lmysqlclient



getlight.o : getlightsensor.h getlightsensor.cpp
	g++ -Wall -c -o getlight.o getlightsensor.cpp

get_temp.o : get_temperature.h get_temperature.cpp
	g++ -Wall -c -o get_temp.o get_temperature.cpp

rgb.o : rgbtest.h rgbtest.cpp
	g++ -Wall -c -o rgb.o rgbtest.cpp 

fan.o : fanon.h fanon.cpp
	g++ -Wall -c -o fan.o fanon.cpp

clean :
	rm -r fan.o getlight.o get_temp.o rgb.o smartfarm
