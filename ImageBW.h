/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */


#ifndef IMAGE_BW_H
#define IMAGE_BW_H

#include "ImageBW.h"
using namespace std;

/* This class defines a generic black/white image.
 * In particular it contains a brightness histogram and
 * its fundamentals data fields.
 */
class ImageBW {

 private:
  /* dimension of the brightness histogram */
  int hd;
  /* brightness histogram normalized */
  float* bHisto;
  /* minimum of the histogram */
  int  minHisto;
  /* maximum of the histogram */
  int  maxHisto;
  /* histogram barycentre */
  float bc;
  /* width * height pixels */
  int width;
  int height;
  /* the image */
  unsigned char* image;

 public:
  /* 'ctors */
  ImageBW(unsigned char* = 0, int = 0, int = 0);
  /* calculate the brightness histogram and the barycentre */
  void brightHisto();
  /* returns the minimum of the histogram */
  int getMinHisto() const;
  /* returns the maximum of the histogram */
  int getMaxHisto() const;
  /* returns the width of the source and destination image */
  int getWidth() const;
  /* returns the height of the source and destination image */
  int getHeight() const;
  /* returns the pointer to the source image */
  unsigned char* getImage() const;
  /* returns the brightness histogram */
  float* getBrightHisto() const;
  /* returns the histogram barycentre */
  float getHistoBc() const;
  /* returns the histogram dimension */
  int getHistoDim() const;
  /* returns the distance of the value from the histogram barycentre */
  float getBcDist(float) const;
  /* set an image of width * height pixels */
  void setImage(unsigned char*, int, int);
  /* Destructor */
  ~ImageBW();

}; //end class Image


#endif
