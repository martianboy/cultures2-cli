#ifndef TYPES_H
#define TYPES_H

typedef unsigned int u4;
typedef unsigned short u2;

typedef struct Header {
  unsigned int version;
  unsigned int dirs_no;
  unsigned int files_no;
} Header;

typedef struct DirInfo {
  unsigned char* path;
  unsigned int   depth;
} DirInfo;

typedef struct FileInto {
  unsigned char* path;
  unsigned int   offset;
  unsigned int   length;
} FileInfo;

typedef struct CIF_3FD {
  unsigned short int id;   // 0x03FD
  unsigned char unknown0[6]; // (0)
  unsigned int unknown1; // (1)
  unsigned int NrOfEntries; // number of text entries
  unsigned int NrOfEntries_dup1; //  The "NrOfEntries" again
  unsigned int NrOfEntries_dup2; //  The "NrOfEntries" again
  unsigned int SizeOfTextTable;
  unsigned int Unk2; // (0x03E9)
  unsigned int Unk3; // (0)
  unsigned int SizeOfIndexTable; // = NrOfEntries * 4
//  int ENCODED_INDEXTABLE[];
//  char Unk4; // (1)
//  int Unk5; // (0x03E9)
//  int Unk6; // (0)
//  int SizeOfTextTable_dup1; // The "SizeOfTextTable" again
//  char* ENCODED_TEXTTABLE;
} CIF_3FD;

typedef struct CIF_041 {
  unsigned short int id;   // 0x0041
  unsigned int Unk0; // (1)
  unsigned int NrOfEntries; // number of text entries
  unsigned int NrOfEntries_dup1; //  The "NrOfEntries" again
  unsigned int Unk1; // (0x0A)
  unsigned int SizeOfIndexTable; // = NrOfEntries * 4
//  int ENCODED_INDEXTABLE[];
//  short int Unk2; // (0x0001)
//  int Unk3; // (0x0000000A)
//  int SizeOfTextTable;
//  char* ENCODED_TEXTTABLE;
} CIF_041;
#endif
