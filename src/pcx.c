#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pcx.h"

void read_header(FILE *fp) {
  PCX_Header header;
  fread(&header, sizeof(PCX_Header), 1, fp);
}

RGBColor* read_palette(FILE *fp) {
  // fseek(fp, sizeof(PCX_Header), SEEK_CUR);
  fseek(fp, -769, SEEK_END);

  RGBColor *extended_palette = malloc(256 * 3);
  fread(extended_palette, 3, 256, fp);

  return extended_palette;
}

// RGBColor* read_pcx(char* path) {
//   FILE *fp = fopen(path, "r");
//   RGBColor *palette = read_palette(fp);
//   fclose(fp);

//   return palette;
// }

RGBColor* read_pcx_palette(char* path) {
  FILE *fp = fopen(path, "r");
  RGBColor *palette = read_palette(fp);
  fclose(fp);

  return palette;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: bin/pcx <path/to/file.pcx>\n");
    return 0;
  }

  RGBColor* palette = read_pcx_palette(argv[1]);

  printf(".hey {\n");
  for (int i = 0; i < 256; i++) {
    printf("  --color_%02X: #%02X%02X%02X;\n", i, palette[i].blue, palette[i].green, palette[i].red);
  }
  printf("}\n");

  free(palette);

  return 0;
}
