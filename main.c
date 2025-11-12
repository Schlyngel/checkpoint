#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

char checkdir[] = ".check";

int createRepo() {
  
  int repo;
  repo = mkdir(checkdir, 0777);

  if (repo == 0) {
    printf("checkpoint repo has been created\n");
    return 0;
  } else if (repo == -1 & errno == 17) {
    printf("repo exists already. moving on!\n");
    return 0;
  } else {
    printf("something went wrong... i'mma head out.\n");
    return 1;
  }

}

int createMetaFile() {
  FILE* metaFile;
  metaFile = fopen(".check/meta.check", "r");

  if (metaFile == NULL) {    
    metaFile = fopen(".check/meta.check", "w");
    fprintf(metaFile,"meta file!");
    
    printf("meta file created\n");
    
    fclose(metaFile);
    return 0;
  } else {
    printf("meta file exists, so moving on\n");
    fclose(metaFile);
    return 0;
  }

  printf("something somewhere went really wrong...");
  fclose(metaFile); // this might throw a segfault but I can't prove it
  return 1;
}

void getFileAndPut() {
  FILE* main;
  int64_t fileSize;

  main = fopen("main.c", "r");

  fseek(main, 0, SEEK_END);
  fileSize = ftell(main);
  rewind(main);
  char* fileContent = (char*)malloc(fileSize);
  fread(fileContent, 1, fileSize, main);

  fclose(main);
  main = fopen(".check/main.c.check", "w");

  fprintf(main, fileContent);

  free(fileContent);
  fclose(main);
}

int main(int argc, char* argv[]) {
  int repo;

  repo = createRepo();

  if (repo != 0) {
    return 1;
  }

  int metaFile;

  metaFile = createMetaFile();
  if (metaFile == 1) return 1;

  getFileAndPut();

  return 0;
}
