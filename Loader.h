/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */


#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
using namespace std;

/* This class provides to open and save a file in the file system using static methods. 
 * On details, it read with -rb access, write with -wb accesso and can append with -ab
 * access. All these methods return true if they can open/save the file, otherwise 
 * return false.*/
class Loader {

 public:
  /* load filename and put the content in the array file of dim dimension */
  static bool read(char*, unsigned char*, int); 
  /* write filename and put the content in the array file of dim dimension */
  static bool write(char*, unsigned char*, int);
  /* append filename and put the content in the array file of dim dimension */
  static bool append(char*, unsigned char*, int);

}; //end class Loader

#endif
