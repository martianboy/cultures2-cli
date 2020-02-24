#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bmd.h"

void print_bmd_structure(BMD_File *bmd_file) {
  int i;

  printf("%10s: %10X\n", "magic", bmd_file->header->magic);
  printf("%10s: %10d\n", "zero_0", bmd_file->header->zero_0);
  printf("%10s: %10d\n", "zero_1", bmd_file->header->zero_1);
  printf("%10s: %10d\n", "num_frames", bmd_file->header->num_frames);
  printf("%10s: %10d\n", "num_pixels", bmd_file->header->num_pixels);
  printf("%10s: %10d\n", "num_rows", bmd_file->header->num_rows);
  printf("%10s: %10d\n", "unknown_1", bmd_file->header->unknown_1);
  printf("%10s: %10d\n", "unknown_2", bmd_file->header->unknown_2);

  printf("\nSection 1:\n");
  for (i = 0; i < bmd_file->header->num_frames; i++) {
    printf("\nFrame %d:\n", i);
    printf("%10s: %10X\n", "type", bmd_file->frame_info[i].type);
    printf("%10s: %10X\n", "meta_1", bmd_file->frame_info[i].meta_1);
    printf("%10s: %10X\n", "meta_2", bmd_file->frame_info[i].meta_2);
    printf("%10s: %10u\n", "width", bmd_file->frame_info[i].width);
    printf("%10s: %10u\n", "len", bmd_file->frame_info[i].len);
    printf("%10s: %10u\n", "off", bmd_file->frame_info[i].off);
  }

  printf("\nSection 3:\n");
  for (i = 0; i < bmd_file->header->num_rows; i++) {
    printf("%6d | %4d | %08X\n", i, bmd_file->row_info[i].indent, bmd_file->row_info[i].offset);
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
