/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */


#include "ImageRGBInterleaved.h"
#include <iostream>


/* 'ctors */
ImageRGBInterleaved::ImageRGBInterleaved(unsigned char* _image, int _width, int _height) {
  if(_image != 0 && _width > 0 && _height > 0) {
    int k = 0;
    width = _width;
    height = _height;
    imageRGB = new unsigned char*[width * height];
    for(int i = 0; i < width * height; i++) {
      imageRGB[i] = new unsigned char[3];
      for(int j = 0; j < 3; j++) {
	imageRGB[i][j] = _image[k];
	k++;
      }
    }
  } else {
    imageRGB = 0;
    height = width = 0;
  }
}


/* get a rgb image of width * height pixels */
unsigned char** ImageRGBInterleaved::getImage() const { return imageRGB; }

/* set an image of width * height pixels */
void ImageRGBInterleaved::setImage(unsigned char* _image, int _width, int _height) {
  if(_image != 0 && _width > 0 && _height > 0) {
    if(imageRGB != 0) {
      for(int i = 0; i < width * height; i++) {
	delete[] imageRGB[i];
      }
      delete[] imageRGB;
    }
    width = _width;
    height = _height;
    int k = 0;
    imageRGB = new unsigned char*[width * height];
    for(int i = 0; i < width * height; i++) {
      imageRGB[i] = new unsigned char[3];
      for(int j = 0; j < 3; j++) {
	imageRGB[i][j] = _image[k];
	k++;
      }
    }
  }
}

/* returns the width of the source and destination image */
int ImageRGBInterleaved::getWidth() const { return width; }

/* returns the height of the source and destination image */
int ImageRGBInterleaved::getHeight() const { return height; }


/* destructor */
ImageRGBInterleaved::~ImageRGBInterleaved() {
  if(imageRGB != 0) {
    for(int i = 0; i < width * height; i++) {
      delete[] imageRGB[i];
    }
    delete[] imageRGB;
  }
}
