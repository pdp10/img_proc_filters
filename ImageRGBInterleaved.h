/*
 * Course: Elaborazione delle Immagini
 * University: Padova (IT)
 * Student: Piero Dalle Pezze (486228-sin)
 */

#ifndef IMAGE_RGB_INTERLEAVED_H
#define IMAGE_RGB_INTERLEAVED_H

using namespace std;

/* This class derives defines a rgb interleaved image (es. rgbrgbrgb...triple of rgb).*/
class ImageRGBInterleaved {

 private:
  /* rgb image */
  unsigned char** imageRGB;
  /* width * height pixels */
  int width;
  int height;

 public:
  /* 'ctors */
  ImageRGBInterleaved(unsigned char* = 0, int = 0, int = 0);
  /* get a rgb image of width * height pixels */
  unsigned char** getImage() const;
  /* set an image of width * height pixels */
  void setImage(unsigned char*, int, int);
  /* returns the width of the source and destination image */
  int getWidth() const;
  /* returns the height of the source and destination image */
  int getHeight() const;
  /* destructor */
  ~ImageRGBInterleaved();

};  //end class ImageRGBInterleaved

#endif
