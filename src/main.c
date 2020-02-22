#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "sort.h"

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

int main() {
  FILE *fp = fopen("/mnt/c/Games/Cultures2/DataX/Libs/data0001.lib", "r");
  Header header;
  fread(&header, sizeof(Header), 1, fp);

  printf("Hey!\n");
  printf("%d %d %d\n", header.version, header.dirs_no, header.files_no);

  int i;

  for (i = 0; i < header.dirs_no; i++) {
    read_dir_info(fp);
  }

  FileInfo **files = (FileInfo **) malloc(header.files_no * sizeof(FileInfo *));

  for (i = 0; i < header.files_no; i++) {
    files[i] = read_file_info(fp);
  }

  mergeSort(files, 0, header.files_no - 1);

  for (i = 0; i < header.files_no; i++) {
    printf("%8X %8X %s\n", files[i]->offset, files[i]->length, files[i]->path);
  }

  free_file_info_array(files, header.files_no);
  printf("%X\n", (unsigned int) ftell(fp));
  fclose(fp);
  return 0;
}
