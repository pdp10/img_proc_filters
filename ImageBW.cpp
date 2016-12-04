/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */


#include "ImageBW.h"
#include <iostream>

/* calculate the brightness histogram and the barycentre */
void ImageBW::brightHisto() {
  int i, v;
  minHisto = 255;              // forces to calculate
  maxHisto = 0;
  bc = 0.0f;
  /* initialize */
  for(i = 0; i < hd; i++) {
    bHisto[i] = 0.0;
  }
  /* create histogram */
  for(i = 0; i < width * height; i++) {
    v = image[i];
    bHisto[v] += 1.0;
    if(v < minHisto) minHisto = v;
    if(v > maxHisto) maxHisto = v;
  }
  /* calculate the histogram barycentre and normalize */
  for(i = 0; i < hd; i++) {
    bHisto[i] = bHisto[i] / (width * height);   // normalize
    bc = bc + ((float)i * bHisto[i]);
  }
}


/* 'ctors */
ImageBW::ImageBW(unsigned char* _image, int _width, int _height) : hd(256) {
  if(_image != 0 && _width > 0 && _height > 0) {
    bHisto = new float[hd];
    image = _image;
    width = _width;
    height = _height;
    /* Initialize minHisto, maxHisto, bc, bHisto */
    brightHisto();
  } else {
    image = 0;
    height = width = 0;
    minHisto = maxHisto = 0;
    bc = 0;
    bHisto = 0;
  }
}

/* returns the minimum of the histogram */
int ImageBW::getMinHisto() const { return minHisto; }

/* returns the maximum of the histogram */
int ImageBW::getMaxHisto() const { return maxHisto; }

/* returns the width of the source and destination image */
int ImageBW::getWidth() const { return width; }

/* returns the height of the source and destination image */
int ImageBW::getHeight() const { return height; }

/* returns the pointer to the source image */
unsigned char* ImageBW::getImage() const { return image; }

/* returns the brightness histogram */
float* ImageBW::getBrightHisto() const { return bHisto; }

/* returns the histogram barycentre */
float ImageBW::getHistoBc() const { return bc; }

/* returns the histogram dimension */
int ImageBW::getHistoDim() const { return hd; }

/* returns the distance of the value from the histogram barycentre */
float ImageBW::getBcDist(float _value) const { return _value - bc; }

/* set an image of width * height pixels */
void ImageBW::setImage(unsigned char* _image, int _width, int _height) {
  if(_image != 0 && _width > 0 && _height > 0) {
    image = _image;
    width = _width;
    height = _height;
    /* Update minHisto, maxHisto, bc, bHisto */
    brightHisto();
  }
}



/* destructor */
ImageBW::~ImageBW() {
  if(bHisto != 0) {
    delete[] bHisto;
  }
}
