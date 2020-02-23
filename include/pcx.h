#ifndef PCX_H
#define PCX_H

typedef struct RGBColor {
  unsigned char blue;
  unsigned char green;
  unsigned char red;
} RGBColor;

typedef struct PCX_Header {
  unsigned char magic;    // 0x0A
  unsigned char version;  // 0x05
  unsigned char encoding_method;  // 0x01 | 0x00
  unsigned char bits_per_pixel;  // 0x01 | 0x02 | 0x04 | 0x08
  unsigned short x0;
  unsigned short y0;
  unsigned short x1;
  unsigned short y1;
  unsigned short h_dpi;
  unsigned short v_dpi;
  unsigned char palette[16][3];
  unsigned char reserved;
  unsigned char color_planes;
  unsigned short bytes_per_color_plane;
  unsigned short palette_type;
  unsigned short h_res;
  unsigned short v_res;
  unsigned char reserved_block[54];
} PCX_Header;

RGBColor* read_pcx_palette(char* path);

#endif