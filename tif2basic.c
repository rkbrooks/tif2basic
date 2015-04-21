#include <stdio.h>
#define MAXSIZE 100000
int imageWidth = 280;
int imageHeight = 192;

int main(int argc, char *argv[]) {
  
  char *filename = argv[1];
  unsigned char imageData[imageWidth * imageHeight];
  long int filesize;

  
  if (!filename || argc!=2) {
    printf("TIF to Applesoft\nUsage: ./tif2basic image.tif\n\nPlease have your image cropped to 280x192px, indexed color mode (2 colors -- black and white), and saved as a TIF with no compression, pixels ordered per-channel, and a byte order of Macintosh.\n");
    return 1;
  }
  
  // Grab the input file
  FILE *fp;
  fp = fopen(filename, "rb");
  
  // Our first check should be the filesize. If it's a perfect size for
  // our width * height, then we can assume this is already the perfect
  // format, and rip raw byte values.
  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  if (filesize == imageWidth * imageHeight) {
    // Must be the raw pixel data, so let's pull the bytes out into the
    // imageData array.
    fread(imageData, sizeof(imageData), 1, fp);
  } else {
    // This could be a TIF file. Let's check...
    
    unsigned char fileSig[4]; // Used to check if input is a TIF
    fread(fileSig, sizeof(fileSig), 1, fp); // read 10 bytes to our buffer
  
    // Check if this is a TIF image in little-endian format
    unsigned char tifSig[4] = {0x4D, 0x4D, 0x00, 0x2A};
    unsigned int fileSigMatch = 1; // Will spoil if something doesn't match
  
    for (unsigned long i = 0; i < sizeof(tifSig); i++) {
      if (tifSig[i] != fileSig[i]) {
        fileSigMatch = 0;
      }
    }
  
    if (!fileSigMatch) {
      printf("Please only use TIF files with this program.\n");
      return 1;
    }
  
    // If this runs, then the above checks out. Let's read in the file
    // data.
    unsigned char imageBuffer[MAXSIZE]; // Enough to hold everything
    fread(imageBuffer, sizeof(imageBuffer), 1, fp);
  
    // Now let's search for the 'end of data' string -- I've noticed it
    // is always 00 03 A0 when exporting from Photoshop CS4, so I'm just
    // going to assume that.
    unsigned int iEndOfImageData;
    unsigned char testbyte, nextbyte;
    for (unsigned int byteNum = 0; byteNum < (sizeof(imageBuffer) - 1); byteNum++) {
      testbyte = imageBuffer[byteNum];
      nextbyte = imageBuffer[byteNum + 1];
    
      if (testbyte == 0x03 && nextbyte == 0xA0) {
        // Found it
        iEndOfImageData = byteNum - 2; // One for 0x00, one more for data
        break;
      }
    }
  
    // Time to get the image data. We know the index of the last byte of
    // image data, so let's start there and work backwards.
    for (unsigned int byteNum = 0; byteNum < sizeof(imageData); byteNum++) {
      imageData[sizeof(imageData) - 1 - byteNum] = imageBuffer[iEndOfImageData - byteNum];
    }
  }
  
  // Now that we have the image data, let's make the BASIC program!
  int basicLineNumber = 100;
  
  // Print the image interpreter routine
  // This is originally written by Brian Broker, found on
  // http://www.calormen.com/jsbasic/.
  printf("10\tHGR2 : HCOLOR= 1\n20\tFOR Y = 0 TO 191\n30\t  FOR X = 0 TO 279 STEP 16\n40\t    READ N\n50\t    FOR I = 0 TO 15\n60\t     N = N/2 : IF N <> INT(N) THEN HPLOT X+I, Y : N = INT(N)\n70\t    NEXT\n80\t  NEXT\n90\tNEXT\n");
  
  for (int lineNum = 0; lineNum < 192; lineNum++) {
    int linePixelStart = lineNum * imageWidth;
    int blockNum;
    int block[18];
    
    // We should set up a pixel threshold, to distinguish between Apple
    // II white and black pixels. Here, I'm setting the threshold to
    // 0x00 so anything that isn't 'black' is considered 'white'.
    unsigned char pixelThreshold = 0x01;
    unsigned char pixelValue;
    
    // Get the first 17 blocks (of 16 pixels)
    for (blockNum = 0; blockNum < 17; blockNum++) {
      float blockAsNumber = 0;
      
      for (int pixelNum = 15; pixelNum >= 0; pixelNum--) {
        int imagePixelNum = linePixelStart + (16 * blockNum) + pixelNum;
        
        pixelValue = imageData[imagePixelNum];
        
        if (pixelThreshold <= pixelValue) {
          blockAsNumber += 0.5;
        }
        
        blockAsNumber *= 2;
      }
      
      block[blockNum] = (int)blockAsNumber;
    }
    
    // Get the 18th block (of 8 pixels)
    float blockAsNumber = 0;
    for (int pixelNum = 7; pixelNum >= 0; pixelNum--) {
      int imagePixelNum = linePixelStart + (16 * blockNum) + pixelNum;
      
      pixelValue = imageData[imagePixelNum];
      
      if (pixelThreshold <= pixelValue) {
        blockAsNumber += 0.5;
      }
      
      blockAsNumber *= 2;
    }
    block[blockNum] = (int)blockAsNumber;    
    
    // Now that we have the block, write it to screen
    printf("%d\tDATA ", basicLineNumber);
    
    for (int i = 0; i < 18; i++) {
      if (17 != i) {
        printf("%d,", block[i]);
      } else {
        printf("%d\n", block[i]);
      }
    }
    
    basicLineNumber += 10;
  }
  
  return 0;
}
