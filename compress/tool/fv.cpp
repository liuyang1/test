/* fv.cpp - File entropy visualization program

Copyright (C) 2006, 2014, Matt Mahoney.  This program is distributed
without warranty under terms of the GNU general public license v3.
See http://www.gnu.org/licenses/gpl.txt

To compile: g++ fv.cpp

Usage: fv file (Requires 512 MB memory)

The output is fv.bmp with the given size in pixels, which visually
displays where matching substrings of various lengths and offests are
found.  A pixel at x, y is (black, red, green, blue) if the last matching
substring of length (1, 2, 4, 8) at x occured y bytes ago.  x and y
are scaled so that the image dimensions match the file length.
The y axis is scaled log base 10.  The maximum range of a match is 1 GB.
*/

#define NDEBUG  // remove for debugging
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <new>
#include <ctime>
using namespace std;

// hash table size, needs HSIZE*4 bytes of memory (512 MB).
// To reduce memory usage, use a smaller power of 2.  This may cause the program to
// miss long range matches in very large files, but won't affect smaller files.
const int HSIZE=0x8000000;  

typedef unsigned int U32;  // 32 bit unsigned int

// Image - can be drawn to, and saved as a .bmp file 
class Image {
public:
  Image(int w, int h): pixels(w*h*3), width(w), height(h) {}  // size w by h
  void saveBMP(const char *filename);  // Save as .bmp file

  // Add colors (-255 to 255) to pixel at x,y (origin at lower left)
  void put(int x, int y, int red, int green, int blue) {
    assert(x>=0);
    assert(x<width);
    assert(y>=0);
    assert(y<height);
    const int i=(y*width+x)*3;
    int c=pixels[i]+blue;
    pixels[i]=c>255?255:c<0?0:c;
    c=pixels[i+1]+green;
    pixels[i+1]=c>255?255:c<0?0:c;
    c=pixels[i+2]+red;
    pixels[i+2]=c>255?255:c<0?0:c;
  }
private:
  vector<unsigned char> pixels;  // width * height * blue-green-red (3 bytes)
  int width, height;  // Image size in pixels
  void out2(FILE *f, int x) {fprintf(f, "%c%c", x, x>>8);}  // Write 2 bytes
  void out4(FILE *f, unsigned long x)  // Write 4 bytes, LSB first
    {fprintf(f, "%c%c%c%c", int(x), int(x>>8), int(x>>16), int(x>>24));}
};

// Save as a .bmp file
void Image::saveBMP(const char *filename) {
  FILE *f=fopen(filename, "wb");
  if (!f) {
    perror(filename);
    return;
  }
  fprintf(f,"BM");  // magic number for .bmp files
  out4(f, 54+pixels.size());  // file size
  out4(f, 0);  // reserved
  out4(f, 54);  // offset to start of image (no palette)
  out4(f, 40);  // info header size
  out4(f, width);  // image size in pixels
  out4(f, height);
  out2(f, 1);  // image planes
  out2(f, 24);  // output bits per pixel
  out4(f, 0);  // no compression
  out4(f, width*height*3);  // image size in bytes
  out4(f, 3000);  // x pixels per meter
  out4(f, 3000);  // y pixels per meter
  out4(f, 0);  // colors
  out4(f, 0);  // important colors
  for (int i=0; i<int(pixels.size()); ++i)  // write the image
    putc(pixels[i], f);
  fclose(f);
}

// ilog(x) = int(log(x)*c) in range 0 to 255, c is specified in constructor
// (faster than a direct computation)
class Ilog {
  double t[256];  // t[i] = exp(i/c), ilog(t[i]) = i
public:
  Ilog(double c);
  int operator()(double x) const {
    // find i such that t[i-1] < x <= t[i] by binary search
    int i=128;  
    if (x<t[i]) i-=128; i+=64;
    if (x<t[i]) i-=64;  i+=32;
    if (x<t[i]) i-=32;  i+=16;
    if (x<t[i]) i-=16;  i+=8;
    if (x<t[i]) i-=8;   i+=4;
    if (x<t[i]) i-=4;   i+=2;
    if (x<t[i]) i-=2;   i+=1;
    if (x<t[i]) i-=1;
    return i;
  }
};

Ilog::Ilog(double c) {
  assert(c>0);
  for (int i=0; i<256; ++i)
    t[i]=exp(i/c);
}

// argv = fv filename
int main(int argc, char **argv) {
 try {
  time_t start_time=clock();

  // Read file
  if (argc<=1) {
    printf(
      "fv 1.1 - file statistics visualizer\n"
      "(C) 2006, 2014, Matt Mahoney\n"
      "This is free software under GPL v3, www.gnu.org/licenses/gpl.txt\n"
      "\n"
      "To use: fv filename\n"
      "\n"
      "The output is fv.bmp, a 512 x 256 image which plots the distribution of\n"
      "string matches of length 1, 2, 4, and 8 by location in file (x axis) and\n"
      "distance backwards to the previous match (y axis, log scale).  The length\n"
      "of the match is color coded: black=1, red=2, green=4, blue=8.\n"
      "The file must be at least 1 byte and at most 2 GB\n");
    return 1;
  }
  FILE *f=fopen(argv[1], "rb");
  if (!f) {
    perror(argv[1]);
    return 1;
  }
  fprintf(stderr, "Reading %s\n", argv[1]);
  fseek(f, 0, SEEK_END);
  const double size=ftell(f);
  if (size<1) {
    fprintf(stderr, "file is either too big or empty\n");
    return 1;
  }

  // Create blank white image
  const int width=512;
  const int height=256;
  fprintf(stderr, "Drawing fv.bmp %d by %d from %s (%1.0f bytes)\n", 
    width, height, argv[1], size);
  Image g(width, height);
  for (int i=0; i<width; ++i)
    for (int j=0; j<height; ++j)
      g.put(i, j, 255, 255, 255);

  // Draw tick marks on the Y axis (log base 10 scale)
  const int y_label_width=width/50;
  if (height>=2 && width>=2) 
    for (int i=1; i<int(size); i*=10)
      for (int j=1; j<10; ++j)
        if (i*j<int(size))
          for (int k=0; k<y_label_width; ++k)
            g.put(k, int(height*log(i*j)/log(size)), -255/j, -255/j, -255/j);

  // Darken x,y where there are matching strings at x and y (scaled) in s
  const double csd=10.0*width*height/(size+0.5);  // Color scale
  const int cs=int(csd)+1;  // rounded
  const double L2=height/log(2+size);
  Ilog ilog(L2);
  const double xscale=width*0.98/size;
  vector<U32> index(HSIZE);  // hash -> checksum (2 high bits), location (30 bits)
  for (int i=0; i<4; ++i) {
    const int start_pass=clock();
    fseek(f, 0, SEEK_SET);
    U32 h=0;
    double xd=y_label_width-xscale;
    if (i>=2) memset(&index[0], 0, index.size()*sizeof(U32));
    for (U32 j=0; j<size; ++j) {
      int c=getc(f);
      if (i==0) h=c+0x10000;  // 1 byte
      else if (i==1) h=(h*256+c)&0xffff;  // 2 bytes
      else if (i==2) h=h*29*256+c+1;  // hash of last 4 bytes
      else h=h*(16*123456789)+c+1;    // hash of last 8 bytes
      U32& p=index[(h^h>>16)&(HSIZE-1)];
      xd+=xscale;
      const U32 chk=h&0xc0000000;  // 2 bit hash checksum
      if (p>chk && p<chk+j) {
        const int x=int(xd);
        const double r=double(rand())*(1.0/RAND_MAX);
        const int y=ilog(r+(j+chk-p));
        if (cs>1 || rand()<csd*RAND_MAX) {
          switch(i) {
            case 0: g.put(x, y, -cs, -cs, -cs); break;  // black
            case 1: g.put(x, y, 0, -cs, -cs); break;  // red
            case 2: g.put(x, y, -cs, 0, -cs); break;  // green
            case 3: g.put(x, y, -cs, -cs, 0); break;  // blue
          }
        }
      }
      p=j+chk;
    }
    fprintf(stderr, "Drew part %d of 4 in %1.2f seconds\n", i+1,
      double(clock()-start_pass)/CLOCKS_PER_SEC);
  }
  g.saveBMP("fv.bmp");
  fprintf(stderr, "Created fv.bmp in %1.2f seconds\n", 
    double(clock()-start_time)/CLOCKS_PER_SEC);
 }
 catch(bad_alloc x) {
  fprintf(stderr, "Out of memory\n");
 }
 catch(...) {
  fprintf(stderr, "error\n");
 }
 return 0;
}

