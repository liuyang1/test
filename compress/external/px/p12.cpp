/* P12 - File archiver and compressor.  Matt Mahoney
Like P6, but uses 1 and 2 word contexts, and (like P9) a faster
hash function.

To compress:      P12 archive file file...  (1 or more file names), or
  or (MSDOS):     dir/b | P12 archive       (read file names from input)
  or (UNIX):      ls    | P12 archive
To decompress:    P12 archive
To list contents: more < archive

Compression:  The files listed are compressed and stored in the archive,
which is created.  The archive must not already exist.  File names may
specify a path, which is stored.  If there are no file names on the command
line, then P12 prompts for them, reading until the first blank line or
end of file.

Decompression:  No file names are specified.  The archive must exist.
If a path is stored, the file is extracted to the appropriate directory,
which must exist.  P12 does not create directories.  If the file to be
extracted already exists, it is not replaced; rather it is compared with
the archived file, and the offset of the first difference is reported.

It is not possible to add, remove, or update files in an existing archive.
If you want to do this, extract the files, delete the archive, and
create a new archive with just the files you want.

The archived file names are stored in a readable header as follows:

P12\r\n
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
1 output.  The inputs can be arbitrary functions of the previous data,
intended to be functions which are likely to predict the next data bit, Y.
The output is an estimate of P(Y) = 1/(1 + e^(-SUM(i) Wi * Xi)).
The weights Wi are incrementally updated to minimize the prediction
error, E = Y - P(Y).  The weight update is:

  Wi = Wi + (RS + RL/sigma2) * Xi * E

where RS and RL are short and long term learning rates, and

  sigma2 = (C0+d)*(C1+d)/(C0+C1+2d)

is the variance of the training data in context Xi, where C0 and C1
are the counts of Y=0 and Y=1 in context Xi, and 0 < d <= 1 is a parameter
to avoid division by 0 (which would infer P(Y) = 0 or 1).  In practice
the counts are halved when either exceeds 250 to avoid 8-bit overflow.

The paramter RS is typically 0 to 0.5, higher values for data that
changes statistics frequently (like text to binary).  0 is optimal for
stationary data (uniform statistics).  RL should be between 1/m and 1
for m (= 6) inputs.  Low values are good for correlated inputs, 1 is
optimal for independent inputs.

The m inputs are divided into 6 sets for a total of 2^22 = 4,194,304
inputs.  Only one input from each set is active at any time.  The
active input within each set is selected as a hash function of the
immediate context, 1-4 previous bytes or 1-2 previous words, plus the
0-7 bits of the partial current byte, plus the position (0-7) within
the current byte.  The context sets are:
- The last byte and current 0-7 bits with a leading 1, concatenated
  to a 16 bit number, i.  (Xi = 1, and Xj = 0 for 0 <= j < 64K and j!=i).
- The last 2 bytes plus current bits and position, hashed to a 22 bit number.
- The last 3 bytes plus current bits and position, hashed to a 22 bit number.
- The last 4 bytes plus current bits and position, hashed to a 22 bit number.
- The current word (letters A-Z, a-z) plus current bits, hashed to 22 bits.
- The current and previous words, plus current bits, hashed to 22 bits.

The 5 hashed context indices share a common space of 2^22 - 2^16 inputs
that do not overlap the first set of 2^16 inputs.

P12 differs from P6 in that it uses 6 contexts instead of 5 (last 1-5
bytes).  For the last two contexts, the selected input Xi depends only
on the letters in the words and not on any characters between them.

The parameters were hand tuned to d = 0.5, RS = 0.06, RL = 0.38
on "Alice in Wonderland".
*/

#if sizeof(int)!=4
#error "32 bit compiler required"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <new.h>
#include "std.h"  // Or use <vector> and <string>

typedef double real;
typedef unsigned long ulong;

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

// Compute RL/s^2 = 1024*RL*(c0+c1+1)/((c0+0.5)*(c1+0.5)) by table lookup
// Input is c0:c1 as concatenated 8-bit numbers
class Sigma2 {
private:
  short table[65536];
public:
  int operator()(int c01) {return table[c01&0xffff];}
  Sigma2(double RL);
};

// Initialize table
Sigma2::Sigma2(double RL) {  
  for (int c0=0; c0<256; ++c0)
    for (int c1=0; c1<256; ++c1)
      table[(c0<<8)+c1]=1024*RL*(c0+c1+1)/((c0+0.5)*(c1+0.5));
}

/* A Predictor predicts the next bit given the bits so far.  Methods:
   p() returns probability of a 1 being the next bit, P(y = 1)
     as a 16 bit number (0 to 64K-1).
   update(y) updates the prediction model with bit y (0 or 1)
*/

class Predictor {
public:
  enum {
    NX=6,        // Number of input sets
    N=0x400000   // Number of inputs (2^22)
  };
  struct Wt {       // Array of synapses
    short w;        // Weight *1K
    union {
      struct C {unsigned char c0, c1;} c;  // Counts of 0, 1
      unsigned short c01;  // Concatenated counts for sigma2 function
    };
  };
  int x[NX];        // Indices of active inputs
  Wt* wt;           // Array[N] of synapses
  ulong s0;         // Last 0-7 bits of current byte with leading 1 (1-255)
  ulong s7, s3;     // Last 7 complete bytes of input: 7-4, 3-1
  ulong sw0, sw1;   // Hash of current and previous words
  int pr;           // Returned by p()
  Sigma2 sigma2;    // Computes RL/sigma^2
public:
  int p() const {return pr;}  // Probability * 64K
  void update(int y);   // Train neural network on bit y
  Predictor();
};

// Init neural network state
Predictor::Predictor(): pr(32768), s7(0), s3(0), s0(1), sw0(0), sw1(0),
  sigma2(0.38) // Set RL here
{
  wt=new Wt[N];
  memset(wt, 0, N*sizeof(Wt));  // Faster than a constructor
  for (int i=0; i<NX; ++i)
    x[i]=0;
}

// Train network to predict bit y next time, save next prediction in pr
void Predictor::update(const int y) {

  // Train network
  const int error=(y<<16)-p();
  static const int RS=1024*0.06;
  const int CMAX=250;
  for (int i=0; i<NX; ++i) {
    Wt& w=wt[x[i]+s0];
    if ((w.c.c0+=1-y)>CMAX | (w.c.c1+=y)>CMAX) {
      w.c.c0=w.c.c0>>1;
      w.c.c1=w.c.c1>>1;
    }
    w.w+=(error*(RS+sigma2(w.c01)))>>16;
  }

  // Update current state from input bit y
  s0=(s0<<1)|y;            // Encodes position and 0-7 bits of current byte
  if (s0>=256) {           // End of byte?
    if (isalpha(s0&255))
      sw0=sw0*997+s0;
    else {
      if (sw0)
        sw1=sw0;
      sw0=0;
    }
    s7=(s7<<8)|(s3>>24);   // Bytes 7-4 back
    s3=(s3|(s0&255))<<8;   // Last 3 full bytes, padded with a 0 byte
    s0=1;

    // Select active inputs (hash x[1..4] modulo primes < 2^22 - 2^16 - 2^8)
    x[0]=4128768+(s3&0xffff);       // Last byte
    x[1]=(s3&0xffffff)%4128511;     // Last 2 bytes
    x[2]=s3%4128493;                // Last 3 bytes
    x[3]=(s3+s7*0x3000000)%4128451; // Last 4 bytes
    x[4]=(sw1+sw0*29)%4128401;      // Last 2 words
    x[5]=sw0%4128409;               // Last word
  }

  // Compute next p
  int sum=0;
  for (int i=0; i<NX; ++i) {
    Wt& w=wt[x[i]+s0];
    sum+=w.w;
  }
  pr=g(sum);
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
public:
  Encoder(Mode m, FILE* f);
  int encode(int bit=0);
  void print() const;
  ~Encoder();
};

// Constructor
Encoder::Encoder(Mode m, FILE* f): mode(m), archive(f), x1(0),
    x2(0xffffffff), x(0), eofs(0), xchars(0), encodes(0),
    predictor() {

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
    printf("%d/%d = %f bpc in %1.2f sec\n",
      xchars, encodes/8,
      xchars*64.0/encodes,
      clock()/CLOCKS_PER_SEC);
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
    printf("P12 - File compressor/archiver\n"
      "To compress:  P12 archive filename [filename...]\n"
      "  or (MSDOS): dir/b | P12 archive\n"
      "  or (UNIX):  ls    | P12 archive\n"
      "To extract:   P12 archive\n"
      "To view:      more < archive\n");
    return 1;
  }

  // File names and sizes from input or archive
  vector<string> filename; // List of names
  vector<long> filesize;   // Size or -1 if error

  // Extract files
  FILE* archive=fopen(argv[1], "rb");
  if (archive) {
    printf("Extracting archive %s ...\n", argv[1]);

    // Read "P12\r\n" at start of archive
    if (getline(archive) != "P12") {
      printf("Archive file %s not in P12 format\n", argv[1]);
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
        printf("%s: Bad P12 header format %d %d\n", argv[1],
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
        fclose(f);
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
    fprintf(archive, "P12\r\n");
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
