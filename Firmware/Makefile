CC = xtensa-lx106-elf-gcc
CFLAGS = -I. -I/opt/esp-open-sdk/sdk/include -DICACHE_FLASH -mlongcalls
LDLIBS = -nostdlib -Wl,--start-group -lmain -ldriver -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc
LDFLAGS = -Teagle.app.v6.ld
SRC = user_main.c i2c.c interface.c interrupt_config.c network.c oled_functions.c timerkeeping.c

all: 0x00000.bin

0x00000.bin: user_main
	esptool.py elf2image $^

user_main: $(SRC)

flash: user_main-0x00000.bin
	esptool.py write_flash 0 user_main-0x00000.bin 0x10000 user_main-0x10000.bin

clean:
	clear
	rm -f user_main user_main.o user_main-0x00000.bin user_main-0x10000.bin
	rm *.o
