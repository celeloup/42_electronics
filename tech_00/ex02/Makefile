MCU=atmega328p
F_CPU=16000000

all: hex flash

hex:
	avr-gcc -mmcu=${MCU} -D F_CPU=${F_CPU} -o main.bin main.c
	avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -p ${MCU} -c arduino -U flash:w:main.hex:i -F -P /dev/ttyUSB*

screen:
	screen -S joe /dev/ttyUSB* 115200

quit:
	screen -X -S joe kill

clean:
	rm main.hex
	rm main.bin