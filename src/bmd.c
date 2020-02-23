#include <stdio.h>
#include <string.h>
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

  // for (int i = 0; i < len / 4; i++) {
  //   printf("Row %d:\n", i);

  //   printf("%6s: %10u\n", "indent", rows[i].indent);
  //   printf("%6s: %10X\n", "offset", rows[i].offset);
  // }

  return rows;
}

BMD_File* read_struct(FILE *fp) {
  BMD_Header *bmd_header = malloc(sizeof(BMD_Header));
  fread(bmd_header, sizeof(BMD_Header), 1, fp);

  BMD_File *bmd_file = malloc(sizeof(BMD_File));
  bmd_file->header = bmd_header;

  // printf("\nSection 1:\n");
  unsigned int frames_section_len = read_section_header(fp);
  bmd_file->frame_info = read_frames_section(fp, frames_section_len);

  // printf("\nSection 2:\n");
  unsigned int pixels_section_len = read_section_header(fp);
  bmd_file->pixels = malloc(pixels_section_len);
  fread(bmd_file->pixels, pixels_section_len, 1, fp);

  // printf("\nSection 3:\n");
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

void print_bmd_structure(BMD_File *bmd_file) {
  printf("%10s: %10X\n", "magic", bmd_file->header->magic);
  printf("%10s: %10d\n", "zero_0", bmd_file->header->zero_0);
  printf("%10s: %10d\n", "zero_1", bmd_file->header->zero_1);
  printf("%10s: %10d\n", "num_frames", bmd_file->header->num_frames);
  printf("%10s: %10d\n", "num_pixels", bmd_file->header->num_pixels);
  printf("%10s: %10d\n", "num_rows", bmd_file->header->num_rows);
  printf("%10s: %10d\n", "unknown_1", bmd_file->header->unknown_1);
  printf("%10s: %10d\n", "unknown_2", bmd_file->header->unknown_2);

  printf("\nSection 1:\n");
  for (int i = 0; i < bmd_file->header->num_frames; i++) {
    printf("\nFrame %d:\n", i);
    printf("%10s: %10X\n", "type", bmd_file->frame_info[i].type);
    printf("%10s: %10X\n", "meta_1", bmd_file->frame_info[i].meta_1);
    printf("%10s: %10X\n", "meta_2", bmd_file->frame_info[i].meta_2);
    printf("%10s: %10u\n", "width", bmd_file->frame_info[i].width);
    printf("%10s: %10u\n", "len", bmd_file->frame_info[i].len);
    printf("%10s: %10u\n", "off", bmd_file->frame_info[i].off);
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: bin/bmd <path/to/file.bmd>\n");
    return 0;
  }

  BMD_File *bmd_file = read_bmd(argv[1]);
  print_bmd_structure(bmd_file);

  free_bmd(bmd_file);

  return 0;
}
