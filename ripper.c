#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

const char* DEFAULT_WAD_LOCATION_OSX = "/Volumes/Hotline Miami 2 Wrong Number/Hotline Miami 2 Wrong Number.app/Contents/Resources/game/HotlineMiami2.app/Contents/Resources/hlm2_data_desktop.wad";
char* inputPath = NULL;

struct WADHeaderStruct {
  int32_t count;
} typedef WADHeader;

struct FileHeaderStruct {
  char* name;
  int32_t nameLength;
  int64_t offset;
  int64_t length;
} typedef FileHeader;

int mkdirtree(const char* fileName) {
  char* path = (char*)fileName;
  char* end = strstr(path, "/");
  while(end) {
    int len = end-path;
    char name[len+1];
    memcpy(name, path, end-path);
    name[len] = 0x00;

    mkdir(name, 0755);

    end = strstr(path + len + 1, "/");
  }

  return 0;
}

int extract() {
  FILE* file = fopen(inputPath, "r");

  if(!file) {
    return -1;
  }

  // Read wad header (= get number of files)
  WADHeader header;
  fread(&header, sizeof(header.count), 1, file);

  FileHeader fileHeader[header.count];
  char* fileNameBuffer;
  for(unsigned int i=0; i<header.count; i++) {
    // Read length of filename
    fread(&fileHeader[i].nameLength, sizeof(fileHeader[i].nameLength), 1, file);

    // Read filename itself
    fileHeader[i].name = (char*)malloc(fileHeader[i].nameLength + 1);
    fread(fileHeader[i].name, fileHeader[i].nameLength, 1, file);
    fileHeader[i].name[fileHeader[i].nameLength] = 0x00;

    // Read length and offset
    fread(&fileHeader[i].length, sizeof(fileHeader[i].length), 1, file);
    fread(&fileHeader[i].offset, sizeof(fileHeader[i].offset), 1, file);

    // Prepare output folder structure
    mkdirtree(fileHeader[i].name);
  }

  // Dump all files to disk
  char buffer[1024];
  for(unsigned int i=0; i<header.count; i++) {
    FILE* of = fopen(fileHeader[i].name, "w");
    int bytesRemaining = fileHeader[i].length;
    size_t bytes_read = 0;
    while(bytes_read != bytesRemaining) {
      if(bytesRemaining <= sizeof(buffer)) {
        bytes_read = bytesRemaining;
        bytesRemaining = 0;
      }
      else {
        bytes_read = sizeof(buffer);
        bytesRemaining -= sizeof(buffer);
      }
      bytes_read = fread(&buffer, sizeof(char), bytes_read, file);
      fwrite(buffer, bytes_read, 1, of);
    }
    printf("+ wrote file #%d: %s\n", i+1, fileHeader[i].name);
    free(fileHeader[i].name);
    fclose(of);
  }

  fclose(file);
  return 0;
}

int autoDetect() {
  FILE* file = fopen(DEFAULT_WAD_LOCATION_OSX, "r");

  if(file) {
    inputPath = (char*)DEFAULT_WAD_LOCATION_OSX;
    fclose(file);
    return 1;
  }

  return 0;
}

int main(int argc, const char *argv[]) {
  inputPath = (char*)argv[1];
  if(argc != 2 && !autoDetect()) {
    printf("Usage: \n");
    return -1;
  }

  int ret = extract();
  if(ret < 0) {
    printf("Failed to extract!\n");
  }
  else {
    printf("Unpacking succeeded!\n");
  }

  return ret;
}
