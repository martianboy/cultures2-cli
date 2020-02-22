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

#endif