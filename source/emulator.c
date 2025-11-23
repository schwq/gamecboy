#include <cpu.h>
#include <dma.h>
#include <emulator.h>
#include <pthread.h>
#include <timer.h>
#include <window.h>

emu_context emu = {0};

void emu_cycle(unsigned int cycles)
{
  FOR_I(n, cycles)
  {
    FOR_I(i, 4)
    {
      emu.ticks++;
      timer_tick();
      // ppu_tick();
    }

    dma_tick();
  }
}

void* cpu_thread(void* p)
{
  timer_init();
  init_cpu();
  //ppu_init();

  while (emu.running && !cpu.control.shutdown) {
    if (emu.paused) {
      SDL_Delay(10);
      continue;
    }

    run_cpu();
  }
}

int emu_run(int argc, char** argv)
{
  if (argc == 1) {
    logfmt(lwarn, "You must declared a rom path!");
    return EXIT_FAILURE;
  }

  if (argc == 2) {
    if (!load_rom(argv[1])) {
      logfmt(lerror, "Cannot load game!\n");
      return EXIT_FAILURE;
    }
    logfmt(linfo, "Loaded game : %s\n", argv[1]);
  }

  window_init("GB emulator");

  emu.die = false;
  emu.running = true;
  emu.paused = false;

  pthread_t thread;

  if (pthread_create(&thread, NULL, cpu_thread, NULL)) {
    logfmt(lerror, "Cannot create CPU thread!");
    return EXIT_FAILURE;
  }

  while (window.running && !emu.die) {
    window_run();
  }

  return EXIT_SUCCESS;
}