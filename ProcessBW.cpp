/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */

#include "ProcessBW.h"
#include <iostream>

/* used by the convolution pixel */
int ProcessBW::pixel(unsigned char* imageS, int x, int y) {
  int u, v;
  int a;
  int p = 0;
  for(v = -ofs; v <= ofs; v++) {
    if(y + v < 0 || y + v >= source->getHeight()) continue;
    for(u = -ofs; u <= ofs; u++) {
      if(x + u < 0 || x + u >= source->getWidth()) continue;
      a = imageS[(x + u) + ((y + v) * source->getWidth())];
      p += (a * kernel[u + ofs + ((v + ofs) * kd)]);
    }
  }
  if(scale != 0) {   // blur filter
    p /= scale;
    if(p < 0) p = 0;
  } else {          //sharpen or edge detect
    if(p < 0) p = -p;
  }
  if(p > 255) p = 255;
  return p;
}


/* 'ctors */
ProcessBW::ProcessBW(ImageBW* _source, ImageBW* _dest) {
  source = _source;
  dest = _dest;
  kd = 0;
  ofs = 0;
  kernel = 0;
  scale = 0;
}


/* FILTERS */

/* increase the contrast of source and save in dest */
ImageBW* ProcessBW::contrast(float a, float b) {
  int x, y;
  int v;
  unsigned char* imageS = source->getImage();
  unsigned char* imageD = dest->getImage();
  for(y = 0; y < source->getHeight(); y++) {
    for(x = 0; x < source->getWidth(); x++) {
      v = imageS[x + (y * source->getWidth())];
      /* the formula for the contrast filter */
      v = (int)round(a + b * v);
      if(v > 255) v = 255; 
      if(v < 0) v = 0; 
      imageD[x + (y * source->getWidth())] = v;
    }
  }
  dest->brightHisto();
  return dest;
}

/* apply a gamma_correction filter with gamma parameter to source and save in dest */
ImageBW* ProcessBW::gammaCorrection(float gamma) {
  unsigned char* imageS = source->getImage();
  unsigned char* imageD = dest->getImage();
  int x, y;
  float v;
  for(y = 0; y < source->getHeight(); y++) {
    for(x = 0; x < source->getWidth(); x++) {
      v = (float)imageS[x + (y * source->getWidth())];
      /* the formula for the gamma correction filter */
      v = pow(v / 255.0f, gamma) * 255.0f;
      if(v > 255.0f) v = 255.0f; 
      if(v < 0.0f) v = 0.0f;
      imageD[x + (y * source->getWidth())] = (int)round(v);
    }
  }
  dest->brightHisto();
  return dest;
}

/* calculates parameters a and b for the contrast filter */
void ProcessBW::calculate_A_B(float* a, float* b) {
  *b =  ( 255.0f / (float)( source->getMaxHisto() - source->getMinHisto() ) );
  *a =  -( (*b) * (float) source->getMinHisto() );
}

/* calculate the gamma value, so the histogram barycentre of source image became value. */
float ProcessBW::calculateGamma(float value) {
  float gamma = 0.0f;
  if(value > 0.0f && source->getHistoBc() > 0.0f) {
    gamma = ( log( value/255.0f )  / log( source->getHistoBc()/255.0f ) );  // == logBC (value)
  } else {
    gamma = 1.0;
  }
  cout << "\ngamma = " << gamma << endl;
  return gamma;
}


/* apply a convolution filter. It depends of the kernel used */
ImageBW* ProcessBW::convolution() {
  if(kernel != 0) {
    unsigned char* imageS = source->getImage();
    unsigned char* imageD = dest->getImage();
    int x, y;
    int v;
    for(y = 0; y < source->getHeight(); y++) {
      for(x = 0; x < source->getWidth(); x++) {
	/* calculate using the kernel and the source image, 
	 * the pixel of the destination image */ 
	imageD[x + (y * source->getWidth())] = pixel(imageS, x, y);
      }
    }
  }
  dest->brightHisto();
  return dest;
}


/* MIX FILTERS */

/* It sequentially applies:
 * 1) a contrast filter.
 * 2) a gamma correction filter.
 * 3) a sharpen convolution filter.
 */
ImageBW* ProcessBW::projectFilter() {
  float a = 0.0f, b = 0.0f;
  ImageBW* firstS = source, * firstD = dest;

  /* apply a contrast filter that maximize the excursion of the 
   * minimum and maximum intensity of the source image */ 
  calculate_A_B(&a, &b);
  contrast(a, b); 

  source = dest;
  dest = new ImageBW( 
		   new unsigned char[source->getWidth() * source->getHeight()], 
		   source->getWidth(), source->getHeight());

  /* apply a gamma correction filter that balance the histogram baricenter
   * of the source to the standard value of 128.0 */  
  gammaCorrection(calculateGamma(128.0));

  source = dest;
  dest = firstD;

  /* apply a sharpen filter to improve the visibility of details */
  convolution();

  delete[] source->getImage();
  delete source;
  source = firstS;

  return dest;
}

/* set the destination image */
void ProcessBW::setDest(ImageBW* _dest) {
  dest = _dest;
}

/* set the source and destination images */
void ProcessBW::setImages(ImageBW* _source, ImageBW* _dest) {
  source = _source;
  dest = _dest;
}

/* get the source image */
ImageBW* ProcessBW::getSource() const { return source; }

/* get the destination image */
ImageBW* ProcessBW::getDest() const { return dest; }




/* KERNEL */  

/* returns the pointer to the kernel used. */
int* ProcessBW::getKernel() const { return kernel; }

/* returns the dimension of the kernel side */
int ProcessBW::getKernelDim() const { return kd; }

/* returns the scale of the kernel */
int ProcessBW::getKernelScale() const { return scale; }

/* set a new kernel. kd is the dimension for the _kernel */
void ProcessBW::setKernel(int* _kernel, int _kd, int _scale) {
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
ProcessBW::~ProcessBW() {
  if(kernel != 0) {
    delete[] kernel;
  }
}
