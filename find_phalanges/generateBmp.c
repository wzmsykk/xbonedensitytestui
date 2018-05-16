#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "segment.h"
extern IMAGE*	pImage;
//extern LCD*		pLcd;


int generateFile(){
	int			i, j;
	char		pcImFilename[1024];
	HISTOGRAM	*pShHistImage;
	MIDLINE		*pShMidline;
	BONEINFO	*pShBoneInfo;
	ROI			*pShROI;
	LCD			*pShLCD, *lcd_original;
	LCDROI		*pShLCDROI;
	
	pImage	=	(IMAGE*)malloc(sizeof(IMAGE));
	memset(pImage, 0, sizeof(IMAGE));

	memset(pcImFilename, 0, 1024);

	pShHistImage	=	(HISTOGRAM*)malloc(sizeof(HISTOGRAM));
	memset(pShHistImage, 0, sizeof(HISTOGRAM));

	pShMidline	=	(MIDLINE*)malloc(sizeof(MIDLINE));
	memset(pShMidline, 0, sizeof(MIDLINE));

	pShBoneInfo		=	(BONEINFO*)malloc(sizeof(BONEINFO));
	memset(pShBoneInfo, 0, sizeof(BONEINFO));

	pShROI	=	(ROI*)malloc(sizeof(ROI));
	memset(pShROI, 0, sizeof(ROI));

	pShLCD	=	(LCD*)malloc(sizeof(LCD));
	memset(pShLCD, 0, sizeof(LCD));

	lcd_original	=	(LCD*)malloc(sizeof(LCD));
	memset(lcd_original, 0, sizeof(LCD));

	pShLCDROI	=	(LCDROI*)malloc(sizeof(LCDROI)*3);
	memset(pShLCDROI, 0, sizeof(LCDROI)*3);

	FILE *fd;    //定义一个文件类型的指针
    FILE *fd2 ; 
    FILE *fd3;
    fd=fopen("input_data","rb");
    //fread(pImage, sizeof(IMAGE),1,fd);
    //printf("%d\n",sizeof(IMAGE));
    char* b = (char* ) malloc(sizeof(char)*9);
    fread(b,sizeof(char),9,fd);
    for(int i=0;i<536;i++)
    	for(int j=0;j<334;j++){
	fread(&pImage->Array[i][j],sizeof(ushort),1,fd);
} 
    
	FindPhalanges(pcImFilename, pShHistImage, pShMidline, pShBoneInfo, pShROI, pShLCD, pShLCDROI);
	
 
	static char *file_name =NULL;    //保存文件名的指针
    static char *file_name2 =NULL;
    static long file_length =sizeof(LCD);
    static long file_length2 =sizeof(LCDROI);
    LCD *file_p = NULL;       //写入数据指针
    //unsigned char *file_p_tmp = NULL;    //写入数据临时指针
    unsigned char *tmp = NULL;
    //file_p = (LCD *)malloc(sizeof(LCD));
    //file_p_tmp = file_p;
    file_name = "lcd";
    file_name2 = "lcdroi";
    
    //*file_p=*pShLCD;
	fd = fopen(file_name, "w");
    fwrite(pShLCD, sizeof(LCD), 1,fd);
    free(file_p);                       
    fclose(fd);
    printf("LCD success!!!\n");
    
    //LCDROI *tmplcdroi=mall
    LCDROI *file_p2 = NULL;
	file_p2 = (LCDROI *)malloc(sizeof(LCDROI)*3);
	*file_p2=*pShLCDROI;
    fd = fopen(file_name2, "wb+");
    fwrite(pShLCDROI, file_length2*3, 1,fd);
    free(file_p2);                       
    fclose(fd);
    printf("LCDROI success!!!\n");
    getchar();
    
	return 0;
}
