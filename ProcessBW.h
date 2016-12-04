/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */

#ifndef PROCESS_BW_H
#define PROCESS_BW_H

#include "ImageBW.h"
#include <math.h>
using namespace std;


/* This active class provide a mode to processing a generic ImageBW.
 * So, it offers generic filters for black/white image processing.
 * It includes the possibility of insert a kernel for the convolution
 * filter family.
 */
class ProcessBW {

 private:
  /* source image */
  ImageBW* source;
  /* destination image */
  ImageBW* dest;
  /* dimension of the kernel (kd * kd) */
  int kd;
  /* offset for the kernel */
  int ofs;
  /* the scale value *sum of the kernel */
  int scale;
  /* kernel */
  int* kernel;
  /* used by the convolution pixel */
  int pixel(unsigned char*, int, int);

 public:
  /* 'ctors */
  ProcessBW(ImageBW* = 0, ImageBW* = 0);
  /* FILTERS */
  /* increase the contrast of source and save in dest */
  ImageBW* contrast(float, float);
  /* apply a gamma_correction filter with gamma parameter to source and save in dest */
  ImageBW* gammaCorrection(float);
  /* calculates parameters a and b for the contrast filter */
  void calculate_A_B(float*, float*);
  /* calculate the gamma value, so the histogram barycentre of source image became value. */
  float calculateGamma(float);
  /* apply a convolution filter. It depends of the kernel used */
  ImageBW* convolution();
  /* MIX FILTERS */
  /* filter request */
  ImageBW* projectFilter();
  /* set the destination image */
  void setDest(ImageBW*);
  /* set the source and destination images */
  void setImages(ImageBW*, ImageBW*);
  /* get the source image */
  ImageBW* getSource() const;
  /* get the destination image */
  ImageBW* getDest() const;
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
  ~ProcessBW();

}; //end class ProcessBW

#endif
