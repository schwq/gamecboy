#include <cartridge.h>
#include <common.h>
#include <cpu.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
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

  init_cpu();
  while (!cpu.control.shutdown) {
    run_cpu();
  }

  return EXIT_SUCCESS;
}
