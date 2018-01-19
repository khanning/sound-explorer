PROJECT=main
SOURCES=$(PROJECT).c fatfs/ff.c fatfs/diskio.c fatfs/mmc_avr_spi.c
MMCU=atxmega16a4u
F_CPU=32000000
TARGET=x16a4u
PROGRAMMER=jtag2pdi

CFLAGS=-mmcu=$(MMCU) -Wall -Os -DF_CPU=$(F_CPU)

$(PROJECT).hex: $(PROJECT).out
	avr-objcopy -O ihex $(PROJECT).out $(PROJECT).c.hex;\
	avr-size --mcu=$(MMCU) --format=avr $(PROJECT).out

$(PROJECT).out: $(SOURCES)
	avr-gcc $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)

flash: $(PROJECT).hex
	avrdude -p $(TARGET) -c $(PROGRAMMER) -U flash:w:$(PROJECT).c.hex

clean:
	rm -v $(PROJECT).c.hex $(PROJECT).out
