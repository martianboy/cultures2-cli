#ifndef PNG_UTIL_H
#define PNG_UTIL_H

typedef struct RGBA {
  unsigned int red : 8;
  unsigned int green : 8;
  unsigned int blue : 8;
  unsigned int alpha : 8;
} RGBA;

typedef struct Bitmap {
  RGBA *pixels;
  int   width;
  int   height;
} Bitmap;

RGBA* pixel_at(Bitmap *bmp, int x, int y);
void free_bitmap(Bitmap *bmp);
int save_png_to_file(Bitmap *bitmap, const char *path);

#endif