/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */


#include <iostream>
#include "ImageBW.h"
#include "ImageRGBInterleaved.h"
#include "ProcessBW.h"
#include "ProcessRGBInterleaved.h"
#include "Loader.h"
using namespace std;



int main() {

  char filenameIn[20] = "rgb.raw";             // = "bw.raw";  = "rgb.raw"      //input file
  char filenameOut[20] = "output.raw";     //output file
  int width = 800, height = 600;       //pixels
  int val;
  int kd = 0, scale = 0;
  int* kernel = 0;
  float gamma, A, B;
  bool rgb = true, exit = false, ok;
  unsigned char* imageS = 0;
  unsigned char* imageD = 0;
  ImageBW* sourceBW = 0;
  ImageBW* destBW   = 0;
  ImageRGBInterleaved* sourceRGB = 0;
  ImageRGBInterleaved* destRGB = 0;
  ProcessBW* procBW = 0;
  ProcessRGBInterleaved* procRGB = 0;

  while(!exit) {
    ok = true;

    cout << "\n\nINSERTING DATA\n";
    cout << "------------------------------------------------\n";
    cout << "1)Insert the source image (extension .raw): ";
    cin >> filenameIn;
    cout << "2)Insert the destination image (extension .raw): ";
    cin >> filenameOut;
    cout << "3)Insert the width of the source image: ";
    cin >> width;
    cout << "4)Insert the height of the source image: ";
    cin >> height;
    cout << "5)Is an rgb interleaved image? (answer 1[yes], 0[false]) ";
    cin >> val;
    if(width <= 0) {
      ok = false;
      cout << "Error in data input 3: width must be positive\n";
    }
    if(height <= 0) {
      ok = false;
      cout << "Error in data input 4: height must be positive\n";
    }

    if(val == 0) { rgb = false; }
    else {
      if(val == 1) { rgb = true; }
      else { 
	ok = false; 
	cout << "Error in data input 5\n";
      }
    }

    if(ok) {
      cout << "\n\nPROCESSING DATA\n";
      cout << "------------------------------------------------\n"; 

      if(!rgb) {   //BW image
	imageS = new unsigned char[width * height];
	imageD = new unsigned char[width * height];
	/* Read the input file (black/white image)*/
	if(!Loader::read(filenameIn, imageS, width * height)) {
	  ok = false;
	} else {
	  sourceBW = new ImageBW(imageS, width, height);
	  destBW   = new ImageBW(imageD, width, height);
	  procBW   = new ProcessBW(sourceBW, destBW);
	}
      } else {     // RGB image
	imageS = new unsigned char[width * height * 3];
	imageD = new unsigned char[width * height * 3];
	/* Read the input file (rgb interleaved image)*/
	if(!Loader::read(filenameIn, imageS, width * height * 3)) {
	  ok = false;
	} else {
	  sourceRGB = new ImageRGBInterleaved(imageS, width, height);
	  destRGB   = new ImageRGBInterleaved(imageD, width, height);
	  procRGB   = new ProcessRGBInterleaved(sourceRGB, destRGB);
	}
      }
      if(ok) {
       
	cout << "\nChoose the filter (select the number):\n"
	     << "\t(1) Contrast filter\n"
	     << "\t(2) Gamma correction filter\n"
	     << "\t(3) Blur filter\n"
	     << "\t(4) Edge Detect filter\n"
	     << "\t(5) Sharpen filter\n"
	     << "\t(6) Project filter (1 + 2 + 5)\n"
	     << "\n\n Action? ";

	cin >> val;
	switch(val) {
	case 1: 
	  cout << "Please insert the A, B parameters (positive integers):\n"
	       << "A = ";
	  cin >> A;
	  cout << "B = ";
	  cin >> B;
	  if(A > 0 && B > 0) {
	    if(rgb) {
	      procRGB->contrast(A, B);
	    } else {
	      procBW->contrast(A, B);
	    }
	  } else {
	    cout << "Error: A and B must be positive\n";
	  }
	  break;
	case 2: 
	  cout << "Please insert the gamma parameters (positive float):\n"
	       << "gamma = ";
	  cin >> gamma;
	  if(gamma > 0) {
	    if(rgb) {
	      procRGB->gammaCorrection(gamma);
	    } else {
	      procBW->gammaCorrection(gamma);
	    }
	  } else {
	    cout << "Error: gamma must be positive\n";
	  }
	  break;
	case 3:
	    /* initialize a kernel for a blur filter */
	    kd = 3;
	    scale = 6;
	    kernel = new int[kd * kd];
	    kernel[0] = 0;
	    kernel[1] = 1;
	    kernel[2] = 0;
	    kernel[3] = 1;
	    kernel[4] = 2;
	    kernel[5] = 1;
	    kernel[6] = 0;
	    kernel[7] = 1;
	    kernel[8] = 0;
	  if(rgb) {
	    procRGB->setKernel(kernel, kd, scale);
	    procRGB->convolution();
	  } else {
	    procBW->setKernel(kernel, kd, scale);
	    procBW->convolution(); 
	  }
	  break;
	case 4:
	    /* initialize a kernel for a edge-detect filter */
	    kd = 3;
	    kernel = new int[kd * kd];
	    kernel[0] = 0;
	    kernel[1] = 1;
	    kernel[2] = 0;
	    kernel[3] = 1;
	    kernel[4] = -4;
	    kernel[5] = 1;
	    kernel[6] = 0;
	    kernel[7] = 1;
	    kernel[8] = 0;
	  if(rgb) {
	    procRGB->setKernel(kernel, kd, 0);
	    procRGB->convolution();
	  } else {
	    procBW->setKernel(kernel, kd, 0);
	    procBW->convolution(); 
	  }
	  break;
	case 5:
	    /* initialize a kernel for a sharpen filter */
	    kd = 3;
	    kernel = new int[kd * kd];
	    kernel[0] = 0;
	    kernel[1] = -1;
	    kernel[2] = 0;
	    kernel[3] = -1;
	    kernel[4] = 5;
	    kernel[5] = -1;
	    kernel[6] = 0;
	    kernel[7] = -1;
	    kernel[8] = 0;
	  if(rgb) {
	    procRGB->setKernel(kernel, kd, 0);
	    procRGB->convolution();
	  } else {
	    procBW->setKernel(kernel, kd, 0);
	    procBW->convolution(); 
	  }
	  break;
	case 6:
	    /* initialize a kernel for a sharpen filter */
	    kd = 3;
	    kernel = new int[kd * kd];
	    kernel[0] = 0;
	    kernel[1] = -1;
	    kernel[2] = 0;
	    kernel[3] = -1;
	    kernel[4] = 5;
	    kernel[5] = -1;
	    kernel[6] = 0;
	    kernel[7] = -1;
	    kernel[8] = 0;
	  if(rgb) {
	    procRGB->setKernel(kernel, kd, 0);
	    procRGB->projectFilter(); 
	  } else {
	    procBW->setKernel(kernel, kd, 0);
	    procBW->projectFilter(); 
	  }
	  break;
	default: cout << "Filter not defined\n";
	}
	
	if(!rgb) {
	  imageD = destBW->getImage();
	  /* Write the output file (black/white image) */
	  Loader::write(filenameOut, imageD, width * height);
	  /* Print data */
	  cout << "Source image:\n";
	  cout << "\tBarycentre:     " << procBW->getSource()->getHistoBc() << endl;
	  cout << "\tMinimum Histo:  " << procBW->getSource()->getMinHisto() << endl;
	  cout << "\tMaximum Histo:  " << procBW->getSource()->getMaxHisto() << endl;
	  cout << "\tPixels:         " << procBW->getSource()->getWidth() << " x " << procBW->getSource()->getHeight() << endl;
	  cout << "\nDestination image:\n";
	  cout << "\tBarycentre:     " << procBW->getDest()->getHistoBc() << endl;
	  cout << "\tMinimum Histo:  " << procBW->getDest()->getMinHisto() << endl;
	  cout << "\tMaximum Histo:  " << procBW->getDest()->getMaxHisto() << endl;
	  cout << "\tPixels:         " << procBW->getDest()->getWidth() << " x " << procBW->getDest()->getHeight() << endl;
	  delete[] imageS;
	  delete[] imageD;
	  delete sourceBW;
	  delete destBW;
	  delete procBW;

	} else {
	  /* deallocate the memory. imageS and imageD were copied in a matrix  */
	  delete[] imageS;
	  delete[] imageD;
	  /* copy imageBidimension in image_d monodimension */
	  unsigned char** imageBidimension = destRGB->getImage();
	  imageD = new unsigned char[width * height * 3];
	  int k = 0;
	  for(int i = 0; i < width * height; i++) {
	    for(int j = 0; j < 3; j++) {
	      imageD[k] = imageBidimension[i][j];
	      k++;
	    }
	  }
	  /* Write the output file (rgb interleaved image) */
	  Loader::write(filenameOut, imageD, width * height * 3);
	  /* Print data */
	  ProcessBW* p = procRGB->getProcessBW();
	  cout << "Source image:\n";
	  cout << "\tBarycentre:     " << p->getSource()->getHistoBc() << endl;
	  cout << "\tMinimum Histo:  " << p->getSource()->getMinHisto() << endl;
	  cout << "\tMaximum Histo:  " << p->getSource()->getMaxHisto() << endl;
	  cout << "\tPixels:         " << p->getSource()->getWidth() << " x " << p->getSource()->getHeight() << endl;
	  if(val != 3 && val != 4 && val != 5) {
	    cout << "\nDestination image:\n";
	    cout << "\tBarycentre:     " << p->getDest()->getHistoBc() << endl;
	    cout << "\tMinimum Histo:  " << p->getDest()->getMinHisto() << endl;
	    cout << "\tMaximum Histo:  " << p->getDest()->getMaxHisto() << endl;
	    cout << "\tPixels:         " << p->getDest()->getWidth() << " x " << p->getDest()->getHeight() << endl;
	  }
	  /* deallocate image_d */
	  delete[] imageD;
	  delete sourceRGB;
	  delete destRGB;
	  delete procRGB;
	}
      }
      cout << "------------------------------------------------\n"; 
    }
 
    cout << "Do you want to quit? (answer: 1[yes] / 0[no]) ";
    cin >> val;
    if(val != 0) { exit = true; }

  }

  cout << "\nGood bye\n";

}
