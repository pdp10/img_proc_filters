/*
 * Course: Elaborazione delle Immagini
 * Univeristy: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */

#ifndef PROCESS_RGB_INTERLEAVED_H
#define PROCESS_RGB_INTERLEAVED_H

#include "ImageBW.h"
#include "ImageRGBInterleaved.h"
#include "ProcessBW.h"
#include <math.h>
using namespace std;


/* This class provides some filters for rgb interleaved images. */
class ProcessRGBInterleaved {

 private:
  /* source image */
  ImageRGBInterleaved* source;
  /* destination image */
  ImageRGBInterleaved* dest;
  /* dimension of the kernel (kd * kd) */
  int kd;
  /* offset for the kernel */
  int ofs;
  /* the scale value *sum of the kernel */
  int scale;
  /* kernel */
  int* kernel;
  /* to use for some filters */
  ProcessBW* pBW;
  /* used by the convolution pixel */
  int pixel(unsigned char**, int, int, int);
  /* set the image data field of the super class. it chooses
  * the maximum intensity among r, g, b intensity. */
  void calculateIntensity(unsigned char*);
  /* calculate the color of dest. 
   * It is used by some filter */
  void calculateColour(ImageBW*, ImageBW*);

 public:
  /* 'ctors */
  ProcessRGBInterleaved(ImageRGBInterleaved* = 0, ImageRGBInterleaved* = 0);

  /* FILTERS */
  /* increase the contrast of source and save in dest */
  ImageRGBInterleaved* contrast(float, float);
  /* apply a gamma_correction filter with gamma parameter to source and save in dest */
  ImageRGBInterleaved* gammaCorrection(float);
  /* apply a convolution filter. It depends of the kernel used */
  ImageRGBInterleaved* convolution();
  /* MIX FILTERS */
  /* filter request */
  ImageRGBInterleaved* projectFilter();
  /* set the destination image */
  void setDest(ImageRGBInterleaved*);
  /* set the source and destination images */
  void setImages(ImageRGBInterleaved*, ImageRGBInterleaved*);
  /* get the source image */
  ImageRGBInterleaved* getSource() const;
  /* get the destination image */
  ImageRGBInterleaved* getDest() const;
  /* get the process for BW */
  ProcessBW* getProcessBW() const;
  /* KERNEL */  
  /* returns the pointer to the kernel used. */
  int* getKernel() const;
  /* returns the dimension of the kernel side */
  int getKernelDim() const;
  /* returns the scale of the kernel */
  int getKernelScale() const;
  /* set a new kernel. kd is the dimension for the _kernel */
  void setKernel(int*, int, int);
  /* destructor */
  ~ProcessRGBInterleaved();

}; //end class ProcessRGBInterleaved

#endif
