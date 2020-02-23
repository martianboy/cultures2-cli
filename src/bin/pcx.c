#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pcx.h"

// RGBColor* read_pcx(char* path) {
//   FILE *fp = fopen(path, "r");
//   RGBColor *palette = read_palette(fp);
//   fclose(fp);

//   return palette;
// }

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
