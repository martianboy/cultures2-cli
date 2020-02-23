#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "utils/sort.h"

char* read_string(FILE *fp) {
  int len;
  fread(&len, 4, 1, fp);

  char* buf = (char *) malloc(len + 1);
  fread(buf, len, 1, fp);
  buf[len] = 0;

  return buf;
}

void read_dir_info(FILE* fp) {
  char* path = read_string(fp);

  int depth;
  fread(&depth, 4, 1, fp);

  // printf("%d %s\n", depth, path);

  free(path);
}

void convert_to_unix_paths(char* path) {
  for (char *c = path; *c != 0; c++) {
    if (*c == '\\') *c = '/';
  }
}

struct FileInto* read_file_info(FILE* fp) {
  char* path = read_string(fp);
  convert_to_unix_paths(path);

  int offset; fread(&offset, 4, 1, fp);
  int length; fread(&length, 4, 1, fp);

  // printf("%8X %8X %s\n", offset, length, path);

  FileInfo* info = (FileInfo *) malloc(sizeof(FileInfo));
  info->path = path;
  info->offset = offset;
  info->length = length;

  return info;
}

void free_file_info_array(FileInfo **files, int len) {
  for (int i = 0; i < len; i++) {
    free(files[i]->path);
    free(files[i]);
  }

  free(files);
}

void decode_cif(unsigned char *buf, int len) {
  char B = 0;
  int C = 71, D = 126;

  for (int i = 0; i < len; i++) {
    B = buf[i] - 1;
    B = B ^ C;
    C += D;
    D += 33;

    buf[i] = B;
  }
}

void convert_3fd_format(unsigned char* buf) {
  CIF_3FD *cif_header = (CIF_3FD *) buf;
  decode_cif(&buf[40], cif_header->SizeOfIndexTable);
  decode_cif(&buf[40 + cif_header->SizeOfIndexTable + 13], cif_header->SizeOfTextTable);
}

void convert_041_format(unsigned char* buf) {
  CIF_041 *cif_header = (CIF_041 *) buf;
  decode_cif(&buf[22], cif_header->SizeOfIndexTable);

  int SizeOfTextTable = *((int *) &buf[22 + cif_header->SizeOfIndexTable + 6]);
  decode_cif(&buf[22 + cif_header->SizeOfIndexTable + 6 + SizeOfTextTable], SizeOfTextTable);
}

void extract_file(FileInfo *info, FILE *fp) {
  FILE *of = fopen(info->path, "w");

  unsigned char *buf = malloc(info->length);
  fread(buf, info->length, 1, fp);

  if (buf[0] == 0xFD && buf[1] == 0x03) {
    convert_3fd_format(buf);
  }
  if (buf[0] == 0x41 && buf[1] == 0x00) {
    convert_041_format(buf);
  }

  fwrite (buf, info->length, 1, of);
  free(buf);
  fclose(of);
}

int main() {
  FILE *fp = fopen("/mnt/c/Games/Cultures2/DataX/Libs/data0001.lib", "r");
  Header header;
  fread(&header, sizeof(Header), 1, fp);

  printf("Hey!\n");
  printf("%d %d %d\n", header.version, header.dirs_no, header.files_no);

  int i;

  printf("reading directory info...");
  for (i = 0; i < header.dirs_no; i++) {
    read_dir_info(fp);
  }

  FileInfo **files = (FileInfo **) malloc(header.files_no * sizeof(FileInfo *));

  printf("reading file info...\n");
  for (i = 0; i < header.files_no; i++) {
    files[i] = read_file_info(fp);
  }

  printf("sorting files...\n");
  mergeSort(files, 0, header.files_no - 1);

  for (i = 0; i < header.files_no; i++) {
    extract_file(files[i], fp);
    printf("%8X %8X %s\n", files[i]->offset, files[i]->length, files[i]->path);
  }

  free_file_info_array(files, header.files_no);
  printf("%X\n", (unsigned int) ftell(fp));
  fclose(fp);
  return 0;
}
