/*******************************************************
* Name��       creat_bmp.c
* Function:   Creat a bmp picture as 320*240
* Input parameters:
* Output parameters:
* Author: ValerianFan
* Version : 1.0
* Date :  2012-1-16
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern"C"{
#include "generateBmp.h"
}
#include <crtdbg.h>
typedef unsigned char byte;
typedef unsigned short dbyte;
typedef unsigned long int dword;
typedef unsigned short word;


/*******************************************
*����bmp�ļ���ͷ�����ݽṹ
********************************************/
#pragma pack(push,2)  //����2�ֽڶ���


struct tagBITMAPFILEHEADER {
    //bmp file header
    dbyte bfType;        //�ļ�����
    dword bfSize;            //�ļ���С���ֽ�Ϊ��λ
    word bfReserved1;   //����������Ϊ0
    word bfReserved2;   //����������Ϊ0
    dword bfOffBits;         //���ļ�ͷ��ʼ��ƫ����


    //bmp info head
    dword  biSize;            //�ýṹ�Ĵ�С
    dword  biWidth;           //ͼ��Ŀ�ȣ�������Ϊ��λ
    dword  biHeight;          //ͼ��ĸ߶ȣ�������Ϊ��λ
    word biPlanes;          //ΪĿ���豸˵��λ��������ֵ������Ϊ1
    word biBitCount;        //˵��������/����
    dword biCompression;     //ͼ������ѹ������
    dword biSizeImage;       //ͼ���С�����ֽ�Ϊ��λ
    dword biXPelsPerMeter;   //ˮƽ�ֱ��ʣ�����/��
    dword biYPelsPerMeter;   //��ֱ�ֱ��ʣ�ͬ��
    dword biClrUsed;         //λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    dword biClrImportant;    //��ͼ����ʾ����ҪӰ�����ɫ��������Ŀ

    //bmp rgb quad
     //����16λ��24λ��32λ��λͼ����Ҫɫ�ʱ�
    //unsigned char rgbBlue;    //ָ����ɫǿ��
    //unsigned char rgbGreen;   //ָ����ɫǿ��
    //unsigned char rgbRed;     //ָ����ɫǿ��
    //unsigned char rgbReserved; //����������Ϊ0
}BMPFILEHEADER;
#pragma (pop)


struct tagBITMAPFILEHEADER *bmp_p;  //����bmp�ļ�ͷ�ṹ��ָ��
FILE *fd;    //����һ���ļ����͵�ָ��
FILE *fd2 ;
FILE *fd3 ;

/*************************************************************
*��ʼ��bmp�ļ�ͷ��������bmpͼƬ
**************************************************************/


void Init_bmp_head(void)
{
    bmp_p = &BMPFILEHEADER;
    bmp_p-> bfType = 0x4D42;    //�ļ�����
    bmp_p-> bfSize = 0x38436;   //�ļ���С���ֽ�Ϊ��λ
    bmp_p-> bfReserved1 = 0x0;   //����������Ϊ0
    bmp_p-> bfReserved2 = 0x0;   //����������Ϊ0
    bmp_p-> bfOffBits = 0x36;         //���ļ�ͷ��ʼ��ƫ����

    //bmp info head
    bmp_p-> biSize = 0x28;            //�ýṹ�Ĵ�С
    bmp_p-> biWidth = 320;           //ͼ��Ŀ�ȣ�������Ϊ��λ
    bmp_p-> biHeight = -240;          //ͼ��ĸ߶ȣ�������Ϊ��λ
    bmp_p-> biPlanes = 0x01;          //ΪĿ���豸˵��λ��������ֵ������Ϊ1
    bmp_p-> biBitCount = 24;        //˵��������/����
    bmp_p-> biCompression = 0;     //ͼ������ѹ������
    bmp_p-> biSizeImage = 0x38400;//0x09f8;       //ͼ���С�����ֽ�Ϊ��λ
    bmp_p-> biXPelsPerMeter = 0x60;//0x60;   //ˮƽ�ֱ��ʣ�����/��
    bmp_p-> biYPelsPerMeter = 0x60;   //��ֱ�ֱ��ʣ�ͬ��
    bmp_p-> biClrUsed = 0;         //λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    bmp_p-> biClrImportant = 0;    //��ͼ����ʾ����ҪӰ�����ɫ��������Ŀ

}
void myInvalidParameterHandler(const wchar_t* expression,
   const wchar_t* function,
   const wchar_t* file,
   unsigned int line,
   uintptr_t pReserved)
{
   wprintf(L"Invalid parameter detected in function %s."
            L" File: %s Line: %d\n", function, file, line);
   wprintf(L"Expression: %s\n", expression);
   abort();
}
int main(){
    char* formatString;

       _invalid_parameter_handler oldHandler, newHandler;
       newHandler = myInvalidParameterHandler;
       oldHandler = _set_invalid_parameter_handler(newHandler);
    generateFile();
    // Disable the message box for assertions.
      _CrtSetReportMode(_CRT_ASSERT, 0);

      // Call printf_s with invalid parameters.

      formatString = NULL;
      printf(formatString);
}

int createbmp(void)
{
    static char *file_name =NULL;    //�����ļ�����ָ��
    static char *file_name2 =NULL;
    static long file_length = 0x38436;  //�ļ��Ĵ�С�������ļ���
    unsigned char *file_p = NULL;       //д������ָ��
    unsigned char *file_p_tmp = NULL;    //д��������ʱָ��
    unsigned char *byte_copy_p = NULL;   //�ļ�ͷ������ָ��
    unsigned char byte_copy = 0;        //�ļ�ͷ�����ݿ�������
    int i = 0;int j = 0;int tmp=0;
    file_name = "test2.bmp";
    Init_bmp_head();
    file_p = (unsigned char *)malloc(sizeof(unsigned char)*76800*3+54);   //����һ���ڴ�
    file_p_tmp = file_p;
    fd2=fopen("lcd","rb");
    fd3=fopen("lcdroi","rb");
    unsigned char k;
    unsigned char *buffer=( unsigned char *)malloc(sizeof(unsigned char));
    unsigned short *buffer2=( unsigned short *)malloc(sizeof(unsigned short));
    byte_copy_p = (unsigned char *)bmp_p;
    file_p_tmp = file_p;
    for(i = 0;i < 54;i++)
    {
        *file_p_tmp = *byte_copy_p;
        file_p_tmp++;
        byte_copy_p++;
    }
    int r=0;
    for(j=0;j<240;j++)
    for(i = 0;i < 320;i++ )
    {

        fread(buffer,sizeof(unsigned char),1,fd2);

        //if(*buffer2!=0) printf("%d  ",*buffer2);
        k=*buffer;
        k=(unsigned char)255-k;
        //if(k==255){
        //	k=0;
        //}
        //if(i<100){
        //printf("%d",*buffer2);
        //}
        if(i<160){
            for(tmp=0;tmp<6;tmp++){
                if(j<28) *file_p_tmp = 0;
                else *file_p_tmp = k;
       file_p_tmp++;
       }
       }
   }
    //unsigned short max=*buffer2;
    short *x=(  short *)malloc(sizeof(short));
    short *y=( short *)malloc(sizeof(short));
    int f=0;
    for(f=0;f<3;f++){
    fread(buffer2,sizeof(unsigned short),1,fd3);
   // printf("Block%d:",f);
    for(r=0;r<640;r++)
      {
    fread(y,sizeof(short),1,fd3);
    fread(x,sizeof(short),1,fd3);
    printf("%d,%d ",*x,*y);
    if(*x==0&&*y==0) continue;
    if(*x>=320||*y>=240) continue;
    if(*x<0||*y<0) continue;
    file_p_tmp=file_p+54;
    for(tmp=0;tmp<6;tmp++){
    *(file_p_tmp+*y*320*3+*x*3)=255;
    file_p_tmp++;
    }
    }
    //printf("\n\n");
    for(r=0;r<640;r++) fread(buffer2,sizeof(unsigned short),1,fd3);
fd = fopen(file_name, "w+");
   fwrite(file_p, file_length, 1,fd);
fclose(fd);                           //�ͷ�������ڴ棨��Ҫ��

}

free(file_p);

  // printf("Done success!!!\n");
  // getchar();
   return (0);
}
