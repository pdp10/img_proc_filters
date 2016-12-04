/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */

#include "Loader.h"


/* load filename and put the content in the array file of dim dimension */
bool Loader::read(char* filename, unsigned char* file, int dim) {
  FILE* fin;
  if(!(fin = fopen(filename, "rb"))) {
    printf("Error: %s not loaded\n", filename);
    return false;
  }
  fread(file, dim, 1, fin);
  fclose(fin);
  return true;
}

/* write filename and put the content in the array file of dim dimension */
bool Loader::write(char* filename, unsigned char* file, int dim) {
  FILE* fout;
  if(!(fout = fopen(filename, "wb"))) {
    printf("Error: %s not loaded\n", filename);
    return false;
  }
  fwrite(file, dim, 1, fout);
  fclose(fout);
  return true;
}

/* append filename and put the content in the array file of dim dimension */
bool Loader::append(char* filename, unsigned char* file, int dim) {
  FILE* fout;
  if(!(fout = fopen(filename, "ab"))) {
    printf("Error: %s not loaded\n", filename);
    return false;
  }
  fwrite(file, dim, 1, fout);
  fclose(fout);
  return true;
}

