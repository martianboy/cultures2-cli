#include <stdio.h>
#include <stdlib.h>

#include "bmd.h"

unsigned int read_section_header(FILE *fp) {
  BMD_Section_Header header;
  fread(&header, sizeof(BMD_Section_Header), 1, fp);

  return header.length;
}

BMD_Frame_Info* read_frames_section(FILE *fp, int len) {
  BMD_Frame_Info* frames = malloc(len);
  fread(frames, sizeof(BMD_Frame_Info), len / sizeof(BMD_Frame_Info), fp);

  return frames;
}

BMD_Frame_Row* read_rows_section(FILE *fp, int len) {
  BMD_Frame_Row* rows = malloc(len);
  fread(rows, sizeof(BMD_Frame_Row), len / sizeof(BMD_Frame_Row), fp);

  return rows;
}

BMD_File* read_struct(FILE *fp) {
  BMD_Header *bmd_header = malloc(sizeof(BMD_Header));
  fread(bmd_header, sizeof(BMD_Header), 1, fp);

  BMD_File *bmd_file = malloc(sizeof(BMD_File));
  bmd_file->header = bmd_header;

  unsigned int frames_section_len = read_section_header(fp);
  bmd_file->frame_info = read_frames_section(fp, frames_section_len);

  unsigned int pixels_section_len = read_section_header(fp);
  bmd_file->pixels = malloc(pixels_section_len);
  fread(bmd_file->pixels, pixels_section_len, 1, fp);

  unsigned int rows_section_len = read_section_header(fp);
  bmd_file->row_info = read_rows_section(fp, rows_section_len);

  return bmd_file;
}

BMD_File* read_bmd(char* path) {
  FILE *fp = fopen(path, "r");
  BMD_File* bmd_file = read_struct(fp);
  fclose(fp);

  return bmd_file;
}

void free_bmd(BMD_File *bmd_file) {
  free(bmd_file->header);
  free(bmd_file->frame_info);
  free(bmd_file->row_info);
  free(bmd_file->pixels);
  free(bmd_file);
}
