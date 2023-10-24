CC := gcc 
FLAGS := -Wall -g
LINK := -lSDL2main -lSDL2 -lm 
INCLUDE := -Iinclude
all:
	$(CC) $(FLAGS)  source/main.c source/ppu.c source/graphics.c source/ppu.c source/local_time.c source/log_file.c source/cartridge.c source/window.c source/ram.c source/cpu.c source/cpu_cb_inst.c source/instruction.c -o gcb $(INCLUDE) $(LINK) && ./gcb
