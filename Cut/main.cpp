#include "StdAfx.h"
#include<iostream>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//图像输入
	GDALDataset*  poSrcDS;
	GDALDataset*  poSrcDSE;
	//图像输出
	GDALDataset* poDstDS;
   //图像的高度，宽度
	int imgx,imgy;
	//输入图像路径
	char* srcPath  = "./Image/superman.jpg";
	char* srcPathE  = "./Image/earth.jpg";
	//输出图像路径
	char * dstPath = "./Image/thregreg.jpg";
	//图像的内存空间
	GByte* bufferTmp;
	GByte* bufferTmpMRed;
	GByte* bufferTmpMGreen;
	GByte* bufferTmpMBlue;

	GByte* bufferTmpERed;
	GByte* bufferTmpEGreen;
	GByte* bufferTmpEBlue;

	int i, j, bandNum;

	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS = (GDALDataset *)GDALOpenShared(srcPath,GA_ReadOnly);
	poSrcDSE = (GDALDataset *)GDALOpenShared(srcPathE,GA_ReadOnly);

	//获取图像的高度，宽度，波段数
	imgx = poSrcDS->GetRasterXSize();
	imgy = poSrcDS->GetRasterYSize();
    bandNum = poSrcDS->GetRasterCount();

	//输出获取结果
	cout<<"Image x width:"<<imgx<<endl;
	cout<<"Image y height:"<<imgy<<endl;
	cout<<"Band Number:"<<bandNum<<endl;

	//分配内存
	bufferTmp = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpMRed = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpMGreen = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpMBlue = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));

    bufferTmpERed = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpEGreen = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpEBlue = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));

	//读取红色通道缓存在bufferTmp中（红色通道编号1，绿2，蓝3）
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, 
		0, 0, imgx, imgy, bufferTmpMRed, imgx, imgy, GDT_Byte, 0, 0);
	poSrcDSE->GetRasterBand(1)->RasterIO(GF_Read, 
		0, 0, imgx, imgy, bufferTmpERed, imgx, imgy, GDT_Byte, 0, 0);

	poSrcDS->GetRasterBand(2)->RasterIO(GF_Read, 
		0, 0, imgx, imgy, bufferTmpMGreen, imgx, imgy, GDT_Byte, 0, 0);
	poSrcDSE->GetRasterBand(2)->RasterIO(GF_Read, 
		0, 0, imgx, imgy, bufferTmpEGreen, imgx, imgy, GDT_Byte, 0, 0);

	poSrcDS->GetRasterBand(3)->RasterIO(GF_Read, 
		0, 0, imgx, imgy, bufferTmpMBlue, imgx, imgy, GDT_Byte, 0, 0);
	poSrcDSE->GetRasterBand(3)->RasterIO(GF_Read, 
		0, 0, imgx, imgy, bufferTmpEBlue, imgx, imgy, GDT_Byte, 0, 0);

	//创建输出图像
	poDstDS= GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgx, imgy, bandNum, GDT_Byte,NULL);

	//遍历缓冲区，把截取的图片像素设置为255
	for(i = 0; i < imgy ; i++)
	{
		for(j = 0; j < imgx; j++)
		{
			if(GByte(bufferTmpMRed[i * imgx + j ] )  > 10  && GByte(bufferTmpMRed[i * imgx + j ] )  < 160
				&& GByte(bufferTmpMGreen[i * imgx + j ] )  > 100 && GByte(bufferTmpMGreen[i * imgx + j ] )  < 220  
				&& GByte(bufferTmpMBlue[i * imgx + j ] )  > 10 && GByte(bufferTmpMBlue[i * imgx + j ] )  < 110)
			{
				       bufferTmpMGreen[i * imgx + j ]  = bufferTmpEGreen[i * imgx + j ];
				       bufferTmpMRed[i * imgx + j ]  = bufferTmpERed[i * imgx + j ];
				       bufferTmpMBlue[i * imgx + j ]  =  bufferTmpEBlue[i * imgx + j ] ;
		    }
		}
	}

	//分波段输出
for( i  = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read,
			0,0, imgx, imgy ,bufferTmp, imgx, imgy, GDT_Byte, 0,0);

		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write,
			0,0, imgx, imgy ,bufferTmp, imgx, imgy, GDT_Byte, 0,0);
		cout<<"band  "<<i<<"   processing....."<<endl;
	}

	//修改后管道1的数据写入.tif图片中,
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 
		0, 0, imgx, imgy, bufferTmpMRed, imgx, imgy, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 
		0, 0, imgx, imgy, bufferTmpMGreen, imgx, imgy, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 
		0, 0, imgx, imgy, bufferTmpMBlue, imgx, imgy, GDT_Byte, 0, 0);

	//清除缓存
	CPLFree(bufferTmp);
	CPLFree(bufferTmpMRed);
	CPLFree(bufferTmpMGreen);
	CPLFree(bufferTmpMBlue);

	CPLFree(bufferTmpERed);
	CPLFree(bufferTmpEGreen);
	CPLFree(bufferTmpEBlue);

	//关闭输入输出流
	GDALClose(poSrcDS);
	GDALClose(poSrcDSE);
	GDALClose(poDstDS);

	system("pause");
	return 0;


}