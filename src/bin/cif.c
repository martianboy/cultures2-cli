#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"

void read_struct(FILE *fp) {
  CIF_3FD cif_header;
  fread(&cif_header, sizeof(CIF_3FD), 1, fp);
  
  unsigned int index_table[cif_header.NrOfEntries];
  fread(&index_table, 4, cif_header.NrOfEntries, fp);
  fseek(fp, 1 + 4 + 4 + 4, SEEK_CUR);

  char text_table[cif_header.SizeOfTextTable];
  fread(&text_table, cif_header.SizeOfTextTable, 1, fp);

  for (int i = 0; i < cif_header.NrOfEntries; i++) {
    if (text_table[index_table[i]] == 1)
      printf("\n[%s]\n", &text_table[index_table[i] + 1]);
    else
      printf("%X %s\n", text_table[index_table[i]], &text_table[index_table[i] + 1]);
  }
}

void read_cif(char* path) {
  FILE *fp = fopen(path, "r");
  read_struct(fp);
  fclose(fp);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: bin/cif <path/to/file.cif>\n");
    return 0;
  }

  read_cif(argv[1]);
  return 0;
}
