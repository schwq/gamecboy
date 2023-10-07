CC := gcc 
FLAGS := -Wall -g
LINK := -lSDL2main -lSDL2  
all:
	$(CC) $(FLAGS)  source/main.c source/cartridge.c source/window.c source/ram.c source/cpu.c source/cpu_cb_inst.c source/instruction.c -o main $(LINK) && ./main
