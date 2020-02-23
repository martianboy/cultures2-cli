#ifndef BMD_H
#define BMD_H

typedef struct BMD_Header {
  unsigned int magic;
  unsigned int zero_0;
  unsigned int zero_1;
  unsigned int num_frames;
  unsigned int num_pixels;
  unsigned int num_rows;
  unsigned int unknown_1;
  unsigned int unknown_2;
  unsigned int zero_2;
} BMD_Header;

typedef struct BMD_Section_Header {
  unsigned int magic;
  unsigned int zero_0;
  unsigned int length;
} BMD_Section_Header;

typedef struct BMD_Frame_Info {
  unsigned int type;
  unsigned int meta_1;
  unsigned int meta_2;
  unsigned int width;
  unsigned int len;
  unsigned int off;
} BMD_Frame_Info;

typedef struct BMD_Frame_Row {
  int offset : 22;
  int indent : 10;
} BMD_Frame_Row;

typedef struct BMD_File {
  BMD_Header     *header;
  BMD_Frame_Info *frame_info;
  BMD_Frame_Row  *row_info;
  unsigned char  *pixels;
} BMD_File;

BMD_File* read_bmd(char* path);
void free_bmd(BMD_File* bmd_file);

#endif