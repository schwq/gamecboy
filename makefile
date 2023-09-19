all:
	gcc -Wall -g source/main.c source/cartridge.c source/ram.c source/cpu.c source/cpu_cb_inst.c source/instruction.c -o main && .\main.exe -I/include -L/libs/SDL2 -lSDL2 -lSDL2main