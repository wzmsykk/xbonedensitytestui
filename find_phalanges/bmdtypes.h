//////////////////////////////////////////////////////////////////////
// 
// bmdtypes.h
// 
// Header "bmdtypes" for bmd Application
// 
//////////////////////////////////////////////////////////////////////

#if !defined (_BMDTYPES_H_)
#define _BMDTYPES_H_

#include "stateerr.h"
//#include "msg.h"
#include "dewarp.h"

//////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////
typedef  unsigned short ushort;
typedef  unsigned long  ulong;
typedef  unsigned char  uchar;
typedef  unsigned char  byte;


//////////////////////////////////////////////////////////////////////
// Miscellaneous Defines
//////////////////////////////////////////////////////////////////////
#define  NO       0
#define  YES      1

#define  FALSE    0
#define  TRUE     1

//#define  AMPM_LEN 3

//#define  ASIAN  1
//#undef   ASIAN


//////////////////////////////////////////////////////////////////////
//
// Message structure for QNX
//
//////////////////////////////////////////////////////////////////////
#define MSG_LEN   160
typedef struct _tagBMDMSG
{
   ushort   usMsg;
   union
   {
      ulong    ulData;
      struct            // NOTE:  This structure is highly dependent
      {                 // on byte ordering (ie. Motorola vs. Intel)
         ushort   usError;   // Low 2 bytes of ulData.
         ushort   usStatus;   // High 2 bytes of ulData
      };
   };
} BMDMSG;

typedef struct _tagLONGMSG
{
   ushort   usMsg;
	ulong		ulData;
	char		cBuf[MSG_LEN];
} LONGMSG;

typedef struct _tagGPBuf
{
	ushort	bUsed;
	ushort	usLen;
	char		cBuf[MSG_LEN];
}GPBUF;


////////////////////////////////////////////////////////////////////////////////
// Parameter Defines
////////////////////////////////////////////////////////////////////////////////
#define  GENDER_FEMALE                    0
#define  GENDER_MALE                      1

#define  ETHNIC_CAUCASIAN                 0 //share with EUR ethnicity list
#define  ETHNIC_AFRICAN_AMERICAN          1
#define  ETHNIC_HISPANIC                  2
#define  ETHNIC_ASIAN                     3
#define  ETHNIC_OTHERS			            4
#define  ETHNIC_MAX								5

#define	ETHNIC_EUR_OTHERS						1
#define	ETHNIC_EUR_MAX							2

#define  ETHNIC_ASIAN_ASIAN	            0
#define  ETHNIC_ASIAN_CAUCASIAN           1
#define  ETHNIC_ASIAN_OTHERS              2
#define  ETHNIC_ASIAN_MAX						3

#define  FORMAT_DATE_MDY                  0
#define  FORMAT_DATE_DMY                  1

#define  FORMAT_TIME_12HOUR               0
#define  FORMAT_TIME_24HOUR               1

#define  FORMAT_REPORT_RESULTSONLY        0
#define  FORMAT_REPORT_INFORMATIVE        1
#define  FORMAT_REPORT_BOTH               2
#define  FORMAT_REPORT_MAX						3

#define  PRINTER_NONE                     0
#define  PRINTER_HP420                    1
#define  PRINTER_HP695                    2
#define	PRINTER_HP840							3		// Same as HP812
#define	PRINTER_MAX								4

#define  SCALE_FACTOR_WEDGE             1000000
#define  SCALE_FACTOR_BMD                 100
#define  SCALE_FACTOR_GCM2						100000 //changed from 10000 on 05/13/04
#define  SCALE_FACTOR_T_SCORE             100
#define  SCALE_FACTOR_Z_SCORE             100
#define	SCALE_FACTOR_DB						100000

#define  T_SCORE_MEAN_BMD                 56.89*SCALE_FACTOR_BMD
#define  T_SCORE_STDEV                    4.15*SCALE_FACTOR_BMD

#define  ROI_MAX_NUM_COORDS               IMAGE_MAX_X/2 + 2*IMAGE_MAX_Y

#define	PRINT_RESOL_LOW						0
#define	PRINT_RESOL_HIGH						1
#define  PRINT_RESOL_MAX						2

#define PRINT_IMAGE_COLOR						0
#define PRINT_IMAGE_BW							1
#define PRINT_IMAGE_MAX							2

#define ANALYSIS_HIGH							0
#define ANALYSIS_NORMAL							1
#define ANALYSIS_OSTEOPENIA					2
#define ANALYSIS_OSTEOPOROSIS					3
#define ANALYSIS_NA								4

#define SERIAL_NUM_MIN_OEM                "500000"

#define LANGUAGE_US_ENGLISH					0 //share with ASIAN language list
#define LANGUAGE_UK_ENGLISH					1
#define LANGUAGE_SPANISH					2
#define LANGUAGE_GERMAN						3
#define LANGUAGE_FRENCH						4
#define LANGUAGE_ITALIAN					5
#define LANGUAGE_PORTUGUESE					6
#define LANGUAGE_DANISH                     7
#define LANGUAGE_DUTCH						8
#define LANGUAGE_FINNISH                    9
#define LANGUAGE_NORWEGIAN                  10
#define LANGUAGE_SWEDISH                    11
#define LANGUAGE_MAX 					    12

//ASIAN language version only
#define LANGUAGE_ASIAN_SIMP_CHINESE       1 
#define LANGUAGE_ASIAN_MAX						2

//////////////////////////////////////////////////////////////////////
// Common Structures
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// BMDDate
//
//
//////////////////////////////////////////////////////////////////////
typedef  struct _tagBMDDate
{
   short year;
   char  month;
   char  day;
} BMDDATE;


//////////////////////////////////////////////////////////////////////
//
// BMDTime
//
//
//////////////////////////////////////////////////////////////////////
typedef  struct _tagBMDTime
{
   char  hour;
   char  min;
   char  sec;
} BMDTIME;


//////////////////////////////////////////////////////////////////////
//
// BMDPoint
//
// The X,Y coordinates on the image.
//
//////////////////////////////////////////////////////////////////////
typedef  struct   _tagBMDPOINT
{
   short X;
   short Y;
} BMDPOINT;




//////////////////////////////////////////////////////////////////////
// Image Transfer Structures
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// ImageStart
//
//
//////////////////////////////////////////////////////////////////////
typedef  struct _tagBMDImageStart
{
   short    sXmax;
   short    sYmax;
   short    sXdpi;
   short    sYdpi;
   BMDDATE  Date;
   BMDTIME  Time;
} IMAGESTART;


//////////////////////////////////////////////////////////////////////
//
// ImageBlock
//
//
//////////////////////////////////////////////////////////////////////
#define BLK_DATA_SIZE		126
typedef  struct _tagBMDImageBlk
{
   short sBlockNumber;
   unsigned short usData[BLK_DATA_SIZE];
} IMAGEBLK;



//////////////////////////////////////////////////////////////////////
//
// ROIBlock
//
//
//////////////////////////////////////////////////////////////////////
typedef  struct _tagBMDROIBlk
{
   short sBlockNumber;
   unsigned short usData[BLK_DATA_SIZE];
} ROIBLK;




//////////////////////////////////////////////////////////////////////
// Shared Memory Structures
//////////////////////////////////////////////////////////////////////
#define BMD_SHMEM          "BMD_SHMEM"

#define CLEAR_EXAM			0 
#define CLEAR_REFERENCE		1 
#define CLEAR_STATUS 		2 
#define CLEAR_PATIENT		3 
#define CLEAR_OPTIONS		4 
#define CLEAR_HISTOGRAM		5 
#define CLEAR_IMAGE			6 
#define CLEAR_RESULT			7 
#define CLEAR_NVRAM			8 
#define CLEAR_PARAM			9 
#define CLEAR_RAWSAMPLE		10
#define CLEAR_RAWDATA		11



//////////////////////////////////////////////////////////////////////
//
// STATUS
//
//
//////////////////////////////////////////////////////////////////////
typedef struct _tagStatus
{
	ulong	 ulSystem;
	ushort usMain;
	ushort usAcquire;
	ushort usImage;
	ushort usDevice;
	ushort usPrint;
	ushort usErase;
} STATUS;


//////////////////////////////////////////////////////////////////////
//
// Patient
//
// Contains the patient information for calculating the results.
//
//////////////////////////////////////////////////////////////////////
#define  NAME_LEN    	64
#define  ID_LEN      	12
#define	OPERATORID_LEN	12
#define  SW_LEN			25
#define  SERIAL_LEN		32
typedef  struct   _tagPATIENT
{
   char  acPatientName[NAME_LEN];// Output will be formatted as: Last, First Middle
   char  acOperatorName[NAME_LEN];
   char  acOperator[OPERATORID_LEN]; 
   char  acID[ID_LEN];           // Patient's health code or other identifier.
   short sAge;                   // Patient's age.
   short sGender;                // Patient's gender (Male or Female).
   short sEthnicity;             // Patient's ethnicity.
} PATIENT;


//////////////////////////////////////////////////////////////////////
// 
// Options
//
//
//////////////////////////////////////////////////////////////////////
#define 	PASSWORD_LEN		 4
typedef struct _tagOPTIONS
{
   ushort   usUsePatientID;
   short    sPatEthnic;
   short    sRptCopy;
   short    sRptFormat;
   short    sDateFormat;
   short    sTimeFormat;
   short    sLanguage;						// RESERVED
	short		sPrinterType;
   ushort   usEraseTime;  					// in milli seconds
	char		szPassword[PASSWORD_LEN];	// NULL == Disable password
	short		sPrintResolution;
	short		sPrintImageColor;
	ushort	usEraseMaxCount;
	ushort	usEraseMaxPercent;			// Resolution 1/10th of a percent
	ushort	usPMTSatMaxCount;
	ushort	usPMTSatMaxPercent;			// Resolution 1/10th of a percent
	ushort	usLowSignalMeanCount;	  	// Resolution 1/10th of a percent
	ushort	usLanguage;
}OPTIONS;


//////////////////////////////////////////////////////////////////////
// 
// System setting
//
//
//////////////////////////////////////////////////////////////////////
typedef struct _tagSystem
{
	char		acSWVersion[SW_LEN];
	char		acDBVersion[SW_LEN];
}SYSTEM;

//////////////////////////////////////////////////////////////////////
// 
// Language setting
//
//
//////////////////////////////////////////////////////////////////////
typedef struct _tagENVVAR
{	
	short		sLanguage;
}ENVVAR;

//////////////////////////////////////////////////////////////////////
//
//  Histogram
//
//
//////////////////////////////////////////////////////////////////////
#define  HISTOGRAM_SIZE    65536
#define HIST_SCALE_FACTOR			16
typedef  struct   _tagHISTOGRAM
{
   long lHistogram[HISTOGRAM_SIZE/HIST_SCALE_FACTOR+1];
	ushort usMinImage;
	ushort usMaxImage;
	ushort usAirSkinThreshold; //空气，皮肤，骨组织临界值 
	ushort usSkinBoneThreshold;
} HISTOGRAM;


//////////////////////////////////////////////////////////////////////
//
// Image
//
// A DeWarped image and flat field corrected image.
//
// NOTE:  The dimensions must match the dimensions in DeWarp.h!
//
// NOTE:  When included as part of the shared memory, faster to
// get a pointer and then tweek the pointer like...
//    IMAGE*  pImage = &SharedMemory.Image;
//    pImage->Array[x][y] = ...
//
//////////////////////////////////////////////////////////////////////
typedef  struct   _tagIMAGE
{
   BMDDATE  Date;
   BMDTIME  Time;
	ushort 	usFinishedX;
   ushort   Array[IMAGE_MAX_X][IMAGE_MAX_Y];
} IMAGE;


//////////////////////////////////////////////////////////////////////
//
// Wedge
//
// The X,Y coordinates of the wedge within the image.
//
//////////////////////////////////////////////////////////////////////
typedef struct _tagWEDGE
{
   long lMu;
   BMDPOINT ptUL;          // upper left
   BMDPOINT ptLL;          // lower left
   BMDPOINT ptLR;          // lower right
   BMDPOINT ptUR;          // upper right
} WEDGE;


//////////////////////////////////////////////////////////////////////
//
// ImStats
//
// Contains statistics of hand image
//
//////////////////////////////////////////////////////////////////////
typedef struct _tagSTATS
{
	long lMean;
	long lSNR;
} STATS;


//////////////////////////////////////////////////////////////////////
//
// MidLine
//
// Two points define the center line of the bone.  Additionally the two
// points define the two ends of the bone or the two ends of the area.
//
//////////////////////////////////////////////////////////////////////
typedef  struct   _tagMIDLINE
{
   BMDPOINT Proximal;            // Center line point closest to the hand.
   BMDPOINT Distal;              // Center line point closest to tip of finger.
                  //ENHANCE - other details for outlining the bone
} MIDLINE;


//////////////////////////////////////////////////////////////////////
//
// ROI
//
// Lists all bone edge points of a finger
//  
//////////////////////////////////////////////////////////////////////
typedef  struct   _tagROI //region of interest 
{
   ushort usNumPoints;
   BMDPOINT Coords[ROI_MAX_NUM_COORDS];
} ROI;


//////////////////////////////////////////////////////////////////////
//
// BoneInfo
//
// Contains the corrected density of the bone.
//
//////////////////////////////////////////////////////////////////////
typedef  struct   _tagBONEINFO
{
   ushort   usActive;            // Indicates whether information has been calculated.
   ushort   usFound;			 // Indicates whether finger in this position has been found.
   long     lArea;
   long     lBoneMineralDensity;  
	long		lBoneMineralDensityGCM2; //骨矿质 
   long     lCorticalDensity;           // 骨皮质 
   long     lCorticalThickness;
   long     lTrabecularDensity;         //骨小梁 
   long     lTrabecularThickness;
} BONEINFO;


//////////////////////////////////////////////////////////////////////
//
// Result
//
//
//////////////////////////////////////////////////////////////////////
typedef  struct   _tagRESULT
{
   ushort   usActive;            // Indicates whether the results have been calculated.
   long     lTScore;             // t-score
   long     lZScore;             // z-score
	long		lTPercent;				// % of Peak BMD 
	long		lZPercent;
   long     lCTRatio;            // cortical / trabecular ratio
	ushort	usWHOAnalysis;				// Analysis based on WHO guidelines (World Health Organization)
	ushort   usCGSAnalysis;			// Analysis based on CGS guidelines (Chinese Gerontological Society)
} RESULT;

//////////////////////////////////////////////////////////////////////
//
// Scan Counts
//
//
//////////////////////////////////////////////////////////////////////
typedef 	struct	_tagSCANCTS
{
	ulong	ulXrayCount;
	ulong ulScanCount;
	ulong ulMfgCount;
} SCANCTS;


//////////////////////////////////////////////////////////////////////
//
// NVRAM storage
//
//
//////////////////////////////////////////////////////////////////////
#define  SERIAL_NUM_LEN     32
// Note that Watcom default to byte boundary
typedef struct _tagNVRAM
{
   uchar		ucByteUsed;		// Total byte used including checksum & byte used
									// Should only update when write default value
	uchar		ucChecksum;		
   char     szSerialNum[SERIAL_NUM_LEN];
   ushort   usPMTValue;
   uchar    ucZOffset;
   uchar    ucTOffset;
	short		sGain;
	short		sOffset;
   ulong		ulAvgMu;
	ulong		ulExpCnt;
	ulong 	ulAccMu;
} NVRAM;

#define	WRITE_COMPLETED	0
#define	WRITE_PMT			1
#define  WRITE_TOFFSET 		2
#define  WRITE_ZOFFSET		3
#define  WRITE_EXP_CNT		4
#define  WRITE_ACC_MU		5
typedef struct _tagSaveNVRam
{
	ushort	usType;
	NVRAM		NVRam;
} SAVENVRAM;

//////////////////////////////////////////////////////////////////////
//
// Param
//
//
//////////////////////////////////////////////////////////////////////
typedef struct _tagParam
{
	ushort	usSamplePerPixel;
	ushort	usDPI;
	ushort	usSamplePerRev;
	ushort	usRawSamplePerRev;
	ushort	usADCCount;
	ushort	usMaxScanLine;
}PARAM;


//////////////////////////////////////////////////////////////////////
//
// RawSample
//
//
//////////////////////////////////////////////////////////////////////
#define	RAWSAMPLE_MAX_Y	ACQ_MAX_Y*4
typedef  struct   _tagRawSample
{
   BMDDATE  Date;
   BMDTIME  Time;
   ushort usData[RAW_MAX_X + 28][RAWSAMPLE_MAX_Y+60];
} RAWSAMPLE;


//////////////////////////////////////////////////////////////////////
//
// RawData
//
//
//////////////////////////////////////////////////////////////////////
typedef  struct   _tagRawData
{
   BMDDATE  Date;
   BMDTIME  Time;
	ushort usFinishedX;
   ushort usData[RAW_MAX_X + 28][ACQ_MAX_Y+25];
} RAWDATA;


//////////////////////////////////////////////////////////////////////
//
// LCD Image
//
//
//////////////////////////////////////////////////////////////////////
#define LCD_MAX_X		320		// QNX require 320 bytes for 2 pixels per byte
#define LCD_MAX_Y		240
typedef  struct   _tagLCD
{
   uchar ucData[LCD_MAX_Y][LCD_MAX_X];
} LCD;


//////////////////////////////////////////////////////////////////////
//
// LCDROI
//
// Lists all bone edge points of a finger in the LCD display
//  
//////////////////////////////////////////////////////////////////////
#define  LCDROI_MAX_NUM_COORDS			  LCD_MAX_X/2 + 2*LCD_MAX_Y //640
typedef  struct   _tagLCDROI
{
   ushort usNumPoints;
   BMDPOINT Coords[LCDROI_MAX_NUM_COORDS];
   ushort usGraylevel[LCDROI_MAX_NUM_COORDS]; //灰度 
} LCDROI;



//////////////////////////////////////////////////////////////////////
//
// DATABASE
//
// Look up table for database
//  
//////////////////////////////////////////////////////////////////////
#define MIN_DB_AGE								0
#define MAX_DB_AGE   							100
#define MIN_VALID_AGE							20
#define MAX_VALID_AGE							85
#define MIN_ASIAN_AGE							10
#define MAX_ASIAN_AGE							89

#define DB_CAUCASIAN_FEMALE	0
#define DB_CHINESE_FEMALE		1
#define DB_CHINESE_MALE			2
#define DB_DEFAULT				3
#define DB_MAX						4

typedef struct _database
{
	long 	lStdDev;
	long	lMean;
	long	lMeanA1;
	long 	lMeanM1;
   long  lMeanBMD;
}DATABASE;

typedef struct _dbtype
{
	ushort usCaucasianFemale;														
	ushort usCaucasianMale;														
	ushort usAfricanAmericanFemale;														
	ushort usAfricanAmericanMale;														
	ushort usHispanicFemale;														
	ushort usHispanicMale;														
	ushort usAsianFemale;														
	ushort usAsianMale;
	ushort usOthersFemale;														
	ushort usOthersMale;
	ushort usChineseFemale;
	ushort usChineseMale;														
	ushort usAsianOthersFemale;
	ushort usAsianOthersMale;													
}DBTYPE;

typedef struct _ServiceFunction
{
	long lFunction;
	long lParam;
}SERVICE_FUNCTION;

// Shared memory
//
//
//////////////////////////////////////////////////////////////////////
// Update by main, image, expose
typedef struct _tagSHMEM
{
	STATUS		Status;			
   PATIENT     Patient;       // WO for UIProc and SerialProc
   OPTIONS     Option;        // WO for UIProc and SerialProc
	ENVVAR		EnvVar;			// WO for UIProc
   HISTOGRAM   Histogram;     // WO for ACQProc
   IMAGE       Image;         // WO for ACQProc
   IMAGE       Reference;     // WO for ACQProc
   WEDGE       Wedge;
   MIDLINE     Midline[3];    // WO for ImageProc
   BONEINFO    BoneInfo[3];   // WO for ImageProc
   BONEINFO    AvgInfo;       // WO for ImageProc
   RESULT      Result;        // WO for ImageProc
//	RESULT		ResultCGS;	
   ROI         Roi[3];
	SCANCTS		ScanCts;  
	NVRAM       NVRam;         // WO for NVRamProc
	PARAM			Param;
	RAWSAMPLE 	RawSample;
	RAWDATA		RawData;			// WO for simpleio
	SYSTEM		System;
	GPBUF			GPBuf;		  // GP Buffer
	LCD			Lcd;
	LCDROI		LcdRoi[3];
	DBTYPE		DbType;
	DATABASE	   DbLUT[DB_MAX][MAX_DB_AGE];
	STATS			Stats;
} SHMEM;






#endif  // _BMDTYPES_H
	 
