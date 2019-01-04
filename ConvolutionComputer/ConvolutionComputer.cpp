// ConvolutionComputer.cpp : 定义控制台应用程序的入口点。
//
#include "StdAfx.h"

#include<iostream>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

#define BOXFILTER              1
#define MOVEFILTER           2
#define EDGEFILTER            3
#define SHARPNESSFILTER  4
#define EMBOSSINGFILTER  5
#define GAUSSFILTER           6

int ImgFilter(char * srcPath ,char * dstPath, int flag);

//均值模糊 Box filter
int BoxFilter(float *ImagIn, float *ImagOut, int imgx, int imgy);

//运动模糊
int MoveFilter(float *ImagIn, float *ImagOut, int imgx, int imgy);

//边缘检测
int EdgeFilter(float *ImagIn, float *ImagOut, int imgx, int imgy);

//图像锐化 Sharpness Filter
int SharpnessFilter(float *ImagIn, float *ImagOut, int imgx, int imgy);

// 浮雕 Embossing filter
int EmbossingFilter(float *ImagIn, float *ImagOut, int imgx, int imgy);

//高斯模糊Gaussian
int GaussFilter(float* imagIn, float* imagOut, int imgx, int imgy);

int main()
{
		//路径
	 char * srcPath = "./Image/lena.jpg";

	 //注册
	GDALAllRegister();

	 ImgFilter(srcPath ,"./Image/BOXFILTER.tif", BOXFILTER );

	  ImgFilter(srcPath ,"./Image/ MOVEFILTER.tif", MOVEFILTER );

	  ImgFilter(srcPath ,"./Image/EDGEFILTER.tif", EDGEFILTER   );

	  ImgFilter(srcPath ,"./Image/SHARPNESSFILTER.tif", SHARPNESSFILTER );

	 ImgFilter(srcPath ,"./Image/EMBOSSINGFILTER.tif", EMBOSSINGFILTER );

	 ImgFilter(srcPath ,"./Image/GAUSSFILTER.tif", GAUSSFILTER  );

	 return 0;

}

int ImgFilter(char * srcPath ,char * dstPath, int flag)
{
	//图像输入输出
	GDALDataset* poSrcDS;
	GDALDataset* poDstDS;

	//缓存
	float *ImagIn;
	float *ImagOut;
	int i , j,  bandNum;
    int imgx,imgy;

	

	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);


	imgx = poSrcDS->GetRasterXSize();
	imgy = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//分配内存
	ImagIn =  (float*)CPLMalloc(imgx*imgy*sizeof(float));
	ImagOut =  (float*)CPLMalloc(imgx*imgy*sizeof(float));

	//创建输出图像流
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgx,imgy,bandNum,GDT_Byte,NULL);
	
	
	//输出
	for(i = 0; i < bandNum; i++)
	{
	    poSrcDS->GetRasterBand( i + 1)->RasterIO(GF_Read,
			0, 0, imgx, imgy,  ImagIn, imgx ,imgy ,GDT_Float32 ,0 ,0 ) ;
		switch(flag)
		{
			 case  BOXFILTER : BoxFilter(ImagIn, ImagOut,  imgx,  imgy);
				 break;
			 case MOVEFILTER: MoveFilter(ImagIn, ImagOut,  imgx,  imgy);
				 break;
			 case EDGEFILTER : EdgeFilter(ImagIn, ImagOut,  imgx,  imgy);
				 break;
		     case SHARPNESSFILTER : SharpnessFilter(ImagIn, ImagOut,  imgx,  imgy);
				 break;
		    case EMBOSSINGFILTER:  EmbossingFilter(ImagIn, ImagOut,  imgx,  imgy);
		 		 break;
		     case GAUSSFILTER: GaussFilter(ImagIn, ImagOut,  imgx,  imgy);
				 break;
		}
		poDstDS->GetRasterBand( i + 1)->RasterIO(GF_Write,
			0, 0, imgx ,  imgy , ImagOut ,imgx ,imgy ,GDT_Float32 ,0 ,0 );
	} 

	//清除缓存
	CPLFree(ImagIn);
	CPLFree(ImagOut);
	

	//释放输入输出流指针
	GDALClose(poSrcDS);
	GDALClose(poDstDS);
	return(0);
}

//均值模糊 Box filter
int BoxFilter(float *ImagIn, float *ImagOut, int imgx, int imgy)
{
	for(int j = 1; j < imgy -1; j++)
	{
		for(int i= 1; i < imgx - 1; i++)
		{
			ImagOut[j * imgx + i] = (ImagIn[(j - 1) *imgx  + i ] * 0.2 +
				                                    ImagIn[(j + 1) *imgx  + i ] * 0.2 +
													ImagIn[j*imgx  + i +1] * 0.2 +
													ImagIn[j*imgx  + i ] * 0.2 +
													ImagIn[j *imgx  + i -1] * 0.2 ) / 5.0f + 64;
				                                
		}
	}
	return 0;
}

//运动模糊
int MoveFilter(float *ImagIn, float *ImagOut, int imgx, int imgy)
{
	for(int j = 2; j < imgy -2; j++)
	{
		for(int i= 2; i < imgx - 2; i++)
		{
			ImagOut[j * imgx + i] = (ImagIn[(j - 2) *imgx  + i - 2] * 0.2 +
				                                    ImagIn[(j - 1 ) *imgx  + i -1 ] * 0.2 +
													ImagIn[j*imgx  + i ] * 0.2 +
													ImagIn[(j +1 ) *imgx  + i + 1] * 0.2 +
				                                    ImagIn[(j + 2) *imgx  + i + 2 ] * 0.2  ) / 5.0f + 64;
		}
	}
	return 0;
}
//边缘检测
int EdgeFilter(float *ImagIn, float *ImagOut, int imgx, int imgy)
{
	for(int j = 1; j < imgy -1; j++)
	{
		for(int i= 1; i < imgx - 1; i++)
		{
			ImagOut[j * imgx + i] = -ImagIn[(j - 1) *imgx  + i - 1] +
				                                   -ImagIn[(j - 1 ) *imgx  + i  ]  +
												   -ImagIn[(j - 1 ) *imgx  + i +1 ]  +
												   -ImagIn[j*imgx  + i - 1] +
				                                   ImagIn[j *imgx  + i  ] * 8 +
												    -ImagIn[j *imgx  + i +1 ]  +
													-ImagIn[(j + 1) *imgx  + i - 1] +
				                                   -ImagIn[(j + 1 ) *imgx  + i  ]  +
												    -ImagIn[(j + 1 ) *imgx  + i +1 ]  ;
		}
	}
	return 0;
}

//图像锐化 Sharpness Filter
int SharpnessFilter(float *ImagIn, float *ImagOut, int imgx, int imgy)
{
	for(int j = 1; j < imgy -1; j++)
	{
		for(int i= 1; i < imgx - 1; i++)
		{
			ImagOut[j * imgx + i] = -ImagIn[(j - 1) *imgx  + i - 1] +
				                                   -ImagIn[(j - 1 ) *imgx  + i  ]  +
												   -ImagIn[(j - 1 ) *imgx  + i +1 ]  +
												   -ImagIn[j*imgx  + i - 1] +
				                                   ImagIn[j *imgx  + i  ] * 9+
												    -ImagIn[j *imgx  + i +1 ]  +
													-ImagIn[(j + 1) *imgx  + i - 1] +
				                                   -ImagIn[(j + 1 ) *imgx  + i  ]  +
												    -ImagIn[(j + 1 ) *imgx  + i +1 ]  ;
		}
	}
	return 0;
}

// 浮雕 Embossing filter
int EmbossingFilter(float *ImagIn, float *ImagOut, int imgx, int imgy)
{
	for(int j = 1; j < imgy -1; j++)
	{
		for(int i= 1; i < imgx - 1; i++)
		{
			ImagOut[j * imgx + i] = -ImagIn[(j - 1) *imgx  + i - 1] +
				                                   -ImagIn[(j - 1 ) *imgx  + i  ]  +
												   -ImagIn[j*imgx  + i - 1] +
												    ImagIn[j *imgx  + i +1 ]  +
				                                    ImagIn[(j + 1 ) *imgx  + i  ]  +
												    ImagIn[(j + 1 ) *imgx  + i +1 ] +128 ;
		}
	}
	return 0;
}

//高斯模糊Gaussian
int GaussFilter(float* imagIn, float* imagOut, int imgx, int imgy)
{
    // | 0.0120  0.1253  0.2736  0.1253  0.0120 |
    // | 0.1253  1.3054  2.8514  1.3054  0.1253 |
    // | 0.2736  2.8514  6.2279  2.8514  0.2736 |
    // | 0.1253  1.3054  2.8514  1.3054  0.1253 |
    // | 0.0120  0.1253  0.2736  0.1253  0.0120 |

     int i, j;
    for (j=2; j<imgy-2; j++)
    {
        for (i=2; i<imgx-2; i++)
        {
            imagOut[j*imgx+i] = (0.0120*imagIn[(j-2)*imgx+i-2] +
            	                   0.1253*imagIn[(j-2)*imgx+i-1] +
            	                   0.2736*imagIn[(j-2)*imgx+i] +
                                   0.1253*imagIn[(j-2)*imgx+i+1] +
                                   0.0120*imagIn[(j-2)*imgx+i+2] +
                                   0.1253*imagIn[(j-1)*imgx+i-2] +
                                   1.3054*imagIn[(j-1)*imgx+i-1] +
                                   2.8514*imagIn[(j-1)*imgx+i] +
                                   1.3054*imagIn[(j-1)*imgx+i+1] +
                                   0.1253*imagIn[(j-1)*imgx+i+2] +
                                   0.2763*imagIn[j*imgx+i-2] +
                                   2.8514*imagIn[j*imgx+i-1] +
                                   6.2279*imagIn[j*imgx+i] +
                                   2.8514*imagIn[j*imgx+i+1] +
                                   0.2763*imagIn[j*imgx+i+2] +
                                   0.1253*imagIn[(j+1)*imgx+i-2] +
                                   1.3054*imagIn[(j+1)*imgx+i-1] +
                                   2.8514*imagIn[(j+1)*imgx+i] +
                                   1.3054*imagIn[(j+1)*imgx+i+1] +
                                   0.1253*imagIn[(j+1)*imgx+i+2] +
                                   0.0120*imagIn[(j+2)*imgx+i-2] +
                                   0.1253*imagIn[(j+2)*imgx+i-1] +
                                   0.2736*imagIn[(j+2)*imgx+i] +
                                   0.1253*imagIn[(j+2)*imgx+i+1] +
                                   0.0120*imagIn[(j+2)*imgx+i+2]) / 25.0f;
        }
    }
    return 0;
}