#ifndef TYPES_H
#define TYPES_H

typedef struct Header {
  int version;
  int dirs_no;
  int files_no;
} Header;

typedef struct DirInfo {
  char* path;
  int   depth;
} DirInfo;

typedef struct FileInto {
  char* path;
  int   offset;
  int   length;
} FileInfo;

typedef struct CIF_3FD {
  short int id;   // 0x03FD
  char unknown0[6]; // (0)
  int unknown1; // (1)
  int NrOfEntries; // number of text entries
  int NrOfEntries_dup1; //  The "NrOfEntries" again
  int NrOfEntries_dup2; //  The "NrOfEntries" again
  int SizeOfTextTable;
  int Unk2; // (0x03E9)
  int Unk3; // (0)
  int SizeOfIndexTable; // = NrOfEntries * 4
  // int ENCODED_INDEXTABLE[];
  // char Unk4; // (1)
  // int Unk5; // (0x03E9)
  // int Unk6; // (0)
  // int SizeOfTextTable_dup1; // The "SizeOfTextTable" again
  // char* ENCODED_TEXTTABLE;
} CIF_3FD;

typedef struct CIF_041 {
  short int id;   // 0x0041
  int Unk0; // (1)
  int NrOfEntries; // number of text entries
  int NrOfEntries_dup1; //  The "NrOfEntries" again
  int Unk1; // (0x0A)
  int SizeOfIndexTable; // = NrOfEntries * 4
  // int ENCODED_INDEXTABLE[];
  // short int Unk2; // (0x0001)
  // int Unk3; // (0x0000000A)
  // int SizeOfTextTable;
  // char* ENCODED_TEXTTABLE;
} CIF_041;

#endif