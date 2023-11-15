#!/usr/bin/bash
gcc *.c -o app -I./st7735 ./st7735/*.c -lwiringPi -lpthread
./app
