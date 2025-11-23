cmake_minimum_required(VERSION 3.28.3)
set(${CMAKE_C_STANDARD} C17)

project(gamecboy)

set(sourceFiles
    source/main.c 
    source/io.c
    source/cartridge.c 
    source/serial.c
    source/ram.c 
    source/cpu.c
    source/cpu_cb_inst.c 
    source/instruction.c
    source/common.c
    source/interrupt.c
    source/gamepad.c
    source/timer.c
    source/dma.c
    source/ppu.c
    source/emulator.c
    source/window.c
)

add_executable(gamecboy ${sourceFiles})

target_link_libraries(gamecboy SDL2main SDL2 m)
include_directories(include)
add_definitions(-w -g)