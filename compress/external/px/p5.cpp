/* P5 - File archiver and compressor.  Matt Mahoney

To compress:      P5 archive file file...  (1 or more file names), or
  or (MSDOS):     dir/b | P5 archive       (read file names from input)
  or (UNIX):      ls    | P5 archive
To decompress:    P5 archive
To list contents: more < archive

Compression:  The files listed are compressed and stored in the archive,
which is created.  The archive must not already exist.  File names may
specify a path, which is stored.  If there are no file names on the command
line, then P5 prompts for them, reading until the first blank line or
end of file.

Decompression:  No file names are specified.  The archive must exist.
If a path is stored, the file is extracted to the appropriate directory,
which must exist.  P5 does not create directories.  If the file to be
extracted already exists, it is not replaced; rather it is compared with
the archived file, and the offset of the first difference is reported.

It is not possible to add, remove, or update files in an existing archive.

The archived file names are stored in a readable header as follows:

P5\r\n
size name\r\n
size name\r\n
\032\f\0

where "size" is the original number of bytes, as a 9 digit decimal number,
right justified with leading spaces, "name" is the file name (as input,
including path if any), \r = carriage return, \n = linefeed, \f = formfeed,
\032 is the DOS end of file character, and \0 is a NUL.

The header is followed by the compressed data in binary format.  The
input files are concatenated and treated as a single data stream.  Some
improvement in compression is possible by grouping related files together,
for instance, all text files followed by all binary files.

Data is compressed using a bit stream predictive arithmetic encoder
with a neural network predictor.  The predictor estimates the probabilities
P(0) + P(1) = 1 for each bit, given the previous bits.  The arithmetic
encoder begins with a range [0, 1), and divides the range into two
subranges for each input bit, with sizes proportional to P(0) and P(1).
The output is the shortest number that can be expressed as a base 256
fraction (MSB first) within the resulting range.

The decompressor makes the same series of subdivisions using
predictions P(0) and P(1), given the data decompressed so far.
Then it examines the compressed data to select 0 or 1 according to
which range it is in, and outputs that bit.

The predictor is a 2 layer neural network with n inputs, X1...Xn and
1 output.  The inputs are arbitrary functions of the previous data,
intended to be functions which are likely to predict the next data bit, Y.
The output is an estimate of P(Y) = 1/(1 + e^(-SUM(i) Wi * Xi)).
The weights Wi are incrementally updated to minimize the prediction
error, Y - P(Y).

*/

#if sizeof(int)<4
#error "32 bit compiler required"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <new.h>
#include "std.h"

typedef double real;
typedef unsigned long ulong;

// Compute the squashing function: g(x) = 1/(1 + exp(x)) in 16 bit
// arithmetic, where x is scaled by 1K and the result by 64K
class G {
private:
  unsigned short table[11265];  // g(x) = table[x*1K]/64K
public:
  int operator()(int x) const {  // Use fast table lookup
    if (x>=11265)
      return 65535;
    else if (x<=-11265)
      return 0;
    else if (x>=0)
      return table[x];
    else
      return 65535-table[-x];
  }
  G();
} g;  // Global function object g.

// Init table
G::G() {
  for (int i=0; i<=11264; i+=64)  // Compute every 64'th point
    table[i]=min(65535, int(65536.0/(1+exp(-i/1024.0))));
  for (int i=0; i<=11200; i+=64)  // Interpolate
    for (int j=1; j<64; ++j)
      table[i+j]=table[i]+(((table[i+64]-table[i])*j)>>6);
}

// Compute the natural log, ln(x) * 1K, where x is integer.
class Ln {
private:
  short table[16384];  // table[i] = 1K * ln(i)
public:
  Ln();
  int operator()(unsigned int x) const {
    if (x<0x4000)
      return table[x];
    else if (x<0x40000)
      return table[x>>4]+2839;
    else if (x<0x400000)
      return table[x>>8]+5678;
    else if (x<0x4000000)
      return table[x>>12]+8517;
    else if (x<0x40000000)
      return table[x>>16]+11356;
    else
      return table[x>>20]+14196;
  }
} ln;  // Global function

Ln::Ln() {
  table[0]=-32767;
  for (int i=1; i<16384; ++i)
    table[i]=log(i)*1024+0.5;
}


// Read and return a line of input from FILE f (default stdin) up to
// first control character.  Skips CR in CR LF.
string getline(FILE* f=stdin) {
  int c;
  string result="";
  while ((c=getc(f))!=EOF && c>=32)
    result+=char(c);
  if (c=='\r')
    (void) getc(f);
  return result;
}

/* A Predictor predicts the next bit given the bits so far.  Methods:
   p() returns probability of a 1 being the next bit, P(y = 1)
     as a 16 bit number (0 to 64K).
   update(y) updates the prediction model with bit y.
*/

class Predictor {
private:
  struct Wt {       // Array of synapses
    short w;        // Weight *1K
    unsigned char c0, c1;  // Counts of 0 and 1 *2+1
    Wt(): w(0), c0(1), c1(1) {}
  };
  Wt* wt;           // Array[N] of synapses

  // Partition N inputs into N0, N1, ... N[NX-1]
  enum {
    N0=0x800,
    N1=0x10000,
    N2=0x10000,
    N3=0x20000,
//    N4=0x40000,
//    N5=0x40000,
    NX=4,
    N=N0+N1+N2+N3
  };
  int count;        // Number of updates
  ulong s1, s2, s4; // Last n bits are a hash of last n, 2n, 4n bits of input
  int pr;           // Returned by p()
public:
  int p() const {return pr;}  // Probability * 64K
  void update(int y);   // Train neural network on bit y
  Predictor();
  ~Predictor();
};

// Init neural network state
Predictor::Predictor():
  pr(32768), wt(new Wt[N]), count(0), s1(0), s2(0), s4(0) {
}

// Train network to predict bit y next time, save next prediction in pr
void Predictor::update(int y) {

  static int x[NX]; // Indices of active inputs

  // Train network
  const int error=(y<<16)-p();
  const int RS=82;
  const int RL=512;
  const int CMAX=2*125;
  for (int i=0; i<NX; ++i) {
    Wt& w=wt[x[i]];
    if (((w.c1+=y+y)>CMAX) | ((w.c0+=2-y-y)>CMAX)) {
      w.c0=(w.c0>>1)+1;
      w.c1=(w.c1>>1)+1;
    }
    w.w+=(error*(RS+RL*2*(w.c0+w.c1)/(w.c0*w.c1)))>>16;
  }

  // Update current state from input bit y
  s1=(s1<<1)|y;  // Last 32 bits
  switch ((count&1)+y+y) {  // Hash of last 64 bits
    case 1: s2<<=1; break;
    case 2: s2+=3; break;
    case 3: s2<<=1; s2+=7;
  }
  switch ((count&3)+(y<<2)) {  // Hash of last 128 bits
    case 3: s4<<=1; break;
    case 4: s4+=15; break;
    case 5: s4+=31; break;
    case 6: s4+=63; break;
    case 7: s4<<=1; s4+=127;
  } 
  ++count;

  // Select active inputs
  x[0]=((s1&0xff)|(count<<8))&(N0-1);  // Last 8 bits plus position in byte
  x[1]=N0+(s1&(N1-1));  // Last 2 bytes
  x[2]=N0+N1+((s1+(s2&0xfff))&(N2-1));  // Last 3 bytes
  x[3]=N0+N1+N2+((s1+(s2&0xffff))&(N3-1));  // Last 4 bytes
//  x[4]=N0+N1+N2+N3+(s2&(N4-1));  // Last 5 bytes
//  x[5]=N0+N1+N2+N3+N4+(((s4&0xfff)+s2)&(N5-1));   // Last 6 bytes
 
  // Compute next p
  int sum=0;
  for (int i=0; i<NX; ++i) {
    const Wt& w=wt[x[i]];
    sum+=w.w;
  }
  pr=g(sum);
}

// Print stats in destructor
Predictor::~Predictor() {
/*
  // Largest weights
  printf("\nLargest weights\n");
  int wmax=0, wmin=0;
  for (int i=N-1; i>=0; --i) {
    if (wt[i].w>wmax) {
      printf("%d: w=%d c0=%d c1=%d\n",
          i, wt[i].w, wt[i].c0, wt[i].c1);
      wmax=wt[i].w;
    }
    if (wt[i].w<wmin) {
      printf("%d: w=%d c0=%d c1=%d\n",
          i, wt[i].w, wt[i].c0, wt[i].c1);
      wmin=wt[i].w;
    }
  }

  // Histogram of weights
  printf("\nWeights\n");
  static int hist[7][9]; // -32 to +32 by 4 down to +-1/128 by 1/1K
  int zeros=0;
  for (int i=0; i<N; ++i) {
    int w=wt[i].w;
    if (w) {
      for (int j=6; j>=0; --j) {
        if (w>=-4 && w<=4)
          hist[j][w+4]++;
        w/=4;
      }
    }
    else
      ++zeros;
  }
  real scale=32.0;
  for (int i=0; i<7; ++i) {
    printf("%7.4f", scale);
    for (int j=0; j<9; ++j)
      printf(" %6d", hist[i][j]);
    printf("\n");
    scale*=0.25;
  }
  printf("%d zeros\n", zeros);
*/
}


/* An Encoder does arithmetic encoding.  Methods:
   Encoder(COMPRESS, f) creates encoder for compression to archive f, which
     must be open for writing in binary mode
   Encoder(DECOMPRESS, f) creates encoder for decompression from archive f,
     which must be open for reading in binary mode
   encode(bit) in COMPRESS mode compresses bit to file f.
   encode() in DECOMPRESS mode returns the next decompressed bit from file f.
   print() prints compression statistics
*/

typedef enum {COMPRESS, DECOMPRESS} Mode;
class Encoder {
private:
  Predictor predictor;
  Mode mode;             // Compress or decompress?
  FILE* archive;         // Compressed data file
  ulong x1, x2;          // Range, initially [0, 1), scaled by 2^32
  ulong x;               // Last 4 input bytes of archive.
  int eofs;              // Number of EOF's read from data
  long xchars;           // Number of bytes of compressed data
  long encodes;          // Number of bits of uncompressed data
  real entropy;          // Theoretical compresion
  int hist[7][10];       // Error(p) histograms: .0001 .001 .01 .1 all .9 .99
public:
  Encoder(Mode m, FILE* f);
  int encode(int bit=0);
  void print() const;
  ~Encoder();
};

// Constructor
Encoder::Encoder(Mode m, FILE* f): mode(m), archive(f), x1(0),
    x2(0xffffffff), x(0), eofs(0), xchars(0), encodes(0),
    entropy(0), predictor() {

  // In DECOMPRESS mode, initialize x to the first 4 bytes of the archive
  if (mode==DECOMPRESS) {
    for (int i=0; i<4; ++i) {
      int c=getc(archive);
      if (c==EOF) {
        c=0;
        ++eofs;
      }
      else
        ++xchars;
      x=(x<<8)+c;
    }
  }

  // Initialize histograms
  for (int i=0; i<7; ++i)
    for (int j=0; j<10; ++j)
      hist[i][j]=0;
}

/* encode(bit) -- Split the range [x1, x2] at x in proportion to predictor
   P(y = 1).  In COMPRESS mode, make the lower or upper subrange
   the new range according to y.  In DECOMPRESS mode, return 0 or 1
   according to which subrange x is in, and make this the new range.

   Maintain x1 <= x <= x2 as the last 4 bytes of compressed data:
   In COMPRESS mode, write the leading bytes of x2 that match x1.
   In DECOMPRESS mode, shift out these bytes and shift in an equal
   number of bytes into x from the archive.
*/
int Encoder::encode(int y) {
  ++encodes;

  // Split the range
  ulong p=~predictor.p()&0xffff; // Probability P(0) * 64K rounded down
  const ulong xdiff=x2-x1;
  ulong xmid=x1;  // = x1+p*(x2-x1) multiply without overflow, round down
  if (xdiff>=0x10000000) xmid+=(xdiff>>16)*p;
  else if (xdiff>=0x1000000) xmid+=((xdiff>>12)*p)>>4;
  else if (xdiff>=0x100000) xmid+=((xdiff>>8)*p)>>8;
  else if (xdiff>=0x10000) xmid+=((xdiff>>4)*p)>>12;
  else xmid+=(xdiff*p)>>16;
  if (xmid<x1 || xmid>=x2) {
    printf("Range unsplittable: x1=%lX xmid=%lX x2=%lX\n",
      x1, xmid, x2);
    print();
    exit(2);
  }

  // Update the range
  if (mode==COMPRESS) {
    if (y)
      x1=xmid+1;
    else
      x2=xmid;
  }
  else {
    if (x<=xmid) {
      y=0;
      x2=xmid;
    }
    else {
      y=1;
      x1=xmid+1;
    }
  }
  predictor.update(y);

/*
  // Collect stats
  p=65535-p;  // P(1)
  entropy -= (y ? log((p+0.5)/65536.0) : log(1-(p+0.5)/65536.0));
  int ip=int((y ? 1-p/65536.0 : p/65536.0)*100000);
  if (ip<0) ip=0;
  if (ip>99999) ip=99999;
  if (ip<10) hist[0][ip]++;
  if (ip<100) hist[1][ip/10]++;
  if (ip<1000) hist[2][ip/100]++;
  if (ip<10000) hist[3][ip/1000]++;
  hist[4][ip/10000]++;
  if (ip>=90000) hist[5][(ip-90000)/1000]++;
  if (ip>=99000) hist[6][(ip-99000)/100]++;
*/

  // Shift equal MSB's out
  while (((x1^x2)&0xff000000)==0) {
    if (mode==COMPRESS) {
      putc(x2>>24, archive);
      ++xchars;
    }
    x1<<=8;
    x2=(x2<<8)+255;
    if (mode==DECOMPRESS) {
      int c=getc(archive);
      if (c==EOF) {
        c=0;
        if (++eofs>5) {
          printf("Premature end of archive\n");
          print();
          exit(1);
        }
      }
      else
        ++xchars;
      x=(x<<8)+c;
    }
  }
  return y;
}

// Destructor
Encoder::~Encoder() {

  // In COMPRESS mode, write out the remaining bytes of x, x1 < x < x2
  if (mode==COMPRESS) {
    while (((x1^x2)&0xff000000)==0) {
      putc(x2>>24, archive);
      x1<<=8;
      x2=(x2<<8)+255;
      ++xchars;
    }
    putc(x2>>24, archive);  // First unequal byte
    ++xchars;
  }
  print();
}

// Print Encoder stats
void Encoder::print() const {
  if (encodes>0)
    printf("%d/%d = %f bpc (%f theory) in %1.2f sec\n",
      xchars*8, encodes/8,
      xchars*64.0/encodes,
      entropy*8/log(2.0)/encodes,
      clock()/CLOCKS_PER_SEC);
/*
  // Print histogram of p(y)
  static const char* hlabels[7]=
    {".0001", ".001 ", ".01  ", ".1   ", "all  ", ".9   ", ".99  "};
  for (int i=0; i<7; ++i) {
    printf("%s ", hlabels[i]);
    for (int j=0; j<10; ++j)
      printf("%6d ", hist[i][j]);
    printf("\n");
  }
*/
}

// Read one byte from encoder e
int decompress(Encoder& e) {  // Decompress 8 bits, MSB first
  int c=0;
  for (int i=0; i<8; ++i)  
    c=(c<<1)+e.encode();
  return c;
}

// Write one byte c to encoder e
void compress(Encoder& e, int c) {
  for (int i=0; i<8; ++i) {  // Compress 8 bits, MSB first
    e.encode((c&128)?1:0);
    c<<=1;
  }
}

// Fail if out of memory
void handler() {
  printf("Out of memory\n");
  exit(1);
}

// User interface
main(int argc, char** argv) {
  set_new_handler(handler);

  // Check arguments
  if (argc<2) {
    printf("P5 - File compressor/archiver\n"
      "To compress:  P5 archive filename [filename...]\n"
      "  or (MSDOS): dir/b | P5 archive\n"
      "  or (UNIX):  ls    | P5 archive\n"
      "To extract:   P5 archive\n"
      "To view:      more < archive\n");
    return 1;
  }

  // File names and sizes from input or archive
  vector<string> filename; // List of names
  vector<long> filesize;   // Size or -1 if error

  // Extract files
  FILE* archive=fopen(argv[1], "rb");
  if (archive) {

    // Read "P5\r\n" at start of archive
    if (getline(archive) != "P5") {
      printf("Archive file %s not in P5 format\n", argv[1]);
      return 1;
    }

    // Read "size filename" in "%9d %s\r\n" format
    while (true) {
      string s=getline(archive);
      if (s.size()>10) {
        filesize.push_back(atol(s.c_str()));
        filename.push_back(s.substr(10));
      }
      else
        break;
    }

    // Test end of header for "\f\0"
    {
      int c1, c2;
      if ((c1=getc(archive))!='\f' || (c2=getc(archive))!=0) {
        printf("%s: Bad P5 header format %d %d\n", argv[1],
          c1, c2);
        return 1;
      }
    }

    // Extract files from archive data
    Encoder e(DECOMPRESS, archive);
    for (int i=0; i<filename.size(); ++i) {
      printf("%9ld %s: ", filesize[i], filename[i].c_str());

      // Compare with existing file
      FILE* f=fopen(filename[i].c_str(), "rb");
      const long size=filesize[i];
      if (f) {
        bool different=false;
        for (long j=0; j<size; ++j) {
          int c1=decompress(e);
          int c2=getc(f);
          if (!different && c1!=c2) {
            printf("differ at offset %ld, archive=%d file=%d\n",
              j, c1, c2);
            different=true;
          }
        }
        if (!different)
          printf("identical\n");
      }

      // Extract to new file
      else {
        f=fopen(filename[i].c_str(), "wb");
        if (!f)
          printf("cannot create, skipping...\n");
        for (long j=0; j<size; ++j) {
          int c=decompress(e);
          if (f)
            putc(c, f);
        }
        if (f) {
          printf("extracted\n");
          fclose(f);
        }
      }
    }
  }

  // Compress files
  else {

    // Read file names from command line or input
    if (argc>2)
      for (int i=2; i<argc; ++i)
        filename.push_back(argv[i]);
    else {
      printf(
        "Enter names of files to compress, followed by blank line or EOF.\n");
      while (true) {
        string s=getline(stdin);
        if (s=="")
          break;
        else
          filename.push_back(s);
      }
    }

    // Get file sizes
    for (int i=0; i<filename.size(); ++i) {
      FILE* f=fopen(filename[i].c_str(), "rb");
      if (!f) {
        printf("File not found, skipping: %s\n",
          filename[i].c_str());
        filesize.push_back(-1);
      }
      else {
        fseek(f, 0L, SEEK_END);
        filesize.push_back(ftell(f));
        fclose(f);
      }
    }

    // Write header
    archive=fopen(argv[1], "wb");
    if (!archive) {
      printf("Cannot create archive: %s\n", argv[1]);
      return 1;
    }
    fprintf(archive, "P5\r\n");
    for (int i=0; i<filename.size(); ++i) {
      if (filesize[i]>=0)
        fprintf(archive, "%9ld %s\r\n", filesize[i], filename[i].c_str());
    }
    putc(032, archive);
    putc('\f', archive);
    putc(0, archive);

    // Write data
    Encoder e(COMPRESS, archive);
    for (int i=0; i<filename.size(); ++i) {
      const int size=filesize[i];
      if (size>=0) {
        printf("%s: ", filename[i].c_str());
        FILE* f=fopen(filename[i].c_str(), "rb");
        int c;
        for (long j=0; j<size; ++j) {
          if (f)
            c=getc(f);
          else
            c=0;
          compress(e, c);
        }
        if (f) {
          fclose(f);
          e.print();
        }
      }
    }
  }

  return 0;
}
