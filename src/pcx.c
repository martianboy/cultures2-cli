#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pcx.h"

void read_header(FILE *fp) {
  PCX_Header header;
  fread(&header, sizeof(PCX_Header), 1, fp);
}

void* read_palette(FILE *fp) {
  // fseek(fp, sizeof(PCX_Header), SEEK_CUR);
  fseek(fp, -769, SEEK_END);

  void *extended_palette = malloc(256 * 3);
  fread(extended_palette, 3, 256, fp);

  return extended_palette;
}

void* read_pcx(char* path) {
  FILE *fp = fopen(path, "r");
  void *raw = read_palette(fp);
  fclose(fp);

  return raw;
}

RGBColor** read_pcx_palette(char* path) {
  RGBColor **palette = malloc(sizeof(RGBColor*) * 256);

  FILE *fp = fopen(path, "r");
  void *raw = read_palette(fp);
  fclose(fp);

  for (int i = 0; i < 256; i++) {
    palette[i] = (RGBColor *)(raw + i * 3);
  }

  return palette;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: bin/pcx <path/to/file.pcx>\n");
    return 0;
  }

  RGBColor** palette = read_pcx_palette(argv[1]);

  printf(".hey {\n");
  for (int i = 0; i < 256; i++) {
    printf("  --color_%02X: #%02X%02X%02X;\n", i, palette[i]->blue, palette[i]->green, palette[i]->red);
  }
  printf("}\n");

  free(palette);

  return 0;
}
