#include <stdio.h>
#include <stdlib.h>

#include "pcx.h"

void read_header(FILE *fp) {
  PCX_Header header;
  fread(&header, sizeof(PCX_Header), 1, fp);
}

RGBColor* read_palette(FILE *fp) {
  fseek(fp, -769, SEEK_END);

  RGBColor *extended_palette = malloc(256 * 3);
  fread(extended_palette, 3, 256, fp);

  return extended_palette;
}

RGBColor* read_pcx_palette(char* path) {
  FILE *fp = fopen(path, "r");
  RGBColor *palette = read_palette(fp);
  fclose(fp);

  return palette;
}
