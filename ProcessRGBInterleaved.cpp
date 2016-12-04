/*
 * Course: Elaborazione delle Immagini
 * Univeristy: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */

#include "ProcessRGBInterleaved.h"
#include <iostream>

/* used by the convolution pixel */
int ProcessRGBInterleaved::pixel(unsigned char** imageS, int x, int y, int color) {
  int u, v;
  int a;
  int p = 0;
  for(v = -ofs; v <= ofs; v++) {
    if(y + v < 0 || y + v >= source->getHeight()) continue;
    for(u = -ofs; u <= ofs; u++) {
      if(x + u < 0 || x + u >= source->getWidth()) continue;
      a = imageS[(x + u) + ((y + v) * source->getWidth())][color];
      p += (a * kernel[u + ofs + ((v + ofs) * kd)]);
    }
  }
  if(scale != 0) {   // blur filter
    p /= scale;
    if(p < 0) p = 0;
  } else {           //sharpen or edge detect
    if(p < 0) p = -p;
  }
  if(p > 255) p = 255;
  return p;
}

/* set the image data field of the super class. it chooses
 * the media intensity among r, g, b intensity. */
void ProcessRGBInterleaved::calculateIntensity(unsigned char* image) {
  unsigned char** imageRGB = source->getImage();
  for(int i = 0; i < source->getWidth() * source->getHeight(); i++) {
    image[i] = (int)round((imageRGB[i][0] + imageRGB[i][1] + imageRGB[i][2])/3);
  }
}

/* calculate the color of _image */
void ProcessRGBInterleaved::calculateColour(ImageBW* sourceBW, ImageBW* destBW) {
  int i, j, k = 0;
  float l;
  unsigned char* imageS      = sourceBW->getImage();
  unsigned char* imageD      = destBW->getImage();
  unsigned char** imageRGB_s = source->getImage();
  unsigned char** imageRGB_d = dest->getImage();
  for(i = 0; i < dest->getWidth() * dest->getHeight(); i++, k++) {
    for(j = 0; j < 3; j++) {
      l = imageRGB_s[i][j];
      if(imageS[k] == 0) {
	imageRGB_d[i][j] = (int)round(l);
      } else {
	l *= (float)imageD[k] / (float)imageS[k];
	if(l > 255.0f) {
	  l = 255.0f;
	}
	imageRGB_d[i][j] = (int)round(l);
      }
    }
  }
}


/* 'ctors */
ProcessRGBInterleaved::ProcessRGBInterleaved(ImageRGBInterleaved* _source, ImageRGBInterleaved* _dest) {
  if(_source != 0 && _dest != 0) {
    source = _source;
    dest = _dest;
    unsigned char* imageS = new unsigned char[source->getWidth() * source->getHeight()];
    unsigned char* imageD = new unsigned char[source->getWidth() * source->getHeight()];
    calculateIntensity(imageS);
    ImageBW* s = new ImageBW(imageS, source->getWidth(), source->getHeight());
    ImageBW* d = new ImageBW(imageD, source->getWidth(), source->getHeight());
    pBW = new ProcessBW(s, d);
  } else {
    source = 0;
    dest = 0;
    pBW = 0;
  }
  kd = 0;
  ofs = 0;
  kernel = 0;
  scale = 0;
}



/* FILTERS */

/* increase the contrast of source and save in dest */
ImageRGBInterleaved* ProcessRGBInterleaved::contrast(float a, float b) {
  pBW->contrast(a, b);
  calculateColour(pBW->getSource(), pBW->getDest());
  return dest;
}

/* apply a gamma_correction filter with gamma parameter to source and save in dest */
ImageRGBInterleaved* ProcessRGBInterleaved::gammaCorrection(float gamma) {
  pBW->gammaCorrection(gamma);
  calculateColour(pBW->getSource(), pBW->getDest());
  return dest;
}


/* apply a convolution filter. It depends of the kernel used */
ImageRGBInterleaved* ProcessRGBInterleaved::convolution() {
  if(kernel != 0) {
    /* on the three channels */
    unsigned char** imageS = source->getImage();
    unsigned char** imageD = dest->getImage();
    int x, y;
    int v;
    for(y = 0; y < source->getHeight(); y++) {
      for(x = 0; x < source->getWidth(); x++) {
	imageD[x + (y * source->getWidth())][0] = pixel(imageS, x, y, 0);
	imageD[x + (y * source->getWidth())][1] = pixel(imageS, x, y, 1);
	imageD[x + (y * source->getWidth())][2] = pixel(imageS, x, y, 2);
      }
    }
  }
  return dest;
}




/* MIX FILTERS */

/* filter requested */
/* It sequentially applies:
 * 1) a contrast filter.
 * 2) a gamma correction filter.
 * 3) a sharpen convolution filter.
 */
ImageRGBInterleaved* ProcessRGBInterleaved::projectFilter() {
  float a = 0, b = 0;
  ImageRGBInterleaved* firstS = source, * firstD = dest;

  /* apply a contrast filter that maximize the excursion of the 
   * minimum and maximum intensity of the source image */ 
  pBW->calculate_A_B(&a, &b);
  pBW->contrast(a, b);
  calculateColour(pBW->getSource(), pBW->getDest());

  /* setting to apply the 2nd filter */
  source = dest;
  /* Allocate a temporary image */
  unsigned char* tempImage = new unsigned char[source->getWidth() * source->getHeight() * 3];
  dest = new ImageRGBInterleaved(tempImage, source->getWidth(), source->getHeight());
  calculateIntensity(pBW->getSource()->getImage());

  /* apply a gamma correction filter that balance the histogram baricenter
   * of the source to the standard value of 128 */  
  pBW->gammaCorrection(pBW->calculateGamma(128.0));
  calculateColour(pBW->getSource(), pBW->getDest());

  /* setting to apply the 3rd filter */
  source = dest;
  dest   = firstD;

  /* apply a sharpen filter to improve the visibility of details */
  convolution();


  /* free the temporary image */
  delete[] tempImage;
  delete source;
  source = firstS;

  /* Associate with the first original image */
  calculateIntensity(pBW->getSource()->getImage());

  return dest;
}


/* set the destination image */
void ProcessRGBInterleaved::setDest(ImageRGBInterleaved* _dest) {
  dest = _dest;
}

/* set the source and destination images */
void ProcessRGBInterleaved::setImages(ImageRGBInterleaved* _source, ImageRGBInterleaved* _dest) {
  if(pBW != 0) {
    ImageBW* a = pBW->getSource();
    ImageBW* b = pBW->getDest();
    delete a->getImage();
    delete b->getImage();
    delete a;
    delete b;
    delete pBW;
  }
  source = _source;
  dest = _dest;
  kd = 0;
  ofs = 0;
  kernel = 0;
  scale = 0;
  unsigned char* imageS = new unsigned char[source->getWidth() * source->getHeight()];
  unsigned char* imageD = new unsigned char[source->getWidth() * source->getHeight()];
  calculateIntensity(imageS);
  ImageBW* s = new ImageBW(imageS, source->getWidth(), source->getHeight());
  ImageBW* d = new ImageBW(imageD, source->getWidth(), source->getHeight());
  pBW = new ProcessBW(s, d);
}
  
/* get the source image */
ImageRGBInterleaved* ProcessRGBInterleaved::getSource() const { return source; }

/* get the destination image */
ImageRGBInterleaved* ProcessRGBInterleaved::getDest() const { return dest; }

/* get the process for BW */
ProcessBW* ProcessRGBInterleaved::getProcessBW() const { return pBW; }

/* KERNEL */  

/* returns the pointer to the kernel used. */
int* ProcessRGBInterleaved::getKernel() const { return kernel; }

/* returns the dimension of the kernel side */
int ProcessRGBInterleaved::getKernelDim() const { return kd; }

/* returns the scale of the kernel */
int ProcessRGBInterleaved::getKernelScale() const { return scale; }

/* set a new kernel. kd is the dimension for the _kernel */
void ProcessRGBInterleaved::setKernel(int* _kernel, int _kd, int _scale) {
  if(_kd > 0) {
    if(kernel != 0) {
      delete[] kernel;
    }
    kd = _kd;
    ofs = ((kd - 1) / 2);
    kernel = _kernel;
    scale = _scale;
  }
}

/* destructor */
ProcessRGBInterleaved::~ProcessRGBInterleaved() { 
  if(kernel != 0) {
    delete[] kernel;
  }
  if(pBW != 0) {
    ImageBW* a = pBW->getSource();
    ImageBW* b = pBW->getDest();
    delete a->getImage();
    delete b->getImage();
    delete a;
    delete b;
    delete pBW;
  }
}
