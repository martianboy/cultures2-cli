#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <png.h>

#include "bmd.h"
#include "pcx.h"
#include "utils/pngutil.h"

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

void extract_frame_type_2(BMD_File *bmd_file, int frame, RGBColor *palette, const char *out_path) {
  int frame_start = bmd_file->frame_info[frame].off;
  int frame_count = bmd_file->frame_info[frame].len;
  int width = bmd_file->frame_info[frame].width;

  unsigned char* frame_buf = &bmd_file->pixels[bmd_file->row_info[frame_start].offset];

  Bitmap bmp;
  bmp.height = frame_count + 1;
  bmp.width = width;
  bmp.pixels = calloc(sizeof(RGBA), bmp.height * bmp.width);

  for (int row = 0; row < frame_count; row++) {
    int indent = bmd_file->row_info[row + frame_start].indent;
    int i = 0;

    while (i < indent) {
      set_pixel(pixel_at(&bmp, i++, row), 0, 0, 0, 0);
    }

    unsigned char head = *(frame_buf++);
    while (head != 0) {
      if (head < 0x80) {
        set_pixel_rgb(pixel_at(&bmp, i++, row), &palette[head]);
      } else {
        i += head - 0x80;
      }
      head = *(frame_buf++);
    }

    while (i < width) {
      set_pixel(pixel_at(&bmp, i++, row), 0, 0, 0, 0);
    }
  }

  save_png_to_file(&bmp, out_path);
  free(bmp.pixels);
}

void extract_frame_type_1(BMD_File *bmd_file, int frame, RGBColor *palette, const char *out_path) {
  int frame_start = bmd_file->frame_info[frame].off;
  int frame_count = bmd_file->frame_info[frame].len;
  int width = bmd_file->frame_info[frame].width;

  unsigned char* frame_buf = &bmd_file->pixels[bmd_file->row_info[frame_start].offset];

  Bitmap bmp;
  bmp.height = frame_count + 1;
  bmp.width = width;
  bmp.pixels = calloc(sizeof(RGBA), bmp.height * bmp.width);

  for (int row = 0; row < frame_count; row++) {
    int indent = bmd_file->row_info[row + frame_start].indent;
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
  free(bmp.pixels);
}

void extract_frame(BMD_File *bmd_file, int frame, RGBColor *palette, const char *out_path) {
  int type = bmd_file->frame_info[frame].type;

  switch (type) {
    case 1:
      extract_frame_type_1(bmd_file, frame, palette, out_path);
      break;
    case 2:
      extract_frame_type_2(bmd_file, frame, palette, out_path);
      break;
    case 3:
      printf("Unknown frame type %d\n", type);
      break;
  }
}

void convert(char *bmd_path, char *pcx_path) {
  RGBColor *palette = read_pcx_palette(pcx_path);
  BMD_File *bmd_file = read_bmd(bmd_path);

  char *out_path = malloc(100);

  for (int i = 0; i < bmd_file->header->num_frames; i++) {
    sprintf(out_path, "output/frame%03d.png", i);
    extract_frame(bmd_file, i, palette, out_path);
  }

  free(palette);
  free_bmd(bmd_file);
}

int main(int argc, char **argv) {
  char* pcx_path = "data/engine2d/bin/palettes/landscapes/rock03.pcx";
  char* bmd_path = "data/engine2d/bin/bobs/ls_chest.bmd";

  convert(bmd_path, pcx_path);

  return 0;
}