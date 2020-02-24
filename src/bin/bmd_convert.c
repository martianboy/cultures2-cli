#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <png.h>

#include "bmd.h"
#include "pcx.h"
#include "utils/pngutil.h"

// unsigned int row_boundary(BMD_File *bmd_file, int frame) {
//   int off = bmd_file->frame_info[frame].off;
//   int len = bmd_file->frame_info[frame].len;

//   unsigned int m = 0, n = 0;

//   for (int i = 0; i < bmd_file->header->num_rows; i++) {
//     if (bmd_file->row_info[i].offset >= off && bmd_file->row_info[i].offset < off + len) {
//       if (i < m || m == 0) m = i;
//       if (i > n) n = i;
//     };
//   }

//   return m + (n << 16);
// }

void set_pixel(RGBA *pixel, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
  pixel->alpha = alpha;
  pixel->blue = blue;
  pixel->green = green;
  pixel->red = red;
}

void set_pixel_rgb(RGBA *pixel, RGBColor *color) {
  pixel->alpha = 0xFF;
  pixel->red = color->red;
  pixel->green = color->green;
  pixel->blue = color->blue;
}

void extract_frame(BMD_File *bmd_file, int frame, RGBColor *palette, const char *out_path) {
  int off = bmd_file->frame_info[frame].off;
  int width = bmd_file->frame_info[frame].width;

  unsigned char* frame_buf = &bmd_file->pixels[off];
  // unsigned int boundary = row_boundary(bmd_file, frame);
  int frame_start = 0; // boundary & 0xFFFF;
  int frame_end = 47; // boundary >> 16;

  Bitmap bmp;
  bmp.height = frame_end - frame_start + 1;
  bmp.width = width;
  bmp.pixels = calloc(sizeof(RGBA), bmp.height * bmp.width);

  for (int row = 0; row <= frame_end; row++) {
    int indent = bmd_file->row_info[row].indent;
    int i = 0;

    while (i < indent) {
      set_pixel(pixel_at(&bmp, i++, row), 0, 0, 0, 0);
    }

    for (unsigned char head = *(frame_buf++); head != 0; head = *(frame_buf++)) {
      if (head > 0 && head < 0x80) {
        for (int j = 0; j < head; j++) {
          set_pixel_rgb(pixel_at(&bmp, i++, row), &palette[*(frame_buf++)]);
        }
      } else {
        for (int j = 0; j < head - 0x80; j++) {
          set_pixel(pixel_at(&bmp, i++, row), 0, 0, 0, 0);
        }
      }
    }

    while (i < width) {
      set_pixel(pixel_at(&bmp, i++, row), 0, 0, 0, 0);
    }
  }

  save_png_to_file(&bmp, out_path);
  free_bitmap(&bmp);
}

void convert(char *bmd_path, char *pcx_path) {
  RGBColor *palette = read_pcx_palette(pcx_path);
  BMD_File *bmd_file = read_bmd(bmd_path);

  extract_frame(bmd_file, 0, palette, "frame0.png");
  extract_frame(bmd_file, 1, palette, "frame1.png");
  extract_frame(bmd_file, 2, palette, "frame2.png");
  extract_frame(bmd_file, 3, palette, "frame3.png");
  extract_frame(bmd_file, 4, palette, "frame4.png");
  extract_frame(bmd_file, 5, palette, "frame5.png");

  free(palette);
  free_bmd(bmd_file);
}

int main(int argc, char **argv) {
  char* pcx_path = "data/engine2d/bin/palettes/landscapes/rock03.pcx";
  char* bmd_path = "data/engine2d/bin/bobs/ls_chest.bmd";

  convert(bmd_path, pcx_path);

  return 0;
}