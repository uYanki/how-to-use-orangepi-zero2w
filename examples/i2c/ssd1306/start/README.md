# SSD1360

```shell
# scan address
root@orangepizero2w:/home/orangepi# i2cdetect -y 2
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- 3c -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --

# recompile wiringPi
$ cd..
$ ./build

# compile then run example
$ cd /home/orangepi/Desktop/examples/i2c/ssd1306/
$ gcc -Wall -o app main.c -lwiringPi -lpthread
$ ./app

# compile then run default demo 
$ cd /usr/src/wiringOP/examples/
$ gcc -Wall -o app oled_demo.c -lwiringPi -lpthread
$ ./app /dev/i2c-2
```

