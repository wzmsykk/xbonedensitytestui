/*********************** #includes ******************************/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmdtypes.h"

/*********************** ERROR CODES ****************************/
#define SUCCESS 0
#define ERR_OPENFILE   501
#define ERR_READFILE   502
#define ERR_WRITEFILE  503

/************************ typedefs ******************************/
typedef unsigned short  ushort;
typedef unsigned long   ulong;
typedef unsigned int    uint;

typedef struct _tagRESAMPLED
{
   ushort Array[LCD_MAX_X][LCD_MAX_Y];
}RESAMPLED;

typedef struct _tagTRANSPOSE
{
   ushort Array[LCD_MAX_Y][LCD_MAX_X];
}TRANSPOSE;



/******************** FUNCTION PROTOTYPES ***********************/
ushort ReadImage (char* pcFilename, IMAGE* pImage);
ushort WriteImage (char* pcSuffix, char cFilename[81], short sSizeX, short sSizeY, IMAGE* pImage);
ushort WriteDisplayImage (char* pcSuffix, char cFilename[81], short sSizeX, short sSizeY, RESAMPLED* pImage);
ushort WriteDisplayImage2 (char* pcSuffix, char cFilename[81], short sSizeX, short sSizeY, TRANSPOSE* pImage);
ushort WriteLCDImage (char* pcSuffix, char cFilename[81], short sSizeX, short sSizeY, LCD* pImage);
ushort WriteLCDROI (LCDROI* pLcdRoi);
