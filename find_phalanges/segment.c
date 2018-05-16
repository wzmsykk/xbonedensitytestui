/*********************** #includes ******************************/
#include "segment.h"
#include <math.h>
//#include "loglut.c"
//#include "database.c"


/********************* EXTERNAL VARIABLES/FUNCTIONS************************/
//extern ushort	bDebug;
//extern long     lAdjMu;
//extern float    fDriftMu;
//extern void LogMsg (char* pcBuf);

/********************* GLOBAL VARIABLES **************************/
// Pointers passed in from image.c & shared memory
IMAGE* pImage = NULL;           // FindPhalanges
LCD* pLcd = NULL;			   // TransposeAndApplyLUT
static HISTOGRAM* pHistImage = NULL;   // FindPhalanges
static MIDLINE* pMidline = NULL;       // FindPhalanges
static BONEINFO* pBoneInfo = NULL;     // ComputeBMD, RecomputeAvgBMD
//static BONEINFO* pAvgInfo = NULL;      // ComputeBMD, RecomputeAvgBMD, FindTandZScores
//static WEDGE* pWedge = NULL;           // ComputeMu, ComputeBMD
static ROI* pRoi = NULL;               // FindPhalanges
static LCDROI* pLcdRoi = NULL;		   // ResampleROIPixels
//static NVRAM* pNvram = NULL;		   // ComputeBMD
static char* pcFilename;
static char cFilename[81];
//extern SHMEM* pShMem;

// Global within segment.c but not needed in image.c/ShMem
static IMAGE Copy;
static IMAGE Filtered;  // write out both filtered images to same IMAGE object
static IMAGE Segmented;
//static IMAGE Marked;
static RESAMPLED Resampled;
static HISTOGRAM HistFFS;
static IMAGE_INFO ImageInfo;
static MIDLINE_INFO Finger[MAX_NUM_ACTUAL_FINGERS];
static BONE_EDGES BoneEdges[MAX_NUM_ACTUAL_FINGERS];
static ushort usDisplayLUT[MAX_NUM_GRAYLEVELS];
static BMDPOINT WedgeDistal; //末梢点 
static BMDPOINT WedgeProximal; //内侧点 
static DATABASE *pDbLUT;
extern long lLogLUT[NUM_LOG_VALUES];
//extern const ZSCORE_LUT ZScoreLUT[GENDER_MAX][AGE_MAX][ETHNIC_MAX];

/************************ FUNCTION *******************************
 *
 * NAME:    InitializeModulelVariables
 *
 * AUTHOR:  J M Mitchell 
 *
 * SUMMARY: Initialize variables of image module to zero
 *
 * CALL:    InitializeModuleVariables
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 11/08/99 JMM created
 *
 ****************************************************************/
void InitializeModuleVariables()
{
   memset(&ImageInfo, 0, sizeof(ImageInfo));
   memset(Finger, 0, sizeof(Finger));
   memset(BoneEdges, 0, sizeof(BoneEdges));
   memset(usDisplayLUT, 0, sizeof(usDisplayLUT)); //LUT 灰度映射表 
   memset(&WedgeDistal, 0, sizeof(WedgeDistal));
   memset(&WedgeProximal, 0, sizeof(WedgeProximal));
}  //初始化 


/************************ FUNCTION *******************************
 *
 * NAME:    FindPhalanges
 *
 * AUTHOR:  J M Mitchell 
 *
 * SUMMARY: Segments to trimodal image and finds bone ROIs of 
 *          middle phalanges of 2nd, 3rd, and 4th fingers of hand
 *
 * CALL:    FindPhalanges(char* pcImFilename, HISTOGRAM* pShHistImage, MIDLINE* pShMidline, BONEINFO* pShBoneInfo, ROI* pShROI, LCD* pShLCD, LCDROI* pShLCDROI)
 *
 * RETURNS: ec (error code)
 *
 * HISTORY: 
 * 07/21/99 JMM created
 *
 ****************************************************************/
ushort FindPhalanges(char* pcImFilename, HISTOGRAM* pShHistImage, MIDLINE* pShMidline, BONEINFO* pShBoneInfo, ROI* pShROI, LCD* pShLCD, LCDROI* pShLCDROI)
{
   ushort ec = SUCCESS;
   ushort f;
   short sX;
   short sY;
   PROFILES Profiles;
   
   pcFilename = pcImFilename;
   pHistImage = pShHistImage;
   pMidline = pShMidline;
   pBoneInfo = pShBoneInfo;
   pRoi = pShROI;
   pLcd = pShLCD;
   pLcdRoi = pShLCDROI;

   /*  Allows to read in dewarped data from binary file
   strcpy(cFilename, "/home/root/bmd/tiff/hand");
   //strcpy(cFilename, "/home/root/bmd/image/alqnx");
   if ((ec = ReadImage(cFilename, pImage)) != SUCCESS)
   {
      return ec;
   }
   pcFilename = &cFilename;
   */ 
   for (sX = 0; sX < IMAGE_MAX_X; sX++)
   {
      for (sY = 0; sY < IMAGE_MAX_Y; sY++)
      {
         Copy.Array[sX][sY] = pImage->Array[sX][sY];
      }
   }
   InitializeModuleVariables();
   memset(&Profiles, 0, sizeof(Profiles));
   CreateHistogram(pHistImage, &Copy); 
   //for(int i=0;i<=HISTOGRAM_SIZE/HIST_SCALE_FACTOR;i++) printf("%d   %d  \n",i,pHistImage->lHistogram[i]);

   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
      Profiles.sYprofile[f][0] = YD;
      Profiles.sYprofile[f][1] = YP;
      Profiles.sYprofile[f][2] = 0;
   }

   if (pHistImage != NULL && &Copy != NULL)
   {
      if ((ec = SegmentImage(&Profiles)) != SUCCESS)
	  { 
		printf("%d  ",ec); 
		 return ec; 
	  }
      FindMidlines(&Profiles);
      FindJoints();
      ReadjustMidline();
	  FindBoneROIs();
     //DrawROIsOnImage();
     //ec = WriteImage("_roi", pcFilename, IMAGE_MAX_X, IMAGE_MAX_Y, &Marked);

	  ResampleROIPixels();
	  //ec = WriteDisplayImage("_ds", pcFilename, LCD_MAX_X, LCD_MAX_Y, &Resampled);
	  TransposeAndApplyDisplayLUT(); 
	  //ec = WriteDisplayImage2("_t_lut", pcFilename, LCD_MAX_Y, LCD_MAX_X, &Transposed);
	  //ec = WriteLCDImage("_4bit", pcFilename, LCD_MAX_Y, LCD_MAX_X, pLcd);
	  strcpy(cFilename, "LCD");
	  //ec = WriteLCDImage("_4bit", &cFilename, LCD_MAX_Y, LCD_MAX_X, pLcd);
   }
   return ec;
}


/************************ FUNCTION *******************************
 *
 * NAME:    SegmentImage
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Performs all functions necessary to flat field skin
 *
 * CALL:    SegmentImage( PROFILES* pProfiles)
 *
 * RETURNS: none
 *
 * HISTORY: 
 * 06/04/99 JMM   Created
 *
 ****************************************************************/
ushort SegmentImage(PROFILES* pProfiles)
{
   ushort ec = SUCCESS;
   ushort f;
   //MIDLINE Midline[MAX_NUM_ACTUAL_FINGERS];
   PROFILES SkinProfiles;
   BMDPOINT Proximal[MAX_NUM_ACTUAL_FINGERS];
   BMDPOINT Distal[MAX_NUM_ACTUAL_FINGERS];


   memset(&SkinProfiles, 0, sizeof(SkinProfiles));
   memset(Proximal, 0, sizeof(Proximal));
   memset(Distal, 0, sizeof(Distal));
  
   
   if ((ec = DefineImageParameters(pHistImage)) != SUCCESS)
   {
	  return ec;
   }
   ThresholdBackground();
   //ec = WriteImage("_flatx", pcFilename, IMAGE_MAX_X, IMAGE_MAX_Y, &Copy);
   FilterBackground();
   //ec = WriteImage("_flat", pcFilename, IMAGE_MAX_X, IMAGE_MAX_Y, &Filtered);
   if ((ec = CountFingers(pProfiles, &Filtered)) != SUCCESS)   // use skin edges
   {
	  return ec;
   }

   for (f = 0; f < ImageInfo.usNumFingers; f++)
   {
      Distal[f].X = (pProfiles->sLeftSkin[f][0] + pProfiles->sRightSkin[f][0]) / 2;
      Distal[f].Y = pProfiles->sYprofile[f][0];
      Proximal[f].X = (pProfiles->sLeftSkin[f][1] + pProfiles->sRightSkin[f][1]) / 2;
      Proximal[f].Y = pProfiles->sYprofile[f][1];
   }
   if ((ec = CalcInitialMidlineEquation(ImageInfo.usNumFingers, Proximal, Distal, pMidline, Finger, pProfiles)) != SUCCESS)  //initial estimate
   {
	  return ec;
   }

   FindTip(ImageInfo.usAir, &Filtered);                   //fingertip
   FindPIPandDIP(FINGERTIP, &Copy);   // initial estimate using fingertip as stopping point
   FindNewProfiles(&SkinProfiles);
   FindProfileSkinEdges(&SkinProfiles, &Filtered);    //find along new profiles
   FindProfileBoneEdges(&SkinProfiles);    //uses gradient technique

   FlatFieldSkin(&SkinProfiles);
   //ec = WriteImage("_ffs", pcFilename, IMAGE_MAX_X, IMAGE_MAX_Y, &Filtered);
   CreateHistogram(&HistFFS, &Filtered);
   //for(int i=0;i<=HISTOGRAM_SIZE/HIST_SCALE_FACTOR;i++) printf("%d   %d  \n",i,HistFFS.lHistogram[i]);
   if ((ec = DefineImageParameters (&HistFFS)) != SUCCESS)
   {
	  return ec;
   }

   ThresholdSkin();
   //ec = WriteImage("_bonex", pcFilename, IMAGE_MAX_X, IMAGE_MAX_Y, &Filtered);
   FilterSkinAndBone();
   //ec = WriteImage("_bone", pcFilename, IMAGE_MAX_X, IMAGE_MAX_Y, &Segmented);
   printf("FingerNum:%d \n",ImageInfo.usNumFingers);
   printf("Air:%d  Skin:%d  Bone:%d  \nAirSkinThreshold:%d  SkinBoneThreshold:%d\n",ImageInfo.usAir/16,ImageInfo.usSkin/16,ImageInfo.usBone/16,ImageInfo.usAirSkinThreshold/16,ImageInfo.usSkinBoneThreshold/16);
   return ec;
}


/************************ FUNCTION *******************************
 *
 * NAME:    DefineImageParameters
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Sets image parameters of importance
 *
 * CALL:    DefineImageParameters(HISTOGRAM* pHistogram)
 *
 * RETURNS: void
 *
 * HISTORY: 
 * 06/01/99 JMM   Created
 *
 ****************************************************************/
ushort DefineImageParameters (HISTOGRAM* pHistogram)
{
   long lBin;
   long* plHistogram = NULL;

   plHistogram = pHistogram->lHistogram;

   lBin = 0;
   //while (plHistogram[lBin] == 0 && lBin <= MAX_GRAYLEVEL)
   while (plHistogram[lBin] == 0 && lBin <= MAX_GRAYLEVEL/HIST_SCALE_FACTOR)
   {
      lBin++;
   }

   //if (lBin >= MAX_GRAYLEVEL)
   if (lBin >= MAX_GRAYLEVEL/HIST_SCALE_FACTOR)
   {
      return ERR_IMAGE_EMPTY_HISTOGRAM;
   }
   ImageInfo.usMinImage = (ushort) lBin;


   //lBin = MAX_GRAYLEVEL;
   lBin = MAX_GRAYLEVEL/HIST_SCALE_FACTOR;
   while (plHistogram[lBin] == 0)
   {
      lBin--;
   }
   if (lBin <= 0)
   {
      return ERR_IMAGE_EMPTY_HISTOGRAM;
   }
   ImageInfo.usMaxImage = (ushort) lBin;
   //printf("%d  %d  ",ImageInfo.usMaxImage,ImageInfo.usMinImage);

   return SUCCESS;
}


/************************ FUNCTION *******************************
 *
 * NAME:    ThresholdBackground
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Defines threshold between hand tissue and background (air)
 *          Operates only on original Image
 *
 * CALL:    ThresholdBackground()
 *
 * GLOBALS: Image and HistImage
 *
 * RETURNS: none
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 *
 ****************************************************************/
ushort ThresholdBackground ()
{
   long* plHistogram = NULL;
   ushort usNumBinsToAvg;
   ushort usRange;
   short nb;
   ushort x, y;
   ushort bin;
   long lSum;
   long lHistArea;
   long lImageArea;
   long lAvgValue;
   long lPeakValue;
   ushort usPeakBin;
   long lDiff;
   long lMinDiff;
   long lFWHM;
//   ushort usFWHMbin;
   ushort usGamma;
   ushort usThreeSigma;
   ushort* pRow = NULL;

   plHistogram = pHistImage->lHistogram;
   lHistArea = IMAGE_MAX_X * IMAGE_MAX_Y;
   lImageArea = lHistArea;

   usNumBinsToAvg = 7;
   usRange = usNumBinsToAvg / 2;
   if (ImageInfo.usMinImage < usRange) 
   {
      ImageInfo.usMinImage = usRange;
   }

   bin = ImageInfo.usMaxImage - usRange;
   lPeakValue = 0;
   do
   {  
      // Compute avg value of histogram over neighboring bins to smooth out local maxima
      // Determine the peak histogram value and the bin at which it occurs
      lSum = 0;
      for (nb = -usRange; nb <= usRange; nb++)
      {
         lSum += plHistogram[bin+nb];
      }
      lAvgValue = lSum / usNumBinsToAvg;
      
      if (lAvgValue > lPeakValue)
      {
         lPeakValue = lAvgValue;
         usPeakBin = bin;
      }
      
      lHistArea = lHistArea - plHistogram[bin];
      bin--;
   }while (bin > ImageInfo.usMinImage+usRange);

   // Compute 3sigma value from peak and assign to threshold
   lMinDiff = lImageArea;
   lFWHM = lPeakValue / 2;
   bin = usPeakBin;
   do
   {
      lSum = 0;
      for (nb = -usRange; nb <= usRange; nb++)
      {
         lSum += plHistogram[bin+nb];
      }
      lAvgValue = lSum / usNumBinsToAvg;
      lDiff = abs(lAvgValue - lFWHM);
      if (lDiff < lMinDiff) 
      {
         lMinDiff = lDiff;
//         usFWHMbin = bin;
         usGamma = 2 * (usPeakBin - bin);
      }
      bin--;
   }while (lAvgValue > (long)(0.25 * lPeakValue) && bin > ImageInfo.usMinImage);
   usThreeSigma = (ushort)(3 * usGamma / 2.354);
   ImageInfo.usAirSkinThreshold = (usPeakBin - usThreeSigma) * HIST_SCALE_FACTOR;
   ImageInfo.usAir = MAX_GRAYLEVEL;
   ImageInfo.usSkin = ImageInfo.usAirSkinThreshold;
   //printf("%d  ",ImageInfo.usSkin);
   if (ImageInfo.usAir - ImageInfo.usAirSkinThreshold < 5000)
   {
	  return ERR_IMAGE_THRESHOLD_BACKGROUND;
   }

   // Apply threshold to image
   for (x = 0; x < IMAGE_MAX_X; x++)
   {
      pRow = Copy.Array[x];
      for (y = 0; y < IMAGE_MAX_Y; y++)
      {
         if (*(pRow+y) >= ImageInfo.usAirSkinThreshold)
         {
            *(pRow+y) = ImageInfo.usAir; //Air = Background 
         }
      }
   }
//	pShMem->Histogram.usAirSkinThreshold = ImageInfo.usAirSkinThreshold;

   return SUCCESS;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FilterBackground
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Median filters only the background region of original
 *
 * CALL:    FilterBackground()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 *
 ****************************************************************/
void FilterBackground()
{
   ushort usFilter;
   ushort usCount;
   short sX;
   short sY;
   short sDY;
   ushort* pRow0 = NULL;
   ushort* pRow1 = NULL;
   ushort* pRow2 = NULL;
   long lNumFiltered = 0;


   for (sX = 0; sX < IMAGE_MAX_X; sX++)
   {
      usFilter = 0;
      pRow1 = Copy.Array[sX];
      if (sX != 0 && sX != IMAGE_MAX_X - 1)
      {
         pRow0 = Copy.Array[sX - 1];
         pRow2 = Copy.Array[sX + 1];
         usFilter = 1;
      }
      for (sY = 0; sY < IMAGE_MAX_Y; sY++)
      {
         usCount = 0;
         if (usFilter && sY != 0 && sY != IMAGE_MAX_Y - 1)
         {
            if (pRow1[sY] != (ImageInfo.usAir))
            {
               for (sDY = -1; sDY <= 1; sDY++)
               {
                  if (*(pRow0 + sY + sDY) == ImageInfo.usAir)
                  {
                     usCount++;
                  }
                  if (*(pRow1 + sY + sDY) == ImageInfo.usAir)
                  {
                     usCount++;
                  }
                  if (*(pRow2 + sY + sDY) == ImageInfo.usAir)
                  {
                     usCount++;
                  }
               }
            }
         }
         if (usCount >= 5)
         {
            Filtered.Array[sX][sY] = ImageInfo.usAir;
            lNumFiltered += 1;
         }
         else 
         {
            Filtered.Array[sX][sY] = *(pRow1 + sY);
         }
      }
   }
   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    CountFingers
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Determines number of fingers from background thresholded image
 *
 * CALL:    CountFingers(PROFILES* pProfiles, IMAGE* pImage)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 *
 ****************************************************************/
ushort CountFingers(PROFILES* pProfiles, IMAGE* pImage)
{
   ushort usNumPoints;
   ushort p;
   ushort x;
   ushort dx;
   ushort usRange;
   long lPeakIntensity;
   ushort* pusImage = NULL;
   ushort f, df;
   ushort usDiff;
   ushort usMinDiff;

   ushort usIsFinger[NUM_PROFILES];
   ushort usCount[NUM_PROFILES];
   ushort sLeftEdge[MAX_NUM_POSSIBLE_FINGERS][NUM_PROFILES];
   ushort sRightEdge[MAX_NUM_POSSIBLE_FINGERS][NUM_PROFILES];
   ushort sMidpoint[MAX_NUM_POSSIBLE_FINGERS][NUM_PROFILES];
   ushort usMatch[MAX_NUM_POSSIBLE_FINGERS];

   memset(usCount, 0, sizeof(usCount));
   memset(sLeftEdge, 0, sizeof(sLeftEdge));
   memset(sRightEdge, 0, sizeof(sRightEdge));
   memset(sMidpoint, 0, sizeof(sMidpoint));
   memset(usMatch, 0, sizeof(usMatch));

   count:
   for (p = 0; p < NUM_PROFILES; p++)
   {
      usIsFinger[p] = 0;
      usCount[p] = 0;
      pusImage = &pImage->Array[0][pProfiles->sYprofile[0][p]];   // use only 2 profiles on whole image
      for (x = 0; x < IMAGE_MAX_X; x++)
      {
         // Get average intensity over 3 pixel range
		 lPeakIntensity = 0;
         usRange = 2;
         if (IMAGE_MAX_X <= x + usRange)
         {
            usRange = IMAGE_MAX_X - x - 1;
         }
         usNumPoints = 0;
         for (dx = 0; dx <= usRange; dx++)
         {
			lPeakIntensity += *(pusImage + (IMAGE_MAX_Y*dx));
            usNumPoints += 1;
         }
		 if (lPeakIntensity <= ImageInfo.usSkin * usNumPoints && usIsFinger[p] == 0)
         {
            sRightEdge[usCount[p]][p] = x;
            usIsFinger[p] = 1;
         }
		 else if (lPeakIntensity > ImageInfo.usSkin * usNumPoints && usIsFinger[p] == 1)
         {
            sLeftEdge[usCount[p]][p] = x - 1;
            if (sLeftEdge[usCount[p]][p] - sRightEdge[usCount[p]][p] <= FINGER_WIDTH / 2)
            {
               sRightEdge[usCount[p]][p] = 0;
               sLeftEdge[usCount[p]][p] = 0;
            }
			else if (sLeftEdge[usCount[p]][p] < sRightEdge[usCount[p]][p])
			{
			   return ERR_IMAGE_INIT_SKIN_EDGES;
			}
            else usCount[p] += 1;
            usIsFinger[p] = 0;
         }
	     pusImage += IMAGE_MAX_Y;
      }
   }

   if (usCount[0] < usCount[1]&&(pProfiles->sYprofile[0][1]-pProfiles->sYprofile[0][0])>100)
   {
      pProfiles->sYprofile[0][0]+=20;
      goto count;
   }
   else if (usCount[0]>usCount[1]&&(pProfiles->sYprofile[0][1]-pProfiles->sYprofile[0][0])>100)
   {
      pProfiles->sYprofile[0][1]-=20;
      goto count;
   }

   for (p = 0; p < NUM_PROFILES; p++)
   {
      for (f = 0; f < usCount[p]; f++)
      {
         sMidpoint[f][p] = (sLeftEdge[f][p] + sRightEdge[f][p]) / 2;
      }
   }

   // Correct for unequal counts of fingers along the profiles
   if (usCount[0] < usCount[1])
   {
   	  ImageInfo.usNumFingers = usCount[0];
      for (f = 0; f < usCount[0]; f++)
      {
         usMinDiff = IMAGE_MAX_X;
         for (df = 0; df < usCount[1]; df++)
         {
            usDiff = abs(sMidpoint[f][0] - sMidpoint[df][1]);
            if (usDiff < usMinDiff)
            {
               usMatch[f] = df;
               usMinDiff = usDiff;
            }
         }
         pProfiles->sLeftSkin[f][0] = sLeftEdge[f][0];
         pProfiles->sRightSkin[f][0] = sRightEdge[f][0];
         pProfiles->sLeftSkin[f][1] = sLeftEdge[usMatch[f]][1];
         pProfiles->sRightSkin[f][1] = sRightEdge[usMatch[f]][1];
      }
   }
   else if (usCount[1] < usCount[0])
   {
   	  ImageInfo.usNumFingers = usCount[1];
      for (f = 0; f < usCount[1]; f++)
      {
         usMinDiff = IMAGE_MAX_X;
         for (df = 0; df < usCount[0]; df++)
         {
            usDiff = abs(sMidpoint[f][1] - sMidpoint[df][0]);
            if (usDiff < usMinDiff)
            {
               usMatch[f] = df;
               usMinDiff = usDiff;
            }
         }
         pProfiles->sLeftSkin[f][1] = sLeftEdge[f][1];
         pProfiles->sRightSkin[f][1] = sRightEdge[f][1];
         pProfiles->sLeftSkin[f][0] = sLeftEdge[usMatch[f]][0];
         pProfiles->sRightSkin[f][0] = sRightEdge[usMatch[f]][0];
      }
   }
   else
   {
      ImageInfo.usNumFingers = usCount[1];   	
      for (f = 0; f <= ImageInfo.usNumFingers - 1; f++)
      {
         memcpy(pProfiles->sLeftSkin[f], sLeftEdge[f], NUM_PROFILES * sizeof(short));
         memcpy(pProfiles->sRightSkin[f], sRightEdge[f], NUM_PROFILES * sizeof(short));
      }
   }
   if (ImageInfo.usNumFingers > MAX_NUM_ACTUAL_FINGERS)
   {
      // FIND THE SHORTEST ONE(S) AND REMOVE FROM PROFILES
	  // return ERR_IMAGE_MORE_THAN_3_FINGERS;
   }
   else if (ImageInfo.usNumFingers == 0)
   {
	  return ERR_IMAGE_ZERO_FINGERS; 
   }

   return SUCCESS;
}


/************************ FUNCTION *******************************
 *
 * NAME:    TransposeImage
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Transposes Image
 *
 * CALL:    TransposeImage(IMAGE* pImage, IMAGE_T* pTranspose)
 *
 * RETURNS: IMAGE_T* pTransposedImage
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 *
 ****************************************************************/
void TransposeImage(IMAGE* pImage, IMAGE_T* pTranspose)
{
   ushort y;

   for (y = 0; y < IMAGE_MAX_Y; y++)
   {
      memcpy(pTranspose->Array[y], &(pImage->Array[0][y]), IMAGE_MAX_X * sizeof(ushort));
   }

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    CalcInitialMidlineEquation
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Determines the initial midline of each finger
 *
 * CALL:    CalcInitialMidlineEquation (ushort usNumFingers, BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo, PROFILES* pProfiles)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 09/16/99 JMM   Created
 *
 ****************************************************************/
ushort CalcInitialMidlineEquation (ushort usNumFingers, BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo, PROFILES* pProfiles)
{
   short f;
   ushort fCount;
   short sDeltaX;
   short sDeltaY;
   double dIntercept;
   short sX;
   short fNew[MAX_NUM_ACTUAL_FINGERS];

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   { 
	  if (fCount >= usNumFingers)
	  {
		 continue;
	  }
	  fNew[f] = f;
	  pBoneInfo[f].usFound = TRUE;

	  sDeltaX = (pProximal+f)->X - (pDistal+f)->X;
	  sDeltaY = (pProximal+f)->Y - (pDistal+f)->Y; 
	  if (sDeltaY == 0)
	  {
		 //return ERR_IMAGE_COMPUTE_MIDLINE;
		 pBoneInfo[f].usFound = FALSE;
		 ImageInfo.usNumFingers--;
		 usNumFingers--;
		 continue;
	  }

      if (sDeltaX != 0)
      {
         dIntercept = (pDistal+f)->Y - ((double)sDeltaY / sDeltaX * (pDistal+f)->X); 
	 	 sX = (short) (-dIntercept * sDeltaX / sDeltaY);
      }
      else
      {
         dIntercept = 0;
		 sX = (pDistal+f)->X;
      }
      
	  if (sX < 165)
	  {
		 fNew[f] = 0;
	  }
	  else if (sX > 371)
	  {
		 fNew[f] = 2;
	  }
	  else 
	  {
		 fNew[f] = 1;
	  }
	  fCount++;

	  (pMidlineInfo+fNew[f])->sDeltaX = sDeltaX;
	  (pMidlineInfo+fNew[f])->sDeltaY = sDeltaY;
	  (pMidlineInfo+fNew[f])->dIntercept = dIntercept;
      (pMidline+fNew[f])->Proximal.X = (pProximal+f)->X;
      (pMidline+fNew[f])->Proximal.Y = (pProximal+f)->Y;
      (pMidline+fNew[f])->Distal.X = (pDistal+f)->X;
      (pMidline+fNew[f])->Distal.Y = (pDistal+f)->Y;  
   }


   for (f = MAX_NUM_ACTUAL_FINGERS - 1; f >= 0; f--)
   {
	 if ((pBoneInfo+f)->usFound == TRUE && fNew[f] != f)
	 {
		pProfiles->sLeftSkin[fNew[f]][0] = pProfiles->sLeftSkin[f][0];
		pProfiles->sLeftSkin[fNew[f]][1] = pProfiles->sLeftSkin[f][1];
		pProfiles->sRightSkin[fNew[f]][0] = pProfiles->sRightSkin[f][0];
		pProfiles->sRightSkin[fNew[f]][1] = pProfiles->sRightSkin[f][1];
		(pBoneInfo+fNew[f])->usFound = TRUE;
	    (pBoneInfo+f)->usFound = FALSE;		
	 }	  
   }

   if ((pBoneInfo[1].usFound == TRUE) && (pProfiles->sLeftSkin[1][0] >  MAX_X_NOT_ON_WEDGE)) 
   {
	  return ERR_ACQ_WEDGE_NOT_CORRECT;
   }

   return SUCCESS;
}

/************************ FUNCTION *******************************
 *
 * NAME:    CalcMidlineEquation
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Determines the midline of a specified object in the image
 *
 * CALL:    CalcMidlineEquation (ushort usNumFingers, BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 *
 ****************************************************************/
void CalcMidlineEquation (ushort usNumFingers, BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo)
{
   ushort f;
   ushort fCount;

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	
	  (pMidlineInfo+f)->sDeltaX = (pProximal+f)->X - (pDistal+f)->X;
	  (pMidlineInfo+f)->sDeltaY = (pProximal+f)->Y - (pDistal+f)->Y; 
	  if ((pMidlineInfo+f)->sDeltaY == 0)
	  {
		 //return ERR_IMAGE_COMPUTE_MIDLINE;
		 Finger[f].usError = ERR_IMAGE_COMPUTE_MIDLINE;
		 (pBoneInfo+f)->usFound = FALSE;
		 ImageInfo.usNumFingers--;
		 continue;
	  }

      if ((pMidlineInfo+f)->sDeltaX != 0)
      {
	     (pMidlineInfo+f)->dIntercept = (pDistal+f)->Y - ((double)(pMidlineInfo+f)->sDeltaY / (pMidlineInfo+f)->sDeltaX * (pDistal+f)->X); 
      }
      else
      {
	     (pMidlineInfo+f)->dIntercept  = 0;
      }
      
      (pMidline+f)->Proximal.X = (pProximal+f)->X;
      (pMidline+f)->Proximal.Y = (pProximal+f)->Y;
      (pMidline+f)->Distal.X = (pDistal+f)->X;
      (pMidline+f)->Distal.Y = (pDistal+f)->Y;
	  fCount++;  
   } 
   return;
}

/************************ FUNCTION *******************************
 *
 * NAME:    CalcWedgeMidlineEquation //未用 
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Determines the midline of wedge
 *
 * CALL:    CalcWedgeMidlineEquation (BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 09/17/99 JMM   Created
 *
 ****************************************************************/
ushort CalcWedgeMidlineEquation (BMDPOINT* pProximal, BMDPOINT* pDistal, MIDLINE* pMidline, MIDLINE_INFO* pMidlineInfo)
{
   pMidlineInfo->sDeltaX = pProximal->X - pDistal->X;
   pMidlineInfo->sDeltaY = pProximal->Y - pDistal->Y; 
   if (pMidlineInfo->sDeltaY == 0)
   {
	  return ERR_IMAGE_COMPUTE_MIDLINE;
   }

   if (pMidlineInfo->sDeltaX != 0)
   {
	  pMidlineInfo->dIntercept = pDistal->Y - ((double)pMidlineInfo->sDeltaY / pMidlineInfo->sDeltaX * pDistal->X); 
   }
   else
   {
	  pMidlineInfo->dIntercept  = 0;
   }
      
   pMidline->Proximal.X = pProximal->X;
   pMidline->Proximal.Y = pProximal->Y;
   pMidline->Distal.X = pDistal->X;
   pMidline->Distal.Y = pDistal->Y;
   
   return SUCCESS;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindTip
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Finds specified tip of each finger (bone or fingertip)
 *
 * CALL:    FindTip(ushort usThreshold, IMAGE* pImage)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 *
 ****************************************************************/
void FindTip (ushort usThreshold, IMAGE* pImage)
{
   ushort f;
   ushort fCount;
   ushort usTip;
   short  sX;
   short  sY;
   short  sDY;
   ushort usRange;
   long   lLocal;
   BMDPOINT  Tip;


   memset(&Tip, 0, sizeof(Tip));
   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      usTip = 0;
      sY = IMAGE_MAX_Y - 1;
      do
      {
         // Given y position in finger, compute x coordinate
         if (Finger[f].sDeltaX != 0)
         {
            sX = (short)((sY - Finger[f].dIntercept) * Finger[f].sDeltaX / Finger[f].sDeltaY);
         }
         else
         {
            sX = (pMidline+f)->Proximal.X;
         }

         // Find avg intensity of image in local neighborhood 仅在DY=4上取平均
         usRange = 3;
         if (sY < usRange)
         {
            usRange = sY;
         }

         if (sX >= 0 && sX < IMAGE_MAX_X)
         {
            lLocal = 0;
            for (sDY = 0; sDY <= usRange; sDY++)
            {
               lLocal += pImage->Array[sX][sY-sDY];
            }
            lLocal /= sDY;
         
            if (usTip == 0 && ((lLocal == usThreshold) || sY == 0)) 
            {
               Tip.X = sX;
               Tip.Y = sY;
               usTip = 1;
            }
         }
         sY--;
      }while (usTip == 0 && sY >= 0);

      if (usThreshold == ImageInfo.usSkin)
      {
         Finger[f].BoneTip.X = Tip.X;
         Finger[f].BoneTip.Y = Tip.Y;
      }
      else if (usThreshold == ImageInfo.usAir)
      {
         Finger[f].Fingertip.X = Tip.X;
         Finger[f].Fingertip.Y = Tip.Y;
      }
      fCount++;
   }

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindPIPandDIP
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Finds PIP and DIP in image //PIP(proximal interphalangeal point) DIP(distal interphalangeal point)
 *
 * CALL:    FindPIPandDIP(short sTip, IMAGE* pImage)
 *
 * RETURNS: void
 *
 * HISTORY: 
 * 06/03/99 JMM   Created
 *
 ****************************************************************/
void FindPIPandDIP (short sTip, IMAGE* pImage)
{
   ushort f;
   ushort fCount;
   BMDPOINT Tip;
   short sX;
   short sY;
   short sStripWidth;
   short sDX;
   short sYPerpendicular;
   ushort* pRow0 = NULL;
   ushort* pRow1 = NULL;
   ushort* pRow2 = NULL;
   ushort i0, i1, i2, i3, i4, i5, i6, i7, i8;
   ushort usNumPixels;
   long lDelx;
   long lDely;
   long lDel;
   long lGradientProfile[IMAGE_MAX_Y];
   ushort usRange;
   ushort pk;
   ushort usGradientPeaks[NUM_PEAKS];
   ushort usPeakRank[NUM_PEAKS];
   ushort pk2;
   ushort tempPosition;
   ushort tempRank;
   long lMax;
   short sYPeak;
   short i;

   memset(&Tip, 0, sizeof(Tip));
   memset(lGradientProfile, 0, sizeof(lGradientProfile));
   memset(usGradientPeaks, 0, sizeof(usGradientPeaks));
   memset(usPeakRank, 0, sizeof(usPeakRank));

   sStripWidth = 20;
   usRange = FINGER_WIDTH / 4;
   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      if (sTip) 
      {
         Tip.X = Finger[f].Fingertip.X;
         Tip.Y = Finger[f].Fingertip.Y;
      }
      else if (!sTip)
      {
         Tip.X = Finger[f].BoneTip.X;
         Tip.Y = Finger[f].BoneTip.Y;
      }

      for (sY = IMAGE_MAX_Y - 10; sY >= Tip.Y; sY--)
      {
		 lDelx = 0;
		 lDely = 0;
		 lDel = 0;
         usNumPixels = 0;
         if (Finger[f].sDeltaX != 0)
         {
            sX = (short)((sY - Finger[f].dIntercept) * Finger[f].sDeltaX / Finger[f].sDeltaY);
         }
         else
         {
            sX = Tip.X;
         }
    
         // Generate gradient profile in 40 pixel wide strip about midline
         for (sDX = -sStripWidth; sDX <= sStripWidth; sDX++)
         {
            if ((sX + sDX >= 1) && (sX + sDX <= IMAGE_MAX_X - 2))
            {
               pRow0 = pImage->Array[sX + sDX - 1];
               pRow1 = pImage->Array[sX + sDX];
               pRow2 = pImage->Array[sX + sDX + 1];
			   sYPerpendicular = (sY * 1000 - (Finger[f].sDeltaX * sDX * 1000 / Finger[f].sDeltaY)) / 1000; 

               if (sYPerpendicular >= 1 && sYPerpendicular <= IMAGE_MAX_Y - 2)
               {
				  i0 = *(pRow0+sYPerpendicular-1);
				  i1 = *(pRow0+sYPerpendicular);
				  i2 = *(pRow0+sYPerpendicular+1);
				  i3 = *(pRow1+sYPerpendicular-1);
				  i4 = *(pRow1+sYPerpendicular);
				  i5 = *(pRow1+sYPerpendicular+1);
				  i6 = *(pRow2+sYPerpendicular-1);
				  i7 = *(pRow2+sYPerpendicular);
				  i8 = *(pRow2+sYPerpendicular+1);
                  lDelx = labs((long)(i6 + 2*i7 + i8) - (i0 + 2*i1 + i2));
                  lDely = labs((long)(i2 + 2*i5 + i8) - (i0 + 2*i3 + i6));
                  //lDel += lDelx + lDely;
				  lDel += lDely;
                  usNumPixels += 1;
               }
            }
         }
         if (usNumPixels != 0) 
         {
			lGradientProfile[sY] = lDel * 1000 / usNumPixels;
         }
         else 
         {
			lGradientProfile[sY] = 0;
         }
      }
      
      // Find 3 highest peaks in gradient profile
      for (pk = 0; pk < NUM_PEAKS; pk++)
      {
		 lMax = 0;
         for (sY = IMAGE_MAX_Y - 6; sY >= Tip.Y + 1; sY--)
         {
			if (lGradientProfile[sY] > lMax)
            {
			   lMax = lGradientProfile[sY];
               sYPeak = sY;
            }
         }
         usGradientPeaks[pk] = sYPeak;
         usPeakRank[pk] = pk;
         for (i = -usRange; i <= usRange; i++)
         {
            if (sYPeak + i < 0)
            {
               continue;
            }
            else if (sYPeak + i >= IMAGE_MAX_Y)
            {
               break;
            }
			lGradientProfile[sYPeak+i] = 0;
         }
         for (pk2 = 0; pk2 < pk; pk2++)
         {
            if (usGradientPeaks[pk] > usGradientPeaks[pk2])
            {
               tempPosition = usGradientPeaks[pk];
               usGradientPeaks[pk] = usGradientPeaks[pk2];
               usGradientPeaks[pk2] = tempPosition;
               tempRank = usPeakRank[pk];
               usPeakRank[pk] = usPeakRank[pk2];
               usPeakRank[pk2] = tempRank;
            }
         }
      }


      // Determine which peaks correspond to PIP and DIP
      for (pk = 0; pk < NUM_PEAKS; pk++)
      {
         if (usGradientPeaks[0] - usGradientPeaks[1] < FINGER_WIDTH * 3 / 4)
         {
            if (usPeakRank[0] < usPeakRank[1])
            {
               Finger[f].PIP.Y = usGradientPeaks[0] - 7;
            }
            else 
            {
               Finger[f].PIP.Y = usGradientPeaks[1] - 7;
            }
            Finger[f].DIP.Y = usGradientPeaks[2] + 7;
         }
         else if (usGradientPeaks[1] - usGradientPeaks[2] < FINGER_WIDTH * 3 / 4)
         {
            Finger[f].PIP.Y = usGradientPeaks[0] - 7;
            if (usPeakRank[1] < usPeakRank[2])
            {
               Finger[f].DIP.Y = usGradientPeaks[1] + 7;
            }
            else
            {
               Finger[f].DIP.Y = usGradientPeaks[2] + 7;
            }
         }
         else 
         {
            Finger[f].PIP.Y = usGradientPeaks[0] - 7;
            Finger[f].DIP.Y = usGradientPeaks[1] + 7;
         }
      }

	  if (Finger[f].PIP.Y < Finger[f].DIP.Y)
	  {
		 //return (ec = ERR_IMAGE_FIND_JOINTS);
		 Finger[f].usError = ERR_IMAGE_FIND_JOINTS; 
		 (pBoneInfo+f)->usFound = FALSE;
		 ImageInfo.usNumFingers--;
		 continue;
	  }

      if (Finger[f].sDeltaX != 0)
      {
         Finger[f].PIP.X = (short)((Finger[f].PIP.Y - Finger[f].dIntercept) * Finger[f].sDeltaX / Finger[f].sDeltaY);
         Finger[f].DIP.X = (short)((Finger[f].DIP.Y - Finger[f].dIntercept) * Finger[f].sDeltaX / Finger[f].sDeltaY);
      }
      else
      {
         Finger[f].PIP.X = Tip.X;
         Finger[f].DIP.X = Tip.X;
      }
	  fCount++;
   }
   return;
}



/************************ FUNCTION *******************************
 *
 * NAME:    FindNewProfiles
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Finds new profiles to ensure ROIs for skin flat-fielding
 *          do not pass through PIP and DIP
 *
 * CALL:    FindNewProfiles (PROFILES* pProfiles) 
 *
 * RETURNS: void
 *
 * HISTORY: 
 * 06/04/99 JMM   Created
 *
 ****************************************************************/
void FindNewProfiles(PROFILES* pProfiles)
{
   ushort f;
   ushort fCount;
   short sMinDIP;
   short sMaxDIP;
   short sMinPIP;
   short sMaxPIP;

   // Average the locations of all the PIPS, and then all the DIPs
   sMinDIP = IMAGE_MAX_Y;
   sMaxDIP = 0;
   sMinPIP = IMAGE_MAX_Y;
   sMaxPIP = 0;

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      if (sMaxDIP < Finger[f].DIP.Y)
      {
         sMaxDIP = Finger[f].DIP.Y;
      }
      if (sMinPIP > Finger[f].PIP.Y)
      {
         sMinPIP = Finger[f].PIP.Y;
      }
      if (sMaxPIP < Finger[f].PIP.Y)
      {
         sMaxPIP = Finger[f].PIP.Y;
      }
	  fCount++;
   }

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      pProfiles->sYprofile[f][0] = (Finger[f].Fingertip.Y + Finger[f].DIP.Y - 7) / 2;
      pProfiles->sYprofile[f][1] = (sMaxDIP + sMinPIP) / 2;
      pProfiles->sYprofile[f][2] = (Finger[f].PIP.Y + 7 + IMAGE_MAX_Y) / 2;
	  fCount++;
   }

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindProfileSkinEdges
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Finds skin edges along new profiles to ensure ROIs
 *          for skin flat-fielding do not include any bone
 *
 * CALL:    FindProfileSkinEdges(PROFILES* pProfiles, IMAGE* pImage)
 *
 * RETURNS: void
 *
 * HISTORY: 
 * 07/07/99 JMM   Created
 *
 ****************************************************************/
void FindProfileSkinEdges (PROFILES* pProfiles, IMAGE* pImage)
{
   ushort f;
   ushort fCount;
   ushort p;
   short sMaxWidth;
   short sStopL;
   short sStopR;
   short sX;
   short sXPerpendicular;


   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      for (p = 0; p < NUM_PROFILES + 1; p++)
      {
         sMaxWidth = FINGER_WIDTH * 3 / 4;
         if (Finger[f].sDeltaX != 0)
         {
            sX = (short)((pProfiles->sYprofile[f][p] - Finger[f].dIntercept) * Finger[f].sDeltaX / Finger[f].sDeltaY);
         }
         else
         {
            sX = Finger[f].Fingertip.X;
         }
         sStopL = sX + sMaxWidth;
         if (sStopL > IMAGE_MAX_X - 1)
         {
            sStopL = IMAGE_MAX_X - 1;
         }
         sStopR = sX - sMaxWidth;
         if (sStopR < 0)
         {
            sStopR = 0;
         }
         for (sXPerpendicular = sX; sXPerpendicular <= sStopL; sXPerpendicular++)
         {
            if (pImage->Array[sXPerpendicular][pProfiles->sYprofile[f][p]] == ImageInfo.usAir)
            {
               pProfiles->sLeftSkin[f][p] = sXPerpendicular - 1;
               break;
            }
			else if (sXPerpendicular == IMAGE_MAX_X - 1)
			{
			   pProfiles->sLeftSkin[f][p] = sXPerpendicular;
			}
         }
         for (sXPerpendicular = sX; sXPerpendicular >= sStopR; sXPerpendicular--)
         {
            if (pImage->Array[sXPerpendicular][pProfiles->sYprofile[f][p]] == ImageInfo.usAir)
            {
               pProfiles->sRightSkin[f][p] = sXPerpendicular + 1;
               break;
            }
			else if (sXPerpendicular == 0)
			{
			   pProfiles->sRightSkin[f][p] = sXPerpendicular;
			}
         }
		 if (pProfiles->sRightSkin[f][p] > pProfiles->sLeftSkin[f][p])
		 {
			//return ERR_IMAGE_FIND_PROFILE_SKIN_EDGES;
			Finger[f].usError = ERR_IMAGE_FIND_PROFILE_SKIN_EDGES;
		 	(pBoneInfo+f)->usFound = FALSE;
			ImageInfo.usNumFingers--;
		 	break;
		 }
      }
	  if (Finger[f].usError == SUCCESS)
	  {
		 fCount++;
	  }
   }

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindProfileBoneEdges
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Finds bone edges along new profiles to ensure ROIs
 *          for skin flat-fielding do not include any bone
 *
 * CALL:    FindProfileBoneEdges(PROFILES* pProfiles)
 *
 * RETURNS: void
 *
 * HISTORY: 
 * 06/04/99 JMM   Created
 *
 ****************************************************************/
void FindProfileBoneEdges (PROFILES* pProfiles)
{
   ushort f;
   ushort fCount;
   ushort p;
   short sStopL;
   short sStopR;
   short sX;
   short sY;
   short sWidth;
   ushort usRange;
   ushort* pRow0 = NULL;
   ushort* pRow1 = NULL;
   ushort* pRow2 = NULL;
   ushort i0, i1, i2, i3, i4, i5, i6, i7, i8;
   ushort pk;
   short sBin;
   short sDBin;
   long lDelx;
   long lDely;
   long lDel[IMAGE_MAX_X];
   long lMax; 
   short sMaxBin[2];


   usRange = 3 * FINGER_WIDTH / 16;
   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      for (p = 0; p < NUM_PROFILES+1; p++)
      {
         sStopL = pProfiles->sLeftSkin[f][p] - 10;
         sStopR = pProfiles->sRightSkin[f][p] + 10;
         if (sStopL < sStopR)
         {
         	//return ERR_IMAGE_INIT_PROFILE_BONE_EDGES;
			Finger[f].usError = ERR_IMAGE_INIT_PROFILE_BONE_EDGES;
		 	(pBoneInfo+f)->usFound = FALSE;
			ImageInfo.usNumFingers--;
		 	break;
         }
         else
         {
            sWidth = sStopL - sStopR + 1;
         }

         for (sX = sStopR; sX <= sStopL; sX++)
         {
			lDel[sX - sStopR] = 0;
            pRow0 = Filtered.Array[sX - 1];
            pRow1 = Filtered.Array[sX];
            pRow2 = Filtered.Array[sX + 1];
            for (sY = pProfiles->sYprofile[f][p] - 9; sY <= pProfiles->sYprofile[f][p] + 9; sY++)
            {
               // Copy section of image over, same as with gradient before
               i0 = *(pRow0+sY-1);
               i1 = *(pRow1+sY);
               i2 = *(pRow2+sY+1);
               i3 = *(pRow0+sY-1);
               i4 = *(pRow1+sY);
               i5 = *(pRow2+sY+1);
               i6 = *(pRow0+sY-1);
               i7 = *(pRow1+sY);
               i8 = *(pRow2+sY+1);
               lDelx = labs((long)(i6 + 2*i7 + i8) - (i0 + 2*i1 + i2));
			   lDely = labs((long)(i2 + 2*i5 + i8) - (i0 + 2*i3 + i6));
               lDel[sX - sStopR] += lDelx + lDely;
            }
         }

         for (pk = 0; pk < 2; pk++)
         {
            // Finds peak
			lMax = 0;
            for (sBin = 0; sBin < sWidth; sBin++)
            {
			   if (lMax < lDel[sBin])
               {
				  lMax = lDel[sBin];
                  sMaxBin[pk] = sBin;
               }
            }
            // Wipes out data surrounding peak
            sDBin = -usRange;
            if (sMaxBin[pk] + sDBin < 0)
            {
               sDBin = -sMaxBin[pk];
            }
            do
            {
			  lDel[sMaxBin[pk] + sDBin] = 0;
              sDBin++;
            }while (usRange >= sDBin && sWidth > sMaxBin[pk] + sDBin);
         }
            
         // Assigns to L and R bone edges accordingly
         if (sMaxBin[1] > sMaxBin[0]) 
         {
            pProfiles->sLeftBone[f][p] = sMaxBin[1] + sStopR;
            pProfiles->sRightBone[f][p] = sMaxBin[0] + sStopR;
         }
         else
         {
            pProfiles->sLeftBone[f][p] = sMaxBin[0] + sStopR;
            pProfiles->sRightBone[f][p] = sMaxBin[1] + sStopR;
         }
      }
	  if (Finger[f].usError == SUCCESS)
	  {
		 fCount++;
	  }
   }
   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FlatFieldSkin
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Get skin ROIs between skin and bone edges of each 
 *          finger, compute M/L and P/D skin gradients, then apply
 *          corrections to image
 *
 * CALL:    FlatFieldSkin(PROFILES* pProfiles)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/07/99 JMM   Created
 *
 ****************************************************************/
void FlatFieldSkin(PROFILES* pProfiles)
{
   long lDistalSkin[MAX_NUM_ACTUAL_FINGERS]; 
   long lMiddleSkin[MAX_NUM_ACTUAL_FINGERS];
   long lProximalSkin[MAX_NUM_ACTUAL_FINGERS];
   ushort usDelta;
   //ushort usDeltaNew;
   BMDPOINT Center[MAX_NUM_ACTUAL_FINGERS];
   long lLeftSkinGradient;
   long lRightSkinGradient;
   ushort f1;
   ushort f2;
   long lLeftRightSkinGradient;
   long lDiff;
   long lTotalSkinGradient;
   long lProxMidSkinGradient;
   ushort f;
   ushort fCount;
   long lProxDistSkinGradient;
   ushort *pRow = NULL;
   ushort usX;
   ushort usY;

   usDelta = 10;
   switch(ImageInfo.usNumFingers)
   {
      case 3:
         // FINGER 0
         if (pProfiles->sLeftSkin[0][0] - usDelta > pProfiles->sLeftBone[0][0])
         {
            lDistalSkin[0] = FindMean((short)(pProfiles->sLeftSkin[0][0] - usDelta), pProfiles->sYprofile[0][0], LEFT);
         }
         else if (pProfiles->sRightSkin[0][0] + usDelta < pProfiles->sRightBone[0][0])
         {
            lDistalSkin[0] = FindMean((short)(pProfiles->sRightSkin[0][0] + usDelta), pProfiles->sYprofile[0][0], RIGHT);
         }		 
		 else 
	 	 {
			lDistalSkin[0] = DEFAULT_DISTAL_SKIN;
		 }

         if (pProfiles->sLeftSkin[0][1] - usDelta > pProfiles->sLeftBone[0][1])
         {
            lMiddleSkin[0] = FindMean((short)(pProfiles->sLeftSkin[0][1] - usDelta), pProfiles->sYprofile[0][1], LEFT);
         }
         else if (pProfiles->sRightSkin[0][1] + usDelta < pProfiles->sRightBone[0][1])
         {
            lMiddleSkin[0] = FindMean((short)(pProfiles->sRightSkin[0][1] + usDelta), pProfiles->sYprofile[0][1], RIGHT);
         }
		 else
		 {
			lMiddleSkin[0] = DEFAULT_MIDDLE_SKIN_0;
		 }

         if (pProfiles->sLeftSkin[0][2] - usDelta > pProfiles->sLeftBone[0][2])
         {
            lProximalSkin[0] = FindMean((short)(pProfiles->sLeftSkin[0][2] - usDelta), pProfiles->sYprofile[0][2], LEFT);
         }
         else if (pProfiles->sRightSkin[0][2] + usDelta < pProfiles->sRightBone[0][2])
         {
            lProximalSkin[0] = FindMean((short)(pProfiles->sRightSkin[0][2] + usDelta),pProfiles->sYprofile[0][2], RIGHT);
         }
		 else 
		 {
			lProximalSkin[0] = DEFAULT_PROXIMAL_SKIN;
		 }


         // FINGER 1
         if (pProfiles->sRightSkin[1][0] + usDelta < pProfiles->sRightBone[1][0])
         {
            lDistalSkin[1] = FindMean((short)(pProfiles->sRightSkin[1][0] + usDelta), pProfiles->sYprofile[1][0], RIGHT);
         }
         else if (pProfiles->sLeftSkin[1][0] - usDelta > pProfiles->sLeftBone[1][0])
         {
            lDistalSkin[1] = FindMean((short)(pProfiles->sLeftSkin[1][0] - usDelta), pProfiles->sYprofile[1][0], LEFT);
         }
		 else
		 {
			lDistalSkin[1] = DEFAULT_DISTAL_SKIN;
		 }
         
         if (pProfiles->sRightSkin[1][1] + usDelta < pProfiles->sRightBone[1][1])
         {
            lMiddleSkin[1] = FindMean((short)(pProfiles->sRightSkin[1][1] + usDelta), pProfiles->sYprofile[1][1], RIGHT);
         }
         else if (pProfiles->sLeftSkin[1][1] - usDelta > pProfiles->sLeftBone[1][1])
         {
            lMiddleSkin[1] = FindMean((short)(pProfiles->sLeftSkin[1][1] - usDelta), pProfiles->sYprofile[1][1], LEFT);
         }
		 else
		 {
			lMiddleSkin[1] = DEFAULT_MIDDLE_SKIN_1;
		 }

         if (pProfiles->sRightSkin[1][2] + usDelta < pProfiles->sRightBone[1][2])
         {  
            lProximalSkin[1] = FindMean((short)(pProfiles->sRightSkin[1][2] + usDelta), pProfiles->sYprofile[1][2], RIGHT);
         }
         else if (pProfiles->sLeftSkin[1][2] - usDelta > pProfiles->sLeftBone[1][2])
         {
            lProximalSkin[1] = FindMean((short)(pProfiles->sLeftSkin[1][2] - usDelta), pProfiles->sYprofile[1][2], LEFT);
         }
		 else 
		 {
			lProximalSkin[1] = DEFAULT_PROXIMAL_SKIN;
		 }


         // FINGER 2
         if (pProfiles->sRightSkin[2][0] + usDelta < pProfiles->sRightBone[2][0])
         {
            lDistalSkin[2] = FindMean((short)(pProfiles->sRightSkin[2][0] + usDelta), pProfiles->sYprofile[2][0], RIGHT);
         }
         else if (pProfiles->sLeftSkin[2][0] - usDelta > pProfiles->sLeftBone[2][0])
         {
            lDistalSkin[2] = FindMean((short)(pProfiles->sLeftSkin[2][0] - usDelta), pProfiles->sYprofile[2][0], LEFT);
         }
		 else
		 {
			lDistalSkin[2] = DEFAULT_DISTAL_SKIN;
		 }

         if (pProfiles->sRightSkin[2][1] + usDelta < pProfiles->sRightBone[2][1])
         {
            lMiddleSkin[2] = FindMean((short)(pProfiles->sRightSkin[2][1] + usDelta), pProfiles->sYprofile[2][1], RIGHT);
         }
         else if (pProfiles->sLeftSkin[2][1] - usDelta > pProfiles->sLeftBone[2][1])
         {
            lMiddleSkin[2] = FindMean((short)(pProfiles->sLeftSkin[2][1] - usDelta), pProfiles->sYprofile[2][1], LEFT);
         }
		 else
		 {
			lMiddleSkin[2] = DEFAULT_MIDDLE_SKIN_2;
		 }

         if (pProfiles->sRightSkin[2][2] + usDelta < pProfiles->sRightBone[2][2])
         {
            lProximalSkin[2] = FindMean((short)(pProfiles->sRightSkin[2][2] + usDelta), pProfiles->sYprofile[2][2], RIGHT);
         }
         else if (pProfiles->sLeftSkin[2][2] - usDelta > pProfiles->sLeftBone[2][2])
         {
            lProximalSkin[2] = FindMean((short)(pProfiles->sLeftSkin[2][2] - usDelta), pProfiles->sYprofile[2][2], LEFT);
         }
		 else 
		 {
			lProximalSkin[2] = DEFAULT_PROXIMAL_SKIN;
		 }

   		 fCount = 0;
   		 for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   		 {
			if (fCount >= ImageInfo.usNumFingers)
	  		{
		 	   break;
	  		}
	  		else if ((pBoneInfo+f)->usFound != TRUE)
	  		{
		 	   continue;
	  		}	

            Center[f].Y = pProfiles->sYprofile[f][1];
            Center[f].X = (pProfiles->sRightSkin[f][1] + pProfiles->sLeftSkin[f][1]) / 2;
			fCount++;
         }
         lRightSkinGradient = (long)(((double)lMiddleSkin[0] - lMiddleSkin[1]) * Center[1].X / (Center[1].X - Center[0].X));
         lLeftSkinGradient = (long)(((double)lMiddleSkin[2] - lMiddleSkin[1]) * (IMAGE_MAX_X - Center[1].X) / (Center[2].X - Center[1].X));
         break;
      case 2:
		 if (pBoneInfo[0].usFound == TRUE)
		 {
			f1 = 0;
			if (pBoneInfo[1].usFound == TRUE)
			{
			   f2 = 1;
			}
			else
			{
			   f2 = 2;
			}
		 }
		 else
		 {
			f1 = 1;
			f2 = 2;
		 }

         // FINGER f1
         if (pProfiles->sLeftSkin[f1][0] - usDelta > pProfiles->sLeftBone[f1][0])
         {
            lDistalSkin[f1] = FindMean((short)(pProfiles->sLeftSkin[f1][0] - usDelta), pProfiles->sYprofile[f1][0], LEFT);
         }
         else if (pProfiles->sRightSkin[f1][0] + usDelta < pProfiles->sRightBone[f1][0])
         {
            lDistalSkin[f1] = FindMean((short)(pProfiles->sRightSkin[f1][0] + usDelta), pProfiles->sYprofile[f1][0], RIGHT);
         }
		 else
		 {
			lDistalSkin[f1] = DEFAULT_DISTAL_SKIN;
		 }

         if (pProfiles->sLeftSkin[f1][1] - usDelta > pProfiles->sLeftBone[f1][1])
         {
            lMiddleSkin[f1] = FindMean((short)(pProfiles->sLeftSkin[f1][1] - usDelta), pProfiles->sYprofile[f1][1], LEFT);
         }
         else if (pProfiles->sRightSkin[f1][1] + usDelta < pProfiles->sRightBone[f1][1])
         {
            lMiddleSkin[f1] = FindMean((short)(pProfiles->sRightSkin[f1][1] + usDelta), pProfiles->sYprofile[f1][1], RIGHT);
         }
		 else 
		 {
			lMiddleSkin[f1] = DEFAULT_MIDDLE_SKIN_0;
		 }

         if (pProfiles->sLeftSkin[f1][2] - usDelta > pProfiles->sLeftBone[f1][2])
         {
            lProximalSkin[f1] = FindMean((short)(pProfiles->sLeftSkin[f1][2] - usDelta), pProfiles->sYprofile[f1][2], LEFT);
         }
         else if (pProfiles->sRightSkin[f1][2] + usDelta < pProfiles->sRightBone[f1][2])
         {
            lProximalSkin[f1] = FindMean((short)(pProfiles->sRightSkin[f1][2] + usDelta), pProfiles->sYprofile[f1][2], RIGHT);
         }
		 else
		 {
			lProximalSkin[f1] = DEFAULT_PROXIMAL_SKIN;
		 }

         // FINGER f2
         if (pProfiles->sRightSkin[f2][0] + usDelta < pProfiles->sRightBone[f2][0])
         {
            lDistalSkin[f2] = FindMean((short)(pProfiles->sRightSkin[f2][0] + usDelta), pProfiles->sYprofile[f2][0], RIGHT);
         }
         else if (pProfiles->sLeftSkin[f2][0] - usDelta > pProfiles->sLeftBone[f2][0])
         {
            lDistalSkin[f2] = FindMean((short)(pProfiles->sLeftSkin[f2][0] - usDelta), pProfiles->sYprofile[f2][0], LEFT);
         }
		 else
		 {
			lDistalSkin[f2] = DEFAULT_DISTAL_SKIN;
		 }

         if (pProfiles->sRightSkin[f2][1] + usDelta < pProfiles->sRightBone[f2][1])
         {
            lMiddleSkin[f2] = FindMean((short)(pProfiles->sRightSkin[f2][1] + usDelta), pProfiles->sYprofile[f2][1], RIGHT);
         }
         else if (pProfiles->sLeftSkin[f2][1] - usDelta > pProfiles->sLeftBone[f2][1])
         {
            lMiddleSkin[f2] = FindMean((short)(pProfiles->sLeftSkin[f2][1] - usDelta), pProfiles->sYprofile[f2][1], LEFT);
         }
		 else
		 {
			lMiddleSkin[f2] = DEFAULT_MIDDLE_SKIN_1;
		 }

         if (pProfiles->sRightSkin[f2][2] + usDelta < pProfiles->sRightBone[f2][2])
         {
            lProximalSkin[f2] = FindMean((short)(pProfiles->sRightSkin[f2][2] + usDelta), pProfiles->sYprofile[f2][2], RIGHT);
         }
         else if (pProfiles->sLeftSkin[f2][2] - usDelta > pProfiles->sLeftBone[f2][2])
         {
            lProximalSkin[f2] = FindMean((short)(pProfiles->sLeftSkin[f2][2] - usDelta), pProfiles->sYprofile[f2][2], LEFT);
         }
		 else
		 {
			lProximalSkin[f2] = DEFAULT_PROXIMAL_SKIN;
		 }

         lLeftRightSkinGradient = (lMiddleSkin[f1] - lMiddleSkin[f2]) * IMAGE_MAX_X / (pProfiles->sLeftSkin[f2][1] - pProfiles->sLeftSkin[f1][1]);
         break;
      case 1:
		 if (pBoneInfo[0].usFound == TRUE)
		 {
			f1 = 0;
         	if (pProfiles->sLeftSkin[f1][0] - usDelta > pProfiles->sLeftBone[f1][0])
        	{
			   lDistalSkin[f1] = FindMean((short)(pProfiles->sLeftSkin[f1][0] - usDelta), pProfiles->sYprofile[f1][0], LEFT);
         	}
			else if (pProfiles->sRightSkin[f1][0] + usDelta < pProfiles->sRightBone[f1][0])
			{
			   lDistalSkin[f1] = FindMean((short)(pProfiles->sRightSkin[f1][0] + usDelta), pProfiles->sYprofile[f1][0], RIGHT);
         	}
		 	else
		 	{
			   lDistalSkin[f1] = DEFAULT_DISTAL_SKIN;
		 	}

         	if (pProfiles->sLeftSkin[f1][2] - usDelta > pProfiles->sLeftBone[f1][2])
			{
               lProximalSkin[f1] = FindMean((short)(pProfiles->sLeftSkin[f1][2] - usDelta), pProfiles->sYprofile[f1][2], LEFT);
         	}
         	else if (pProfiles->sRightSkin[f1][2] + usDelta < pProfiles->sRightBone[f1][2])
         	{
               lProximalSkin[f1] = FindMean((short)(pProfiles->sRightSkin[f1][2] + usDelta), pProfiles->sYprofile[f1][2], RIGHT);
         	}
		  	else
		 	{
			   lProximalSkin[f1] = DEFAULT_PROXIMAL_SKIN;
		 	}
			break;
		 }
		 else if (pBoneInfo[1].usFound == TRUE)
		 {
			f1 = 1;
		 }
		 else
		 {
			f1 = 2;
		 }
         if (pProfiles->sRightSkin[f1][0] + usDelta < pProfiles->sRightBone[f1][0])
         {
            lDistalSkin[f1] = FindMean((short)(pProfiles->sRightSkin[f1][0] + usDelta), pProfiles->sYprofile[f1][0], RIGHT);
         }
         else if (pProfiles->sLeftSkin[f1][0] - usDelta > pProfiles->sLeftBone[f1][0])
         {
            lDistalSkin[f1] = FindMean((short)(pProfiles->sLeftSkin[f1][0] - usDelta), pProfiles->sYprofile[f1][0], LEFT);
         }
		 else
		 {
			lDistalSkin[f1] = DEFAULT_DISTAL_SKIN;
		 }

         if (pProfiles->sRightSkin[f1][2] + usDelta < pProfiles->sRightBone[f1][2])
         {
            lProximalSkin[f1] = FindMean((short)(pProfiles->sRightSkin[f1][2] + usDelta), pProfiles->sYprofile[f1][2], RIGHT);
         }
         else if (pProfiles->sLeftSkin[f1][2] - usDelta > pProfiles->sLeftBone[f1][2])
         {
            lProximalSkin[f1] = FindMean((short)(pProfiles->sLeftSkin[f1][2] - usDelta), pProfiles->sYprofile[f1][2], LEFT);
         }
		 else
		 {
			lProximalSkin[f1] = DEFAULT_PROXIMAL_SKIN;
		 }
         break;
      default:
         break;
   }

   lTotalSkinGradient = 0;
   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }      
      lDiff = (lDistalSkin[f] - lProximalSkin[f]);
      lTotalSkinGradient += (long)(abs(lProximalSkin[f] - lDistalSkin[f])) * IMAGE_MAX_Y / (pProfiles->sYprofile[f][2] - pProfiles->sYprofile[f][0]);
	  fCount++;
   }
   if (ImageInfo.usNumFingers > 0)
   {
	  lProxMidSkinGradient = lTotalSkinGradient / ImageInfo.usNumFingers;
	  lProxDistSkinGradient = lProxMidSkinGradient;
   }

   /* Apply skin gradient corrections to image */
   switch (ImageInfo.usNumFingers)
   {
      case (3):
         for (usX = 0; usX < Center[1].X; usX++)
         {
            pRow = Filtered.Array[usX];
            for (usY = 0; usY < IMAGE_MAX_Y; usY++)
            {
               if (*(pRow+usY) != ImageInfo.usAir)
               {
				  *(pRow+usY) -= (ushort) ((lProxDistSkinGradient * (IMAGE_MAX_Y - usY) / IMAGE_MAX_Y) + (lRightSkinGradient * (Center[1].X - usX) / Center[1].X));
               }
            }
         }
         for (usX = Center[1].X; usX < IMAGE_MAX_X; usX++)
         {
            pRow = Filtered.Array[usX];
            for (usY = 0; usY < IMAGE_MAX_Y; usY++)
            {
               if (*(pRow+usY) != ImageInfo.usAir)
               {
				  *(pRow+usY) -= (ushort) ((lProxDistSkinGradient * (IMAGE_MAX_Y - usY) / IMAGE_MAX_Y) + (lLeftSkinGradient * (usX - Center[1].X) / (IMAGE_MAX_X - Center[1].X)));
               }
            }
         }
         break;
      case (2):
         for (usX = 0; usX < IMAGE_MAX_X; usX++)
         {
            pRow = Filtered.Array[usX];
            for (usY = 0; usY < IMAGE_MAX_Y; usY++)
            {
               if (*(pRow+usY) != ImageInfo.usAir)
               {
                  *(pRow+usY) -= (ushort) ((lProxDistSkinGradient * (IMAGE_MAX_Y - usY) / IMAGE_MAX_Y) + (lLeftRightSkinGradient * (IMAGE_MAX_X - usX) / IMAGE_MAX_X));
               }
            }
         }
         break;
      case (1):
         for (usX = 0; usX < IMAGE_MAX_X; usX++)
         {
            pRow = Filtered.Array[usX];
            for (usY = 0; usY < IMAGE_MAX_Y; usY++)
            {
               if (*(pRow+usY) != ImageInfo.usAir)
               {
                  *(pRow+usY) -= (ushort) (lProxDistSkinGradient * (IMAGE_MAX_Y - usY) / IMAGE_MAX_Y);
               }
            }
         }
         break;
      default:
         break;
   }

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindMean
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Find mean intensity around the point (usX, usY) just
 *          inside the skin boundary.  Uses FILTERED image
 *
 * CALL:    FindMean(short sX, short sY, short sSide)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/07/99 JMM   Created
 *
 ****************************************************************/
ushort FindMean (short sX, short sY, short sSide)
{
   short sStart;
   short sEnd;
   short dx;
   short dy;
   long lSum;
   ushort usMean;
   ushort usArea;

   /* check that inside array boundaries */
   lSum = 0;
   usArea = 0;
   if (sSide == LEFT)
   {
      sStart = -5; 
      sEnd = 0;
   }
   else
   {
      sStart = 0;
      sEnd = 5;
   }
   for (dx = sStart; dx <= sEnd; dx++)
   {
      if (sX + dx < 0)
      {
         dx = -sX;
      }  
      else if (sX + dx >= IMAGE_MAX_X)
      {
         dx = IMAGE_MAX_X - sX - 1;
      }
      for (dy = -3; dy <= 3; dy++)
      {
         if (sY + dy < 0)
         {
            dy = -sY;
         }
         else if (sY + dy >= IMAGE_MAX_Y)
         {
            break;
         }  
		 lSum += Filtered.Array[sX+dx][sY+dy];
         usArea += 1;
      }
   }
   usMean = (ushort) (lSum / usArea);

   return usMean;
}


/************************ FUNCTION *******************************
 *
 * NAME:    ThresholdSkin
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: 
 *
 * CALL:    ThresholdSkin()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/07/99 JMM   Created
 * 01/08/02 JMM	Added skin/bone threshold and image min to shmem
 *						to create color bone regions for chinese printout
 *
 ****************************************************************/
void ThresholdSkin ()
{
   ushort usMaxBin;
   long i;
   ushort usX;
   ushort usY;
   ushort* pRow = NULL;
   long lImageArea;
   long lMinArea;
   long lArea;
   long lBoneArea;
   long lFifteenPercentArea;
   ushort usSkinBoneEstimate;
   ushort usLeftLimit;
   ushort usRightLimit;
   long lLeftArea;
   long lRightArea;
   ushort usRange;
   long di;
   ulong ulSkinBoneValleyMin;
   long lSkinBoneValleyBin;
   ulong ulPeak;

   lImageArea = IMAGE_MAX_X * IMAGE_MAX_Y;
   lMinArea = lImageArea / 1000;
   
   //usMaxBin = MAX_GRAYLEVEL;
   usMaxBin = MAX_GRAYLEVEL/HIST_SCALE_FACTOR;
   i = ImageInfo.usMinImage;
   lArea = HistFFS.lHistogram[i];
   do 
   {
      lArea += HistFFS.lHistogram[++i];
   }while (lArea < lMinArea && i <= usMaxBin);
   ImageInfo.usMinImage = i;
   lMinArea = lArea;

   /* Compute skin/bone threshold */
   //lBoneArea = (lImageArea - HistFFS.lHistogram[MAX_GRAYLEVEL]) / 2;
   lBoneArea = (lImageArea - HistFFS.lHistogram[MAX_GRAYLEVEL/HIST_SCALE_FACTOR]) / 2;
   lArea = lMinArea;
   do
   {
      lArea += HistFFS.lHistogram[++i];
   }while(lArea < lBoneArea && i <= usMaxBin);
   usSkinBoneEstimate = (ushort) i;
   lBoneArea = lArea;

   /* Find L and R limits on search for bone/skin valley */
   lFifteenPercentArea = (lImageArea - lMinArea - HistFFS.lHistogram[usMaxBin]) * 15 / 100;
   do
   {
      lArea -= HistFFS.lHistogram[i--];
   }while(lArea > lBoneArea - lFifteenPercentArea);
   usLeftLimit = (ushort) i;
   lLeftArea = lArea;

   lArea = lBoneArea;
   i = usSkinBoneEstimate;
   do 
   {
      lArea += HistFFS.lHistogram[++i];
   }while (lArea < lBoneArea + lFifteenPercentArea);
   usRightLimit = (ushort) i;
   lRightArea = lArea;

   /* Find valley */
   usRange = 3;
   ulSkinBoneValleyMin = (ulong) lImageArea;
   do 
   {
	  ulPeak = 0;
      for (di = -usRange; di <= usRange; di++)
      {
		 ulPeak += HistFFS.lHistogram[i+di];
      }

	  if (ulPeak < ulSkinBoneValleyMin)
      {
		 ulSkinBoneValleyMin = ulPeak;
         lSkinBoneValleyBin = i;
      }
      lArea -= HistFFS.lHistogram[i];
      i--;
   }while (lArea > lLeftArea && i > ImageInfo.usMinImage);

   //ImageInfo.usSkinBoneThreshold = (ushort) lSkinBoneValleyBin;
   ImageInfo.usSkinBoneThreshold = (ushort) lSkinBoneValleyBin * HIST_SCALE_FACTOR;
   ImageInfo.usBone = ImageInfo.usSkinBoneThreshold / 2;
   ImageInfo.usMinImage *= HIST_SCALE_FACTOR;
   ImageInfo.usMaxImage *= HIST_SCALE_FACTOR;

   CreateDisplayLUT();
   ResampleForDisplay();

   for (usX = 0; usX < IMAGE_MAX_X; usX++)
   {
      pRow = Filtered.Array[usX];
      for (usY = 0; usY < IMAGE_MAX_Y; usY++)
      {
         if (*(pRow+usY) < ImageInfo.usAir && *(pRow+usY) > ImageInfo.usSkinBoneThreshold)
         {
            *(pRow+usY) = ImageInfo.usSkin;
         }
         else if (*(pRow+usY) <= ImageInfo.usSkinBoneThreshold)
         {
            *(pRow+usY) = ImageInfo.usBone;
         }
      }
   }
	
//	pShMem->Histogram.usSkinBoneThreshold = ImageInfo.usSkinBoneThreshold;
//	pShMem->Histogram.usMinImage = ImageInfo.usMinImage;

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    CreateHistogram
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Creates histogram of input image
 *
 * CALL:    CreateHistogram(HISTOGRAM* pHistogram, IMAGE* pImage)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/07/99 JMM   Created
 *
 ****************************************************************/
void CreateHistogram (HISTOGRAM* pHistogram, IMAGE* pImage) //灰度分布 
{
   long i;
   ushort usX;
   ushort usY;
   ushort* pRow;

   //for (i = 0; i <= MAX_GRAYLEVEL; i++)
   for (i = 0; i <= MAX_GRAYLEVEL/HIST_SCALE_FACTOR; i++)
   {
	  pHistogram->lHistogram[i] = 0;
   }

   for (usX = 0; usX < IMAGE_MAX_X; usX++)
   {
      pRow = pImage->Array[usX];
      for (usY = 0; usY < IMAGE_MAX_Y; usY++)
      {
		 //pHistogram ->lHistogram[*(pRow+usY)] += 1;
         pHistogram->lHistogram[*(pRow+usY)/HIST_SCALE_FACTOR] += 1;
      }
   }

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FilterSkinAndBone
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Median filters the skin and bone regions to eliminate holes
 *
 * CALL:    FilterSkinAndBone()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 *
 ****************************************************************/
void FilterSkinAndBone()
{
   ushort usSkinCount;
   ushort usBoneCount;
   ushort usX;
   ushort usY;
   short sDY;
   ushort* pRow0 = NULL;
   ushort* pRow1 = NULL;
   ushort* pRow2 = NULL;


   for (usX = 1; usX < IMAGE_MAX_X - 1; usX++)
   {
      pRow0 = Filtered.Array[usX - 1];
      pRow1 = Filtered.Array[usX];
      pRow2 = Filtered.Array[usX + 1];
      for (usY = 1; usY < IMAGE_MAX_Y - 1; usY++)
      {
         usSkinCount = 0;
         usBoneCount = 0;
         if (pRow1[usY] != ImageInfo.usAir)
         {
            for (sDY = -1; sDY <= 1; sDY++)
            {
               if (*(pRow0 + usY + sDY) == ImageInfo.usSkin)
               {
                  usSkinCount++;
               }
               else if (*(pRow0 + usY + sDY) == ImageInfo.usBone)
               {
                  usBoneCount++;
               }
               if (*(pRow1 + usY + sDY) == ImageInfo.usSkin)
               {
                  usSkinCount++;
               }
               else if (*(pRow1 + usY + sDY) == ImageInfo.usBone)
               {
                  usBoneCount++;
               }
               if (*(pRow2 + usY + sDY) == ImageInfo.usSkin)
               {
                  usSkinCount++;
               }
               else if (*(pRow2 + usY + sDY) == ImageInfo.usBone)
               {
                  usBoneCount++;
               }
            }   
         }
         if (usSkinCount > 5)
         {
            Segmented.Array[usX][usY] = ImageInfo.usSkin;
         }
         else if (usBoneCount > 5)
         {
            Segmented.Array[usX][usY] = ImageInfo.usBone;
         }
         else 
         {
            Segmented.Array[usX][usY] = *(pRow1 + usY);
         }
      }
   }
   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindMidlines
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Finds midlines of fingers using segmented image
 *
 * CALL:    FindMidlines (PROFILES* pProfiles)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/08/99 JMM   Created
 *
 ****************************************************************/
void FindMidlines (PROFILES* pProfiles)
{
   ushort f;
   ushort fCount;
   BMDPOINT Proximal[MAX_NUM_ACTUAL_FINGERS];
   BMDPOINT Distal[MAX_NUM_ACTUAL_FINGERS]; 

   memset(Proximal, 0, sizeof(Proximal));
   memset(Distal, 0, sizeof(Distal));
   
   FindProfileBoneEdges2(pProfiles);

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      Proximal[f].X = (pProfiles->sLeftBone[f][1] + pProfiles->sRightBone[f][1]) / 2;
      Proximal[f].Y = pProfiles->sYprofile[f][1];
      Distal[f].X = (pProfiles->sLeftBone[f][0] + pProfiles->sRightBone[f][0]) / 2;
      Distal[f].Y = pProfiles->sYprofile[f][0];
	  fCount++;
   }
   CalcMidlineEquation(ImageInfo.usNumFingers, Proximal, Distal, pMidline, Finger); //use bone points 

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindProfileBoneEdges2
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Finds bone edges along _|_ profiles using segmented image
 *
 * CALL:    FindProfileBoneEdges2 (PROFILES* pProfiles)
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/09/99 JMM   Created
 *
 ****************************************************************/
void FindProfileBoneEdges2 (PROFILES* pProfiles)
{
   ushort f;
   ushort fCount;
   ushort p;
   short sStopL;
   short sStopR;
   short sStartX;
   short sLeft;
   short sRight;
   short sX;
   short sY;
   ushort* pRow0 = NULL;
   ushort* pRow1 = NULL;
   ushort* pRow2 = NULL;


   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      for (p = 0; p < NUM_PROFILES; p++)
      {
         sRight = 0;
         sLeft = 0;
         sY = pProfiles->sYprofile[f][p];
         if (Finger[f].sDeltaX != 0)
         {
            sStartX = (short)((sY - Finger[f].dIntercept) * Finger[f].sDeltaX / Finger[f].sDeltaY);
         }
         else
         {
            sStartX = Finger[f].Fingertip.X;
         }
         sStopR = pProfiles->sRightSkin[f][p];
         sStopL = pProfiles->sLeftSkin[f][p];
         pProfiles->sFingerWidth[f][p] = sStopL - sStopR;

         if (sStopR > sStopL)
         {
            //return (ec = ERR_IMAGE_FIND_PROFILE_BONE_EDGES);
			Finger[f].usError = ERR_IMAGE_FIND_PROFILE_BONE_EDGES;
		 	(pBoneInfo+f)->usFound = FALSE;
			ImageInfo.usNumFingers--;
		 	break;
         }

         for (sX = sStartX; sX >= sStopR; sX--)
         {
            pRow0 = Segmented.Array[sX];
            pRow1 = Segmented.Array[sX+1];
            pRow2 = Segmented.Array[sX+2];
            if (*(pRow0+sY) == ImageInfo.usSkin && *(pRow1+sY) == ImageInfo.usSkin && *(pRow2+sY) == ImageInfo.usSkin)
            {
               sRight = sX;
               break;
            }
         }
         for (sX = sStartX; sX <= sStopL; sX++)
         {
            pRow0 = Segmented.Array[sX];
            pRow1 = Segmented.Array[sX+1];
            pRow2 = Segmented.Array[sX+2];
            if (*(pRow0+sY) == ImageInfo.usSkin && *(pRow1+sY) == ImageInfo.usSkin && *(pRow2+sY) == ImageInfo.usSkin)
            {
               sLeft = sX - 1;
               break;
            }
         }
         pProfiles->sLeftBone[f][p] = sLeft;
         pProfiles->sRightBone[f][p] = sRight;
         pProfiles->sBoneWidth[f][p] = sRight - sLeft;
      }
	  if (Finger[f].usError == SUCCESS)
	  {
		 fCount++;
	  }
   }
   return;
}



/************************ FUNCTION *******************************
 *
 * NAME:    FindJoints
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Defines endpoints of middle phalanges
 *
 * CALL:    FindJoints ()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/08/99 JMM   Created
 *
 ****************************************************************/
void FindJoints ()
{
   FindTip(ImageInfo.usSkin, &Segmented);
   FindPIPandDIP(BONETIP, &Copy);
   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    ReadjustMidlines
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Readjust midlines to guarantee bone ends are _|_ to 
 *          true joint spaces in the image
 *
 * CALL:    ReadjustMidlines ()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/08/99 JMM   Created
 *
 ****************************************************************/
void ReadjustMidline()
{ 
   ushort f;
   ushort fCount;
   short sDeltaX_L;
   short sDeltaX_R;
   BMDPOINT Proximal[MAX_NUM_ACTUAL_FINGERS];
   BMDPOINT Distal[MAX_NUM_ACTUAL_FINGERS];   

   memset(Proximal, 0, sizeof(Proximal));
   memset(Distal, 0, sizeof(Distal));

   FindRandLjointEdges();

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      sDeltaX_R = Finger[f].PIP.X - Finger[f].PIP_R.X;
      sDeltaX_L = Finger[f].PIP_L.X - Finger[f].PIP.X;
      if (abs(sDeltaX_L - sDeltaX_R) > 10)
      {
         Finger[f].PIP.X = (Finger[f].PIP_R.X + Finger[f].PIP_L.X) / 2;
         Finger[f].PIP.Y = (Finger[f].PIP_R.Y + Finger[f].PIP_L.Y) / 2;
      }

      sDeltaX_R = Finger[f].DIP.X - Finger[f].DIP_R.X;
      sDeltaX_L = Finger[f].DIP_L.X - Finger[f].DIP.X;
      if (abs(sDeltaX_L - sDeltaX_R) > 10)
      {
         Finger[f].DIP.X = (Finger[f].DIP_R.X + Finger[f].DIP_L.X) / 2;
         Finger[f].DIP.Y = (Finger[f].DIP_R.Y + Finger[f].DIP_L.Y) / 2;
      }
	  Proximal[f].X = Finger[f].PIP.X;
	  Proximal[f].Y = Finger[f].PIP.Y;
	  Distal[f].X = Finger[f].DIP.X;
	  Distal[f].Y = Finger[f].DIP.Y;

	  fCount++;
   }
   CalcMidlineEquation(ImageInfo.usNumFingers, Proximal, Distal, pMidline, Finger);

   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindRandLjointEdges
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Finds the R and L edge points for both the PIP and DIP
 *
 * CALL:    FindRandLjointEdges ()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/09/99 JMM   Created
 *
 ****************************************************************/
void FindRandLjointEdges()
{
   ushort usEdge;
   ushort f;
   ushort fCount;
   short sDX;
   short sYPerpendicular;
   short sXPrev;
   short sYPrev;

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      /* Find right edge of DIP */
      usEdge = 0;
      sDX = 0;
	  sXPrev = 0;
	  sYPrev = 0;
      do
      {
		 sYPerpendicular = (Finger[f].DIP.Y * 1000 + (Finger[f].sDeltaX * sDX * 1000 / Finger[f].sDeltaY)) / 1000; 
		 if (Finger[f].DIP.X - sDX >= 0 && sYPerpendicular >= 0 && sYPerpendicular < IMAGE_MAX_Y)
		 {
         //if (Segmented.Array[Finger[f].DIP.X - sDX][sYPerpendicular] == ImageInfo.usSkin)
			if (Segmented.Array[Finger[f].DIP.X - sDX][sYPerpendicular] == ImageInfo.usSkin || sDX >= FINGER_WIDTH / 2)
            {
               Finger[f].DIP_R.X = Finger[f].DIP.X - sDX - 1;
               Finger[f].DIP_R.Y = sYPerpendicular;
               usEdge = 1;
            }
		 }
		 else if (usEdge == 0)   //?? 
		 {
		    // set to last edge point within image boundaries 
			Finger[f].DIP_R.X = sXPrev;
			Finger[f].DIP_R.Y = sYPrev;
			usEdge = 1;
		 }
         sDX++;
		 sXPrev = Finger[f].DIP.X - sDX;
		 sYPrev = sYPerpendicular;
      //}while (sDX < FINGER_WIDTH/2 && usEdge == 0);
	  }while (usEdge == 0);

      /* Find left edge of DIP */
      usEdge = 0;
      sDX = 0;
	  sXPrev = 0;
	  sYPrev = 0;
      do
      {
		 sYPerpendicular = (Finger[f].DIP.Y * 1000 - (Finger[f].sDeltaX * sDX * 1000 / Finger[f].sDeltaY)) / 1000;
		 if (Finger[f].DIP.X + sDX < IMAGE_MAX_X && sYPerpendicular >= 0 && sYPerpendicular < IMAGE_MAX_Y)
		 {
         //if (Segmented.Array[Finger[f].DIP.X + sDX][sYPerpendicular] == ImageInfo.usSkin)
		    if (Segmented.Array[Finger[f].DIP.X + sDX][sYPerpendicular] == ImageInfo.usSkin || sDX >= FINGER_WIDTH / 2)
            {
               Finger[f].DIP_L.X = Finger[f].DIP.X + sDX + 1;
               Finger[f].DIP_L.Y = sYPerpendicular;
               usEdge = 1;
            }
		 }
		 else if (usEdge == 0)
		 {
			// set to last edge point within image boundaries
            Finger[f].DIP_L.X = sXPrev;
            Finger[f].DIP_L.Y = sYPrev;
            usEdge = 1;	
		 }
         sDX++;
		 sXPrev = Finger[f].DIP.X + sDX;
		 sYPrev = sYPerpendicular;
      //}while (sDX < FINGER_WIDTH/2 && usEdge == 0);
	  }while (usEdge == 0);

      /* Find right edge of PIP */
      usEdge = 0;
      sDX = 0;
	  sXPrev = 0;
	  sYPrev = 0;	  
      do
      {
		 sYPerpendicular = (Finger[f].PIP.Y * 1000 + (Finger[f].sDeltaX * sDX * 1000 / Finger[f].sDeltaY)) / 1000;
		 if (Finger[f].PIP.X - sDX >= 0 && sYPerpendicular >= 0 && sYPerpendicular < IMAGE_MAX_Y)
		 {
         //if (Segmented.Array[Finger[f].PIP.X - sDX][sYPerpendicular] == ImageInfo.usSkin)
			if (Segmented.Array[Finger[f].PIP.X - sDX][sYPerpendicular] == ImageInfo.usSkin || sDX >= FINGER_WIDTH / 2)
            {
               Finger[f].PIP_R.X = Finger[f].PIP.X - sDX - 1;
               Finger[f].PIP_R.Y = sYPerpendicular;
               usEdge = 1;
			}
         }
		 else if (usEdge == 0)
		 {
			// set to last edge point within image boundaries
			Finger[f].PIP_R.X = sXPrev;
			Finger[f].PIP_R.Y = sYPrev;
			usEdge = 1;
		 }
         sDX++;
		 sXPrev = Finger[f].PIP.X - sDX;
		 sYPrev = sYPerpendicular;
      //}while (sDX < FINGER_WIDTH/2 && usEdge == 0);
	  }while (usEdge == 0);

      /* Find left edge of PIP */
      usEdge = 0;
      sDX = 0;
	  sXPrev = 0;
	  sYPrev = 0;
      do
      {
		 sYPerpendicular = (Finger[f].PIP.Y * 1000 - (Finger[f].sDeltaX * sDX * 1000 / Finger[f].sDeltaY)) / 1000;
		 if (Finger[f].PIP.Y + sDX < IMAGE_MAX_X && sYPerpendicular >= 0 && sYPerpendicular < IMAGE_MAX_Y)
		 {
         //if (Segmented.Array[Finger[f].PIP.X + sDX][sYPerpendicular] == ImageInfo.usSkin)
			if (Segmented.Array[Finger[f].PIP.X + sDX][sYPerpendicular] == ImageInfo.usSkin || sDX >= FINGER_WIDTH / 2)
            {
               Finger[f].PIP_L.X = Finger[f].PIP.X + sDX + 1;
               Finger[f].PIP_L.Y = sYPerpendicular;
               usEdge = 1;
            }
		 }
		 else if (usEdge == 0)
		 {
			// set to last edge point within image boundaries
			Finger[f].PIP_L.X = sXPrev;
			Finger[f].PIP_L.X = sYPrev;
			usEdge = 1;
		 }
         sDX++;
		 sXPrev = Finger[f].PIP.X + sDX;
		 sYPrev = sYPerpendicular;
      //}while (sDX < FINGER_WIDTH/2 && usEdge == 0);
	  }while (usEdge == 0);
	  fCount++;
   }
 
   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    FindBoneROIs
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Find bone ROIs in which BMD will be computed
 *
 * CALL:    FindBoneROIs ()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/08/99 JMM   Created
 *
 ****************************************************************/
void FindBoneROIs()
{  
   FindRandLEdges();
   InterpolateMissingBoneEdges();
   return;
}



/************************ FUNCTION *******************************
 *
 * NAME:    FindRandLEdges
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Record R and L skin and bone edges and finger widths along midlines
 *
 * CALL:    FindRandLEdges ()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/14/99 JMM   Created
 *
 ****************************************************************/
void FindRandLEdges()
{
   ushort f;
   ushort fCount;
   short sX;
   short sY;
   short sDX;
   short sYPerpendicular;
   short sXLastL;
   short sXLastR;
   short sXPrevL;
   short sXPrevR;
   short sXRange;
   ushort usSkinR;
   ushort usSkinL;
   ushort usBoneR;
   ushort usBoneL;
   BMDPOINT SkinR;
   BMDPOINT SkinL;

   memset(&SkinR, 0, sizeof(SkinR));
   memset(&SkinL, 0, sizeof(SkinL));

   sXRange = 6;
   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }
	
      for (sY = 0; sY < Finger[f].DIP.Y; sY++)
      {
         BoneEdges[f].Left[sY].X = 0;
         BoneEdges[f].Left[sY].Y = 0;
         BoneEdges[f].usDistToEdgeL[sY] = 0;
         BoneEdges[f].Right[sY].X = 0;
         BoneEdges[f].Right[sY].Y = 0;
         BoneEdges[f].usDistToEdgeR[sY] = 0;
      }
      for (sY = Finger[f].PIP.Y + 1; sY < IMAGE_MAX_Y; sY++)
      {
         BoneEdges[f].Left[sY].X = 0;
         BoneEdges[f].Left[sY].Y = 0;
         BoneEdges[f].usDistToEdgeL[sY] = 0;
         BoneEdges[f].Right[sY].X = 0;
         BoneEdges[f].Right[sY].Y = 0;
         BoneEdges[f].usDistToEdgeR[sY] = 0;
      }
	  sXLastL = 0;
	  sXLastR = 0;
      sXPrevL = 0;
      sXPrevR = 0;
      for (sY = Finger[f].PIP.Y; sY >= Finger[f].DIP.Y; sY--)
      {
         if (Finger[f].sDeltaX != 0)
         {
            sX = (short) ((sY - Finger[f].dIntercept) * Finger[f].sDeltaX / Finger[f].sDeltaY);
         }
         else
         {
            sX = Finger[f].PIP.X;
         }
         Finger[f].sMidlineCoords[sY] = sX;
         
         sDX = 0;
         usSkinL = 0;
         usBoneL = 0;
         BoneEdges[f].usDistToEdgeL[sY] = 0;
         do 
         {
            if ((sX + sDX >= 0) && (sX + sDX < IMAGE_MAX_X))
            {
			   sYPerpendicular = (sY * 1000 - (Finger[f].sDeltaX * sDX * 1000 / Finger[f].sDeltaY)) / 1000;
               if (sYPerpendicular >= 0 && sYPerpendicular <= IMAGE_MAX_Y - 1) 
               {
                  if (usBoneL == 1 && usSkinL == 0 && Segmented.Array[sX+sDX][sYPerpendicular] == ImageInfo.usAir && Segmented.Array[sX+sDX+1][sYPerpendicular] == ImageInfo.usAir && Segmented.Array[sX+sDX+2][sYPerpendicular] == ImageInfo.usAir)
                  {
                     SkinL.X = sX + sDX;
                     SkinL.Y = sYPerpendicular;
                     usSkinL = 1;
                  }
                  else if (usBoneL == 0 && Segmented.Array[sX+sDX][sYPerpendicular] == ImageInfo.usSkin && Segmented.Array[sX+sDX+1][sYPerpendicular] == ImageInfo.usSkin && Segmented.Array[sX+sDX+2][sYPerpendicular] == ImageInfo.usSkin)
                  {
                     if ((sY != Finger[f].PIP.Y && (abs(sXPrevL - (sX + sDX)) <= sXRange)) || (sY == Finger[f].PIP.Y))
                     {
                        BoneEdges[f].Left[sY].X = sX + sDX;
                        BoneEdges[f].Left[sY].Y = sYPerpendicular;
                        BoneEdges[f].usDistToEdgeL[sY] = sDX;
                        sXLastL = sX + sDX;
                        usBoneL = 1;
                     }
                  }
				  else if (usBoneL == 1 && Segmented.Array[sX+sDX][sYPerpendicular] == ImageInfo.usBone && Segmented.Array[sX+sDX+1][sYPerpendicular] == ImageInfo.usBone && Segmented.Array[sX+sDX+2][sYPerpendicular] == ImageInfo.usBone)
				  {
				     usBoneL = 0;
				  }
               }
            }
            sDX++;
         }while (sDX < FINGER_WIDTH && usSkinL == 0);
	     sXPrevL = sXLastL;

         sDX = 0;


         usSkinR = 0;
         usBoneR = 0;
         BoneEdges[f].usDistToEdgeR[sY] = 0;
         do
         {
            if ((sX - sDX >= 0) && (sX - sDX < IMAGE_MAX_X))
            {
               sYPerpendicular = (sY * 1000 + (Finger[f].sDeltaX * sDX * 1000 / Finger[f].sDeltaY)) / 1000;
			   if (sYPerpendicular >= 0 && sYPerpendicular <= IMAGE_MAX_Y - 1) 	
               {
                  if (usBoneR == 1 && usSkinR == 0 && Segmented.Array[sX-sDX][sYPerpendicular] == ImageInfo.usAir && Segmented.Array[sX-sDX-1][sYPerpendicular] == ImageInfo.usAir && Segmented.Array[sX-sDX-2][sYPerpendicular] == ImageInfo.usAir)
                  {
                     SkinR.X = sX - sDX;
                     SkinR.Y = sYPerpendicular;
                     usSkinR = 1;
                  }
                  else if (usBoneR == 0 && Segmented.Array[sX-sDX][sYPerpendicular] == ImageInfo.usSkin && Segmented.Array[sX-sDX-1][sYPerpendicular] == ImageInfo.usSkin && Segmented.Array[sX-sDX-2][sYPerpendicular] == ImageInfo.usSkin)
                  {
                     if ((sY != Finger[f].PIP.Y && (abs(sXPrevR - (sX - sDX)) <= sXRange)) || (sY == Finger[f].PIP.Y))
                     {
                        BoneEdges[f].Right[sY].X = sX - sDX;
                        BoneEdges[f].Right[sY].Y = sYPerpendicular;
                        BoneEdges[f].usDistToEdgeR[sY] = sDX;
                        sXLastR = sX - sDX;
                        usBoneR = 1;
                     }
                  }
				  else if (usBoneR == 1 && Segmented.Array[sX-sDX][sYPerpendicular] == ImageInfo.usBone && Segmented.Array[sX-sDX-1][sYPerpendicular] == ImageInfo.usBone && Segmented.Array[sX-sDX-2][sYPerpendicular] == ImageInfo.usBone)
				  {
				     usBoneR = 0;
				  }
               }
            }
            sDX++;
         }while (sDX < FINGER_WIDTH && usSkinR == 0);
		 sXPrevR = sXLastR;

         if (usSkinL == 1 && usSkinR == 1)
         {
			Finger[f].usWidth[sY] = (ushort) sqrt((double)((long)(SkinL.X - SkinR.X)*(SkinL.X - SkinR.X) + (long)(SkinL.Y - SkinR.Y)*(SkinL.Y - SkinR.Y)));
         }
         /*else
         {
            n = 0;
            while (Finger[f].usWidth[sY + n] != 0)
            {
               Finger[f].usWidth[sY] = Finger[f].usWidth[sY + 1];
               n--;
            }
         }*/
      }
	  fCount++;
   }
   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    InterpolateMissingBoneEdges
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Fill in any missing bone edge points between the PIP
 *          and DIP of each finger.  Also copies edge points to 
 *          ROI for shmem
 *
 * CALL:    InterpolateMissingBoneEdges ()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 06/14/99 JMM   Created
 *
 ****************************************************************/
void InterpolateMissingBoneEdges()
{
   ushort f;
   ushort fCount;
   short sX;
   short sY;
   ushort usNumZeros;
   ushort usDY;
   short sXInterpolated;
   short sYInterpolated;
   short sXMidline;
   short sX1, sX2, sY1, sY2;
   short sXDelta;
   short sYDelta;
   ushort usNumPoints;
   short sXPerpendicular;
   short sYPerpendicular;

 
   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	
      usNumPoints = 0;

      // Fill in points on the left side
      for (sY = Finger[f].DIP.Y; sY <= Finger[f].PIP.Y; sY++)
      {
         sX = Finger[f].sMidlineCoords[sY];
         if (BoneEdges[f].usDistToEdgeL[sY] == 0)
         {
            usNumZeros = 1;
            // assumes PIP is farther right than DIP
            while (BoneEdges[f].usDistToEdgeL[sY+usNumZeros] == 0 && (sY + usNumZeros) <= Finger[f].PIP.Y)
            {
               usNumZeros++;
			   if (usNumZeros >= IMAGE_MAX_X)
			   {
				  //return ERR_IMAGE_INTERPOLATE_MISSING_EDGES;
				  //ec = ERR_IMAGE_INTERPOLATE_MISSING_EDGES;
				  Finger[f].usError = ERR_IMAGE_INTERPOLATE_MISSING_EDGES;
		 		  (pBoneInfo+f)->usFound = FALSE;
				  ImageInfo.usNumFingers--;
		 		  break;
			   }
            }
			if (Finger[f].usError != SUCCESS)
			{
			   break;
			}
            if (sY == Finger[f].DIP.Y)
            {
               for (usDY = 0; usDY < usNumZeros; usDY++)
               {
                  sXInterpolated = BoneEdges[f].Left[sY + usNumZeros].X;
                  sYInterpolated = BoneEdges[f].Left[sY + usNumZeros].Y - usNumZeros + usDY;
                  if (sYInterpolated < 0)
                  {
                     sYInterpolated = 0;
                  }
                  sXMidline = Finger[f].sMidlineCoords[sY + usDY];
                  BoneEdges[f].Left[sY + usDY].X = sXInterpolated;
                  BoneEdges[f].Left[sY + usDY].Y = sYInterpolated;
				  BoneEdges[f].usDistToEdgeL[sY + usDY] = sXInterpolated - sXMidline;
                  //(pRoi+f)->Coords[usNumPoints].X = sXInterpolated;
                  //(pRoi+f)->Coords[usNumPoints].Y = sYInterpolated;
                  //usNumPoints++;
               }
            }
            else if ((sY + usNumZeros - 1) == Finger[f].PIP.Y)
            {
               for (usDY = 0; usDY < usNumZeros; usDY++)
               {
                  sXInterpolated = BoneEdges[f].Left[sY - 1].X;
                  sYInterpolated = BoneEdges[f].Left[sY - 1].Y + usDY + 1;
                  if (sYInterpolated >= IMAGE_MAX_Y)
                  {
                     sYInterpolated = IMAGE_MAX_Y - 1;
                  }
                  sXMidline = Finger[f].sMidlineCoords[sY + usDY];
                  BoneEdges[f].Left[sY + usDY].X = sXInterpolated;
                  BoneEdges[f].Left[sY + usDY].Y = sYInterpolated;
				  BoneEdges[f].usDistToEdgeL[sY + usDY] = sXInterpolated - sXMidline;
                  //(pRoi+f)->Coords[usNumPoints].X = sXInterpolated;
                  //(pRoi+f)->Coords[usNumPoints].Y = sYInterpolated;
                  //usNumPoints++;
               }
            }
            else
            {
               sX1 = BoneEdges[f].Left[sY - 1].X;
               sY1 = BoneEdges[f].Left[sY - 1].Y;
               sX2 = BoneEdges[f].Left[sY + usNumZeros].X;
               sY2 = BoneEdges[f].Left[sY + usNumZeros].Y;
               sXDelta = sX2 - sX1;
               sYDelta = sY2 - sY1;
               for (usDY = 0; usDY < usNumZeros; usDY++)
               {
				  sXInterpolated = (short) (((ushort)sX1 * 1000 + (sXDelta * (usDY + 1) * 1000 / (usNumZeros + 1)) + 500) / 1000);
				  sYInterpolated = (short) (((ushort)sY1 * 1000 + (sYDelta * (usDY + 1) * 1000 / (usNumZeros + 1)) + 500) / 1000);
                  sXMidline = Finger[f].sMidlineCoords[sY + usDY];
                  BoneEdges[f].Left[sY + usDY].X = sXInterpolated;
                  BoneEdges[f].Left[sY + usDY].Y = sYInterpolated;
				  BoneEdges[f].usDistToEdgeL[sY + usDY] = sXInterpolated - sXMidline;
                  //(pRoi+f)->Coords[usNumPoints].X = sXInterpolated;
                  //(pRoi+f)->Coords[usNumPoints].Y = sYInterpolated;
                  //usNumPoints++;
               }
            }
         }
         else
         {
            (pRoi+f)->Coords[usNumPoints].X = BoneEdges[f].Left[sY].X;
            (pRoi+f)->Coords[usNumPoints].Y = BoneEdges[f].Left[sY].Y;
            usNumPoints++;
         }
	  }

	  // if left side yields error, move on to next finger.  
	  if (Finger[f].usError != SUCCESS)
	  {
		 continue;
	  }

      // Fill in points on the right side
      for (sY = Finger[f].DIP.Y; sY <= Finger[f].PIP.Y; sY++)
      {
         sX = Finger[f].sMidlineCoords[sY];
         if (BoneEdges[f].usDistToEdgeR[sY] == 0)
         {
            usNumZeros = 1;
            // assumes PIP is farther right than DIP 
            while (BoneEdges[f].usDistToEdgeR[sY+usNumZeros] == 0 && (sY + usNumZeros) <= Finger[f].PIP.Y)
            {
               usNumZeros++;
			   if (usNumZeros >= IMAGE_MAX_X)
			   {
				  //return ERR_IMAGE_INTERPOLATE_MISSING_EDGES;
				  //ec = ERR_IMAGE_INTERPOLATE_MISSING_EDGES;
				  Finger[f].usError = ERR_IMAGE_INTERPOLATE_MISSING_EDGES;
				  (pBoneInfo+f)->usFound = FALSE;
				  ImageInfo.usNumFingers--;
		 		  break;
			   }
            }
			if (Finger[f].usError != SUCCESS)
			{
			   break;
			}
            if (sY == Finger[f].DIP.Y)
            {
               for (usDY = 0; usDY < usNumZeros; usDY++)
               {
                  sXInterpolated = BoneEdges[f].Right[sY + usNumZeros].X;
                  sYInterpolated = BoneEdges[f].Right[sY + usNumZeros].Y - usNumZeros + usDY;
                  if (sYInterpolated < 0)
                  {
                     sYInterpolated = 0;
                  }
                  sXMidline = Finger[f].sMidlineCoords[sY + usDY];
                  BoneEdges[f].Right[sY + usDY].X = sXInterpolated;
                  BoneEdges[f].Right[sY + usDY].Y = sYInterpolated;
				  BoneEdges[f].usDistToEdgeR[sY + usDY] = sXMidline - sXInterpolated;
                  //(pRoi+f)->Coords[usNumPoints].X = sXInterpolated;
                  //(pRoi+f)->Coords[usNumPoints].Y = sYInterpolated;
                  //usNumPoints++;
               }
            }
            else if ((sY + usNumZeros - 1) == Finger[f].PIP.Y)
            {
               for (usDY = 0; usDY < usNumZeros; usDY++)
               {
                  sXInterpolated = BoneEdges[f].Right[sY - 1].X;
                  sYInterpolated = BoneEdges[f].Right[sY - 1].Y + usDY + 1;
                  if (sYInterpolated >= IMAGE_MAX_Y)
                  {
                     sYInterpolated = IMAGE_MAX_Y - 1;
                  }
                  sXMidline = Finger[f].sMidlineCoords[sY + usDY];
                  BoneEdges[f].Right[sY + usDY].X = sXInterpolated;
                  BoneEdges[f].Right[sY + usDY].Y = sYInterpolated;
                  //BoneEdges[f].usDistToEdgeR[sY + usDY] = (ushort) sqrt(pow((sXInterpolated - sXMidline),2) + pow((sYInterpolated - (sY + usDY)),2));
				  //BoneEdges[f].usDistToEdgeR[sY + usDY] = (ushort) sqrt((double)(sXInterpolated - sXMidline) * (sXInterpolated - sXMidline) + (sYInterpolated - (sY + usDY)) * (sYInterpolated - (sY + usDY)));
				  BoneEdges[f].usDistToEdgeR[sY + usDY] = sXMidline - sXInterpolated;
                  //(pRoi+f)->Coords[usNumPoints].X = sXInterpolated;
                  //(pRoi+f)->Coords[usNumPoints].Y = sYInterpolated;
                  //usNumPoints++;
               }
            }
            else
            {
               sX1 = BoneEdges[f].Right[sY - 1].X;
               sY1 = BoneEdges[f].Right[sY - 1].Y;
               sX2 = BoneEdges[f].Right[sY + usNumZeros].X;
               sY2 = BoneEdges[f].Right[sY + usNumZeros].Y;
               sXDelta = sX2 - sX1;
               sYDelta = sY2 - sY1;

               for (usDY = 0; usDY < usNumZeros; usDY++)
               {
				  sXInterpolated = (short) (((ushort)sX1 * 1000 + (sXDelta * (usDY + 1) * 1000 / (usNumZeros + 1)) + 500) / 1000);
				  sYInterpolated = (short) (((ushort)sY1 * 1000 + (sYDelta * (usDY + 1) * 1000 / (usNumZeros + 1)) + 500) / 1000);
                  BoneEdges[f].Right[sY + usDY].X = sXInterpolated;
                  BoneEdges[f].Right[sY + usDY].Y = sYInterpolated;
				  BoneEdges[f].usDistToEdgeR[sY + usDY] = sXMidline - sXInterpolated; 
				  //pRoi+f)->Coords[usNumPoints].X = sXInterpolated; 
                  // (pRoi+f)->Coords[usNumPoints].Y = sYInterpolated;
                  // usNumPoints++;

               }

            }
         }
         else
         {
            (pRoi+f)->Coords[usNumPoints].X = BoneEdges[f].Right[sY].X;
            (pRoi+f)->Coords[usNumPoints].Y = BoneEdges[f].Right[sY].Y;
            usNumPoints++;
         }
      }
      
	  if (Finger[f].usError == SUCCESS)
	  {
		 // fill in edges along PIP and DIP
		 sY = Finger[f].PIP.Y;
         for (sXPerpendicular = BoneEdges[f].Right[sY].X + 1; sXPerpendicular < BoneEdges[f].Left[sY].X; sXPerpendicular++)
		 {
			sYPerpendicular = (sY * 1000 - ((sXPerpendicular - Finger[f].PIP.X) * Finger[f].sDeltaX * 1000 / Finger[f].sDeltaY)) / 1000; 
			(pRoi+f)->Coords[usNumPoints].X = sXPerpendicular;
			(pRoi+f)->Coords[usNumPoints].Y = sYPerpendicular;
			usNumPoints++;
		 }
		 sY = Finger[f].DIP.Y;
		 for (sXPerpendicular = BoneEdges[f].Right[sY].X + 1; sXPerpendicular <= BoneEdges[f].Left[sY].X; sXPerpendicular++)
		 {
			sYPerpendicular = (sY * 1000 - ((sXPerpendicular - Finger[f].DIP.X) * Finger[f].sDeltaX * 1000 / Finger[f].sDeltaY)) / 1000;
			(pRoi+f)->Coords[usNumPoints].X = sXPerpendicular;
			(pRoi+f)->Coords[usNumPoints].Y = sYPerpendicular;
			usNumPoints++;
		 }
		 (pRoi+f)->usNumPoints = usNumPoints;
		 fCount++;
	  }
   }

   return;
}


/*
************************ FUNCTION *******************************
 *
 * NAME:    DrawROIsOnImage
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Draw bone ROIs on Marked image
 *
 * CALL:    DrawROIsOnImage ()
 *
 * RETURNS: none 
 *
 * HISTORY: 
 * 07/08/99 JMM   Created
 *
 ****************************************************************
void DrawROIsOnImage ()
{
   short sX;
   short sY;
   ushort f;
   ushort fCount;
   ushort i;

   for (sX = 0; sX < IMAGE_MAX_X; sX++)
   {
      for (sY = 0; sY < IMAGE_MAX_Y; sY++)
      {
         Marked.Array[sX][sY] = pImage->Array[sX][sY];
      }
   }

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	

      for (i = 0; i < (pRoi+f)->usNumPoints; i++)
      {
         Marked.Array[(pRoi+f)->Coords[i].X][(pRoi+f)->Coords[i].Y] = ROI_GRAYLEVEL;
      }
	  fCount++;
   }
   return;
}
*/


/************************ FUNCTION *******************************
 *
 * NAME:    GetImageStats
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Find mean, standard deviation and SNR of background
 *
 * CALL:    GetImageStats()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 09/24/99 JMM   Created
 *
 ****************************************************************/
ushort GetImageStats (STATS* pShStats, IMAGE* pShImage)
{
   ushort usX;
   ushort usY;
   ushort* pRow = NULL;
   long lMean;
   long lDiff;
   double dSum; 
   double dStandardDeviation;
   double dSNR;

   pImage = pShImage;
   lMean = 0;
   for (usX = STATS_ROI_LEFT; usX <= STATS_ROI_RIGHT; usX++)
   {
	  pRow = pImage->Array[usX];
	  for (usY = STATS_ROI_TOP; usY <= STATS_ROI_BOTTOM; usY++)
	  {
		 lMean += *(pRow+usY);
	  }
   }
   lMean /= STATS_ROI_AREA;
   pShStats->lMean = lMean;
   if (lMean < MIN_MEAN || lMean > MAX_MEAN)
   {
	  return ERR_IMAGE_MEAN_OUT_OF_RANGE;
   }

   dSum = 0;
   for (usX = STATS_ROI_LEFT; usX <= STATS_ROI_RIGHT; usX++)
   {
	  pRow = pImage->Array[usX];
	  for (usY = STATS_ROI_TOP; usY <= STATS_ROI_BOTTOM; usY++)
	  {
		 lDiff = (*(pRow+usY)) - lMean;
		 dSum += (double) lDiff * lDiff; 		 
	  }
   }
   dStandardDeviation = sqrt(dSum / (STATS_ROI_AREA - 1));

   if (dStandardDeviation <= 0)
   {
      dStandardDeviation = 0.001;
   }
   dSNR = lMean / dStandardDeviation;
   pShStats->lSNR = (long) dSNR;

   if (dSNR < MIN_SNR || dSNR > MAX_SNR)
   {
	  return ERR_IMAGE_SNR_OUT_OF_RANGE;
   }

   return SUCCESS;
}


/************************ FUNCTION *******************************
 *
 * NAME:    CreateDisplayLUT
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Create 16->4bps LUT for display
 *
 * CALL:    CreateDisplayLUT ()
 *
 * RETURNS: 
 *
 * HISTORY: 
 * 08/13/99 JMM   Created
 *
 ****************************************************************/
void CreateDisplayLUT ()
{

   // Calculate LUT
   ushort usMin;
   long i;
   long lNumBoneGraylevels;
   long lNumThickBoneGraylevels;
   long lNumThinBoneGraylevels;
   long lNumSkinGraylevels;
   long lNumBackgroundGraylevels;
   long lTemp;
   ushort usBreakpoint;


   usMin = ImageInfo.usMinImage;
   lNumBoneGraylevels = 11;
   lNumSkinGraylevels = 1;
   lNumBackgroundGraylevels = 1;
   lNumThickBoneGraylevels = 5;        // number of 4-bit graylevels reserved for first linear section
   lNumThinBoneGraylevels = 6;         // number of 4-bit graylevels left for second linear section
   lNumThickBoneGraylevels *= DISPLAY_SCALE_FACTOR;    // convert to current bps
   lNumThinBoneGraylevels *= DISPLAY_SCALE_FACTOR;     // convert to current bps

   // Identify end of first linear section, beginning of second
   usBreakpoint = (ImageInfo.usSkinBoneThreshold - usMin) * 4 / 10 + usMin;

   // First linear section
   for (i = usMin; i <= usBreakpoint; i++)
   {
	  lTemp = lNumThickBoneGraylevels * (i - usMin) / (usBreakpoint-usMin);
	  if (lTemp < 0)
      {
         lTemp = 0;
      }
   	  else if (lTemp > MAX_GRAYLEVEL)
      {
         lTemp = MAX_GRAYLEVEL;
      }
	  usDisplayLUT[i] = (ushort) lTemp;
   }

   // Second linear section
   for (i = usBreakpoint + 1; i < ImageInfo.usSkinBoneThreshold; i++)
   {
	  lTemp = lNumThinBoneGraylevels * (i - usBreakpoint) / (ImageInfo.usSkinBoneThreshold - usBreakpoint) + lNumThickBoneGraylevels;
	  if (lTemp < 0)
      {
         lTemp = 0;
      }
	  else if (lTemp > MAX_GRAYLEVEL)
      {
         lTemp = MAX_GRAYLEVEL;
      }
	  usDisplayLUT[i] = (ushort) lTemp;
   }

   // Set skin to one level
   for (i = ImageInfo.usSkinBoneThreshold; i < ImageInfo.usAirSkinThreshold; i++)
   {
      usDisplayLUT[i] = DISPLAY_SCALE_FACTOR * 13;
   }


   // complete the upper end of look up table
   do
   {
      usDisplayLUT[i] = DISPLAY_SCALE_FACTOR * 15;
   	  i++;
   }while (i < MAX_NUM_GRAYLEVELS);
}


/************************ FUNCTION *******************************
 *
 * NAME:    ResampleForDisplay
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: Resamples filtered original from original DPI to LCD size
 *
 * CALL:    ResampleForDisplay()
 *
 * RETURNS: none
 *
 * HISTORY: 
 * 08/13/99 JMM   Created 
 *
 ****************************************************************/
void ResampleForDisplay ()
{
   ushort usX_Resampled;
   ushort usY_Resampled;
   ushort usX;
   ushort usY;

   for (usX_Resampled = 0; usX_Resampled < LCD_MAX_X; usX_Resampled++)
   {
      usX = (ushort) (((long)usX_Resampled * X_RATIO + 512) >> 10);
      if (usX > IMAGE_MAX_X)
      {
         usX = IMAGE_MAX_X;
      }
      for (usY_Resampled = 0; usY_Resampled < DATA_MIN_Y; usY_Resampled++)
      {
         Resampled.Array[usX_Resampled][usY_Resampled] = DISPLAY_SCALE_FACTOR * 15;
      }
      for (usY_Resampled = DATA_MIN_Y; usY_Resampled < DATA_MAX_Y; usY_Resampled++)
      {
         usY = (ushort) ((((long)usY_Resampled - DATA_MIN_Y) * X_RATIO + 512) >> 10);
         if (usY > IMAGE_MAX_Y)
         {
            usY = IMAGE_MAX_Y;
         }
         Resampled.Array[usX_Resampled][usY_Resampled] = Filtered.Array[usX][usY];
      }
      for (usY_Resampled = DATA_MAX_Y; usY_Resampled < LCD_MAX_Y; usY_Resampled++)
      {
         Resampled.Array[usX_Resampled][usY_Resampled] = DISPLAY_SCALE_FACTOR * 15;
      }
   }
   return;
}


/************************ FUNCTION *******************************
 *
 * NAME:    TransposeAndApplyDisplayLUT
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: See History
 *
 * CALL:    TransposeAndApplyDisplayLUT()
 *
 * RETURNS: none
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 * 08/13/99 JMM   Applies LUT to 2 vertically neighboring pixels
 *                of the 16bit image, converts each to 4bps as type 
 *                uchar, shoves both pixels into 1 byte (with the 
 *                left pixel as the 4 upper bits and the right pixel 
 *                as the 4 lower bits), and finally transposes the 
 *                pixels into horizontal neighbors for display.
 *
 ****************************************************************/
void TransposeAndApplyDisplayLUT ()
{
   ushort usX;
   ushort usY;
   uchar ucTemp;
   uchar ucTemp2;

   for (usY = 0; usY < LCD_MAX_Y; usY++)
   {
      for (usX = 0; usX < LCD_MAX_X; usX += 2)
      {
         ucTemp = (uchar) (usDisplayLUT[Resampled.Array[usX][usY]] / DISPLAY_SCALE_FACTOR);
         ucTemp2 = (uchar) (usDisplayLUT[Resampled.Array[usX + 1][usY]] / DISPLAY_SCALE_FACTOR);
		 ucTemp <<= 4;
		 ucTemp += ucTemp2;
		 pLcd->ucData[usY][usX / 2] = ucTemp;
		 pLcd->ucData[usY][(usX + LCD_MAX_X) / 2] = 0;
      }
   }
   return;
}


RESAMPLED Temp;
/************************ FUNCTION *******************************
 *
 * NAME:    ResampleROIPixels
 *
 * AUTHOR:  Johanna M. Mitchell
 *
 * SUMMARY: See History
 *
 * CALL:    ResampleROIPixels()
 *
 * RETURNS: none
 *
 * HISTORY: 
 * 06/02/99 JMM   Created
 * 08/13/99 JMM   Resamples ROI coordinates and determines the 
 *				  appropriate graylevel for the LCD display image
 * 12/06/99 JMM	  Writes the remapped LCD-ROI pixel coordinates
 *				  and graylevels into a structure in ShMem 
 *
 ****************************************************************/
void ResampleROIPixels ()
{
   ushort f;
   ushort fCount;
	ushort usX;
	ushort usY;
   ushort n;
   ushort usCount;
   ushort usCountPrev;
   ushort usGraylevel;
   ushort usXResampled;
   ushort usYResampled;
   //RESAMPLED Temp;

   for (usX = 0; usX < LCD_MAX_X; usX++)
	{
	   for (usY = 0; usY < LCD_MAX_Y; usY++)
		{
		   Temp.Array[usX][usY] = INITIAL_LCDROI_VALUE;
		}
	}
   //memset(Temp.Array, INITIAL_LCDROI_VALUE, sizeof(Temp.Array)); 

   fCount = 0;
   for (f = 0; f < MAX_NUM_ACTUAL_FINGERS; f++)
   {
	  if (fCount >= ImageInfo.usNumFingers)
	  {
		 break;
	  }
	  else if ((pBoneInfo+f)->usFound != TRUE)
	  {
		 continue;
	  }	
     usCount = 0;
	  usCountPrev = 0;
	  usGraylevel = 0;
     for (n = 0; n < (pRoi+f)->usNumPoints; n++)
      {
         if (usCount % 3 == 0 && usCountPrev % 3 != 0)
         {
            usGraylevel = ROI_GRAYLEVEL - usGraylevel;
         }
         usXResampled = (ushort) (((long)(pRoi+f)->Coords[n].X * LCD_MAX_X + IMAGE_MAX_X/2) / IMAGE_MAX_X);
         usYResampled = (ushort) (((long)(pRoi+f)->Coords[n].Y * LCD_MAX_X + IMAGE_MAX_X/2) / IMAGE_MAX_X + DATA_MIN_Y);
		   usCountPrev = usCount;
		 
         //if (Temp.Array[usXResampled][usYResampled] != 0 && Temp.Array[usXResampled][usYResampled] != ROI_GRAYLEVEL)
		   if (Temp.Array[usXResampled][usYResampled] == INITIAL_LCDROI_VALUE)
         {
            //usCount++;
            //Resampled.Array[usXResampled][usYResampled] = usGraylevel;
			   Temp.Array[usXResampled][usYResampled] = usGraylevel;
			   //ROI coordinates must be transposed for display image
			   pLcdRoi[f].Coords[usCount].X = usYResampled;
			   pLcdRoi[f].Coords[usCount].Y = usXResampled;
			   pLcdRoi[f].usGraylevel[usCount] = usGraylevel / DISPLAY_SCALE_FACTOR;
				usCount++;
         }
      }
	   fCount++;
	   pLcdRoi[f].usNumPoints = usCount;
   }
   //WriteLCDROI(pLcdRoi);
   return;
}

