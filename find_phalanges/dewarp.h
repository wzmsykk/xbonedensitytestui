#if !defined (_DEWARP_H_)
#define _DEWARP_H_
//////////////////////////////////////////////////////////////////////
// 
// dewarp.h
// 
// Based upon:
//   Source Detector Distance  7.283  
//   Source Platform Distance  6.692  
//   Carousel Radius           2.008  
//   Acquisition Pixel Width   0.007504110 
//   Acquisition Pixel Height  0.007507333 
//   Image Pixel Width         0.007504110 
//   Image Pixel Height        0.007507333 
//   Acquisition Max Y         446     
//   Acquisition Max X         634     
// 
// Instructions:
// For each acquisition pixel there is one table entry.  Each entry  
// is composed of a Position and 3 Areas.  The Position points to    
// the first of 4 locations in the 'DeWarped' image that receive the 
// acquisition pixel.  Because the acquisition pixel may overlay up  
// to four adjacent image pixels, each entry has 3 areas (the 4th is 
// implicit).  The 4 areas sum to 256.  Use the following to add the 
// energies from each acquisition pixel to the image pixel.          
// 
//     Y = YPosition
//     X = XPosition
// 
//     Image[X]  [Y]   += AcqValue * Area[0] / 255
//     Image[X+1][Y]   += AcqValue * Area[1] / 255
//     Image[X]  [Y+1] += AcqValue * Area[2] / 255
//     Image[X+1][Y+1] += AcqValue * (255 - sum(Area[0,1,2]) / 255
// 
// NOTE: The table is arrange for easy processing of columns of data
// in the format below.
//     DeWarp[RAW_MAX_X/2][RAW_MAX_Y/2]
// 
// NOTE: The table is only a quarter of the entire acquisition
// area, but since the acquisition area is symetric about the
// source-image-detector it gives a complete solution.
// 
//////////////////////////////////////////////////////////////////////

#define  SDD            7.283       // source-detector distance
#define  SPD            6.692       // source-platform distance
#define  RADIUS         2.008       // carousel radius

#define  ACQ_MAX_Y      512

#define  RAW_MAX_Y      446
#define  RAW_MAX_X      634

#define  IMAGE_MAX_Y    334
#define  IMAGE_MAX_X    536

#define  ACQ_WIDTH      0.0075041
#define  ACQ_HEIGHT     0.0075073

#define  PIXEL_WIDTH    0.0075041
#define  PIXEL_HEIGHT   0.0075073

#define  DEWARP_NAME    "dewarp.dat"


struct _DeWarpHeader
{
   unsigned short usSDD;            // used to verify table format
   unsigned short usSPD;            // used to verify table format
   unsigned short usRADIUS;         // used to verify table format
   unsigned short usSPARE;          // used to verify table format
   unsigned short usAcqYMax;        // to determine table size
   unsigned short usAcqXMax;        // to determine table size
   unsigned short usImageYMax;      // to calculate image size
   unsigned short usImageXMax;      // to calculate image size
};

struct _DeWarpEntry
{
   unsigned short usXPosition;      // 0 to usImageXMax/2
   unsigned char  ucYPosition;      // 0 to usImageYMax/2
   unsigned char  ucArea[3];        // the 4th area is implicit
};


int   DeWarpInit (void);
int   DeWarpFinishedImageX (void);
int   DeWarpAcq (short nAcqX, short* psValue);


#endif  // _DEWARP_H_
