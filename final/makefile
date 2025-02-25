clean :
	del final.asm final.lk final.ihx final.hex final.lst final.map final.mem final.rel final.rst final.sym
	del Keypad4x4.rel Keypad4x4.asm Keypad4x4.lst Keypad4x4.sym
	del LED_Display.rel LED_Display.asm LED_Display.lst LED_Display.sym

all: final.hex final.ihx

final.hex: final.ihx
	packihx final.ihx > final.hex

final.ihx: final.c mylib.lib
	sdcc final.c mylib.lib
 
mylib.lib: Keypad4x4.rel LED_Display.rel
	sdar -rs mylib.lib Keypad4x4.rel LED_Display.rel
	sdranlib mylib.lib

LED_Display.rel: LED_Display.c
	sdcc -c LED_Display.c

Keypad4x4.rel: Keypad4x4.c
	sdcc -c Keypad4x4.c