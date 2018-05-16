
/*********************** #includes ******************************/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "bmdtypes.h"
/*
typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned int	uint;

typedef struct _tagRESAMPLED
{
	ushort Array[LCD_MAX_X][LCD_MAX_Y];
}RESAMPLED;

typedef struct _tagTRANSPOSE
{
	ushort Array[LCD_MAX_Y][LCD_MAX_X];
}TRANSPOSE;
*/
#include "wrapper.h"


/************************ #defines ******************************/
#define SUCCESS            					0
//#define ERR_IMAGE_TIF_OPENFILE   			1
//#define ERR_IMAGE_TIF_READFILE   			2

#define NUM_PROFILES       		2
#define FINGER_WIDTH					105
#define MAX_NUM_POSSIBLE_FINGERS 10
#define MAX_NUM_ACTUAL_FINGERS   3
#define YD                       IMAGE_MAX_Y*611/1000   
#define YP                       IMAGE_MAX_Y*922/1000 
#define NUM_PEAKS                3
#define NUM_JOINTS               2
#define BONETIP                  0
#define FINGERTIP                1
#define LEFT							0
#define RIGHT							1
#define MAX_NUM_GRAYLEVELS			65536
#define MAX_GRAYLEVEL				65535
#define ROI_GRAYLEVEL				65000
#define X_RATIO						IMAGE_MAX_X*1024/LCD_MAX_X
#define DATA_MIN_Y					(LCD_MAX_Y-(IMAGE_MAX_Y*LCD_MAX_X/IMAGE_MAX_X))/2
#define DATA_MAX_Y					LCD_MAX_Y-DATA_MIN_Y
#define DISPLAY_SCALE_FACTOR		4096
#define INITIAL_LCDROI_VALUE		7
#define NUM_LOG_VALUES				60000
#define WEDGE_RAMP_MIN_MU			2.1 * SCALE_FACTOR_WEDGE  //1 * SCALE_FACTOR_WEDGE
#define WEDGE_RAMP_MAX_MU			3.5 * SCALE_FACTOR_WEDGE  //6 * SCALE_FACTOR_WEDGE
#define WEDGE_STEP_MIN_MU			2.0 * SCALE_FACTOR_WEDGE
#define WEDGE_STEP_MAX_MU			3.5 * SCALE_FACTOR_WEDGE
#define WEDGE_RAMP_DELTA_X			-14
#define WEDGE_RAMP_DELTA_Y			156
#define HIST_SCALE_FACTOR			16
#define STATS_ROI_TOP				67				
#define STATS_ROI_LEFT				164
#define STATS_ROI_BOTTOM			166
#define STATS_ROI_RIGHT				188
#define STATS_ROI_AREA				2500
#define MIN_MEAN						42000  //0
#define MAX_MEAN						57000  //MAX_GRAYLEVEL
#define MIN_SNR						50     //0.0
#define MAX_SNR						1000   //130 changed 7/26/00
#define DEFAULT_PROXIMAL_SKIN		2750
#define DEFAULT_DISTAL_SKIN		0
#define DEFAULT_MIDDLE_SKIN_0		1690
#define DEFAULT_MIDDLE_SKIN_1		0
#define DEFAULT_MIDDLE_SKIN_2		1650
#define MAX_AREA_SUM					3500000000
#define PIXELS_TO_TOP_ROI1			6
#define PIXELS_TO_BOTTOM_ROI1		21
#define PIXELS_TO_TOP_ROI2			32
#define PIXELS_TO_BOTTOM_ROI2		47
#define WEDGE_THRESHOLD				7500
#define CONVERT_UNITS_GCM2_GAIN					0.00573 * SCALE_FACTOR_GCM2		// conversion equation (arbs to g/cm^2)
//#define CONVERT_UNITS_GAIN						0.0108 * SCALE_FACTOR_GCM2 	// equation to convert arbs to g/cm^2
//#define CONVERT_UNITS_OFFSET					0.0129 * SCALE_FACTOR_GCM2  	//	is 0.0108*bmd - 0.0129	
#define WHO_MEAN_BMD_ARBS  						56.89 * SCALE_FACTOR_BMD
#define WHO_STDEV_ARBS								 4.15 * SCALE_FACTOR_BMD
#define WHO_MEAN_BMD_GCM2  						3259797  //WHO_MEAN_BMD_ARBS * CONVERT_UNITS_GAIN_GCM2 * SCALE_FACTOR_GCM2
#define WHO_STDEV_GCM2								 237795  //WHO_STDEV_ARBS * CONVERT_UNITS_GAIN_GCM2 * SCALE_FACTOR_GCM2
//#define WHO_MEAN_BMD_GCM2  						 601512  //WHO_MEAN_BMD_ARBS * CONVERT_UNITS_GAIN - (CONVERT_UNITS_OFFSET * SCALE_FACTOR_BMD)
//#define WHO_STDEV_GCM2							  44820 	//WHO_STDEV_ARBS * CONVERT_UNITS_GAIN
#define WHO_TSCORE_NORMAL_OSTEOPENIA		   -1.0 * SCALE_FACTOR_T_SCORE
#define WHO_TSCORE_OSTEOPENIA_OSTEOPOROSIS 	-2.5 * SCALE_FACTOR_T_SCORE
#define CGS_MEAN_BMD_FEMALE_ARBS					59.18 * SCALE_FACTOR_BMD
#define CGS_STDEV_FEMALE_ARBS						 4.32 * SCALE_FACTOR_BMD
#define CGS_MEAN_BMD_MALE_ARBS					62.69 * SCALE_FACTOR_BMD
#define CGS_STDEV_MALE_ARBS						 5.49 * SCALE_FACTOR_BMD
#define CGS_MEAN_BMD_FEMALE_GCM2					3391014  // CGS_MEAN_BMD_FEMALE_ARBS * SCALE_FACTOR_BMD * SCALE_FACTOR_GCM2
#define CGS_STDEV_FEMALE_GCM2						 247536  // CGS_STDEV_FEMALE_ARBS * SCALE_FACTOR_BMD * SCALE_FACTOR_GCM2
#define CGS_MEAN_BMD_MALE_GCM2					3592137  // CGS_MEAN_BMD_MALE_ARBS * SCALE_FACTOR_BMD * SCALE_FACTOR_GCM2
#define CGS_STDEV_MALE_GCM2						 314577  // CGS_STDEV_MALE_ARBS * SCALE_FACTOR_BMD * SCALE_FACTOR_GCM2
//#define CGS_MEAN_BMD_FEMALE_GCM2				 626244  //(59.18*GAIN-OFFSET) * SCALE_FACTOR_BMD * SCALE_FACTOR_GCM2
//#define CGS_STDEV_FEMALE_GCM2					  46656  // (4.32*GAIN) * SCALE_FACTOR_BMD * SCALE_FACTOR_GCM2
//#define CGS_MEAN_BMD_MALE_GCM2					 664152  //(62.69*GAIN-OFFSET) * SCALE_FACTOR_BMD * SCALE_FACTOR_GCM2
//#define CGS_STDEV_MALE_GCM2						  59292  // (5.49*GAIN) * SCALE_FACTOR_BMD * SCALE_FACTOR_GCM2
#define CGS_TSCORE_NORMAL_OSTEOPENIA			-1.0 * SCALE_FACTOR_T_SCORE
#define CGS_TSCORE_OSTEOPENIA_OSTEOPOROSIS 	-2.0 * SCALE_FACTOR_T_SCORE
//#define GENDER_MAX					2   
//#define AGE_MAX						100 // 0-99
#define MAX_X_NOT_ON_WEDGE       337

/************************ TYPEDEFS ******************************/
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef unsigned short  ushort;
typedef unsigned long   ulong;
typedef unsigned int    uint;


typedef struct _tagIMAGE_T
{
   ushort Array[IMAGE_MAX_Y][IMAGE_MAX_X];
}  IMAGE_T;


typedef struct _tagIMAGE_INFO
{
   ushort      usNumFingers;
   ushort      usMinImage;
   ushort      usMaxImage;
   ushort      usAir;
   ushort      usSkin;
   ushort      usBone;
   ushort      usAirSkinThreshold;
   ushort      usSkinBoneThreshold;
} IMAGE_INFO;


typedef struct _tagMIDLINE_INFO
{
   ushort usError;
   short  sDeltaX;
   short  sDeltaY;
   ushort usWidth[IMAGE_MAX_Y];   /* used to initialize search for bone edges -- is it even necessary?????? */
   double dIntercept;
   BMDPOINT  Fingertip;
   BMDPOINT  BoneTip;
   BMDPOINT  DIP;
   BMDPOINT  DIP_L;
   BMDPOINT  DIP_R;
   BMDPOINT  PIP;
   BMDPOINT  PIP_L;
   BMDPOINT  PIP_R;
   short sMidlineCoords[IMAGE_MAX_Y];
   MIDLINE Midline;

} MIDLINE_INFO;


typedef struct _tagPROFILES
{
   short sYprofile[MAX_NUM_ACTUAL_FINGERS][NUM_PROFILES+1]; //3£¬3 
   short sLeftSkin[MAX_NUM_ACTUAL_FINGERS][NUM_PROFILES+1];
   short sRightSkin[MAX_NUM_ACTUAL_FINGERS][NUM_PROFILES+1];
   short sFingerWidth[MAX_NUM_ACTUAL_FINGERS][NUM_PROFILES];//3£¬2 
   short sLeftBone[MAX_NUM_ACTUAL_FINGERS][NUM_PROFILES+1];
   short sRightBone[MAX_NUM_ACTUAL_FINGERS][NUM_PROFILES+1];
   short sBoneWidth[MAX_NUM_ACTUAL_FINGERS][NUM_PROFILES];
} PROFILES;

typedef struct _tagBONE_EDGES
{
   BMDPOINT Left[IMAGE_MAX_Y];
   BMDPOINT Right[IMAGE_MAX_Y];
   ushort usDistToEdgeL[IMAGE_MAX_Y];
   ushort usDistToEdgeR[IMAGE_MAX_Y];
} BONE_EDGES;

//typedef struct _tagZSCORE_LUT
//{
//  long lMean;
//  long lStDev;
//} ZSCORE_LUT;



/******************** FUNCTION PROTOTYPES ***********************/

void InitializeModuleVariables ();

ushort FindPhalanges (char* pcImFilename, HISTOGRAM* pShHistImage, MIDLINE* pShMidline, BONEINFO* pShBoneInfo, ROI* pShROI, LCD* pShLCD, LCDROI* pShLCDROI);
ushort DefineImageParameters (HISTOGRAM *pHistogram);

ushort SegmentImage (PROFILES* pProfiles);
ushort ThresholdBackground ();
void FilterBackground ();
ushort CountFingers (PROFILES* pProfiles, IMAGE* pImage);
void TransposeImage (IMAGE* pImage, IMAGE_T* pTranspose);
ushort CalcInitialMidlineEquation (ushort usNumFingers, BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo, PROFILES* pProfiles);
void CalcMidlineEquation (ushort usNumFingers, BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo);
void FindTip (ushort usThreshold, IMAGE* pImage);
void FindPIPandDIP (short sTip, IMAGE* pImage);
void FindNewProfiles (PROFILES* pProfiles);
void FindProfileSkinEdges (PROFILES* pProfiles, IMAGE* pImage);
void FindProfileBoneEdges (PROFILES* pProfiles);
void FlatFieldSkin (PROFILES* pProfiles);
ushort FindMean (short sX, short sY, short sSide);
void ThresholdSkin ();
void CreateHistogram (HISTOGRAM* pHistogram, IMAGE* pImage);
void FilterSkinAndBone ();

void FindMidlines (PROFILES* pProfiles);
void FindProfileBoneEdges2 (PROFILES* pProfiles);

void FindJoints ();

void ReadjustMidline ();
void FindRandLjointEdges ();

void FindBoneROIs ();
void FindRandLEdges ();
void InterpolateMissingBoneEdges ();
void DrawROIsOnImage ();

ushort GetImageStats (STATS* pShStats, IMAGE* pShImage);
ushort AutodetectWedge (WEDGE* pAlWedge, NVRAM* pShNvram);
ushort ComputeMuRampWedge ();
ushort ComputeMuStepWedge ();
ushort CalcWedgeMidlineEquation (BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo);
void ComputeBMD (BONEINFO* pShAvgInfo);
void RecomputeAvgBMD ();
void FindTandZScores (DBTYPE* pShDbType, DATABASE* pShDbLUT, RESULT* pShResult, PATIENT* pShPatient);
void FindChineseTandZScores (DBTYPE* pShDbType, DATABASE* pShDbLUT, RESULT* pShResult, PATIENT* pShPatient);
void SetZScoreDatabase(DBTYPE* pShDbType, DATABASE* pShDbLUT, PATIENT* pShPatient);

void CreateDisplayLUT ();
void ResampleForDisplay ();
void ResampleROIPixels ();
void TransposeAndApplyDisplayLUT ();

