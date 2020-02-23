#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bmd.h"
#include "pcx.h"

void convert(char *bmd_path, char *pcx_path) {
  RGBColor *palette = read_pcx_palette(pcx_path);
  BMD_File *bmd_file = read_bmd(bmd_path);

  free(palette);
  free_bmd(bmd_file);
}

int main(int argc, char **argv) {
  char* pcx_path = "data/engine2d/bin/palettes/landscapes/rock03.pcx";
  char* bmd_path = "data/engine2d/bin/bobs/ls_chest.bmd";

  convert(bmd_path, pcx_path);

  return 0;
}