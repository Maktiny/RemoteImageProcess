#include "StdAfx.h"
#include<iostream>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//图像输入
	GDALDataset*  poSrcDS;
	//图像输出
	GDALDataset* poDstDS;
   //图像的高度，宽度
	int imgx,imgy;
	//输入图像路径
	char* srcPath  = "./Image/qingdao.jpg";
	//输出图像路径
	char * dstPath = "./Image/q.tif";
	//图像的内存空间
	GByte* bufferTmp;
	GByte* bufferTmp1;

	GByte* bufferTmp_1red;
	GByte* bufferTmp_1green;
	GByte* bufferTmp_1blue;

	GByte* bufferTmp_2red;
	GByte* bufferTmp_2green;
	GByte*bufferTmp_2blue;

	int i, j, bandNum;

	//读取图片片段的起始坐标
	int StartX = 100;
	int StartY = 100;

	int StartX1= 300;
	int StartY1 = 300;

	int StartX2= 500;
	int StartY2= 500;

	//需要截取处理的高度和宽度
	int tmpXlen = 200;
	int tmpYlen = 150;

	int tmpXlen1 = 100;
	int tmpYlen1 = 50;

	int tmpXlen2 = 50;
	int tmpYlen2 = 100;

	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS = (GDALDataset *)GDALOpenShared(srcPath,GA_ReadOnly);

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
	bufferTmp1= (GByte*)CPLMalloc(tmpXlen *tmpYlen *sizeof(GByte));

	bufferTmp_1red= (GByte*)CPLMalloc(tmpXlen1 *tmpYlen1 *sizeof(GByte));
	bufferTmp_1green = (GByte*)CPLMalloc(tmpXlen1 *tmpYlen1*sizeof(GByte));
	bufferTmp_1blue = (GByte*)CPLMalloc(tmpXlen1 *tmpYlen1*sizeof(GByte));

	bufferTmp_2red= (GByte*)CPLMalloc(tmpXlen2 *tmpYlen2 *sizeof(GByte));
	bufferTmp_2green = (GByte*)CPLMalloc(tmpXlen2 *tmpYlen2*sizeof(GByte));
	bufferTmp_2blue = (GByte*)CPLMalloc(tmpXlen2*tmpYlen2*sizeof(GByte));

	//读取在bufferTmp中的红色通道缓存（红色通道编号1，绿2，蓝3）
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, 
		StartX, StartY, tmpXlen, tmpYlen, bufferTmp1, tmpXlen, tmpYlen, GDT_Byte, 0, 0);


	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, 
		StartX1, StartY1, tmpXlen1, tmpYlen1, bufferTmp_1red, tmpXlen1, tmpYlen1, GDT_Byte, 0, 0);
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, 
		StartX2, StartY2, tmpXlen2, tmpYlen2, bufferTmp_2red, tmpXlen2, tmpYlen2, GDT_Byte, 0, 0);

	poSrcDS->GetRasterBand(2)->RasterIO(GF_Read, 
		StartX1, StartY1, tmpXlen1, tmpYlen1, bufferTmp_1green, tmpXlen1, tmpYlen1, GDT_Byte, 0, 0);
	poSrcDS->GetRasterBand(2)->RasterIO(GF_Read, 
		StartX2, StartY2, tmpXlen2, tmpYlen2, bufferTmp_2green, tmpXlen2, tmpYlen2, GDT_Byte, 0, 0);

	poSrcDS->GetRasterBand(3)->RasterIO(GF_Read, 
		StartX1, StartY1, tmpXlen1, tmpYlen1, bufferTmp_1blue, tmpXlen1, tmpYlen1, GDT_Byte, 0, 0);
	poSrcDS->GetRasterBand(3)->RasterIO(GF_Read, 
		StartX2, StartY2, tmpXlen2, tmpYlen2, bufferTmp_2blue, tmpXlen2, tmpYlen2, GDT_Byte, 0, 0);
	
	

	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgx, imgy, bandNum, GDT_Byte,NULL);

	//遍历缓冲区，把截取的图片像素设置为255
	for(i = 0; i < tmpYlen ; i++)
	{
		for(j = 0; j < tmpXlen; j++)
		{
			 bufferTmp1[i * tmpXlen + j ] = (GByte)255;
		}
	}

	//遍历缓冲区，把截取的图片像素设置为255
	for(i = 0; i < tmpYlen1 ; i++)
	{
		for(j = 0; j < tmpXlen1; j++)
		{
			 bufferTmp_1red[i * tmpXlen1 + j ] = (GByte)255;
			 bufferTmp_1green[i * tmpXlen1 + j ] = (GByte)255;
			 bufferTmp_1blue[i * tmpXlen1 + j ] = (GByte)255;
		}
	}

	for(i = 0; i < tmpYlen2 ; i++)
	{
		for(j = 0; j < tmpXlen2; j++)
		{
			 bufferTmp_2red[i * tmpXlen2 + j ] = (GByte)0;
			 bufferTmp_2green[i * tmpXlen2 + j ] = (GByte)0;
			 bufferTmp_2blue[i * tmpXlen2 + j ] = (GByte)0;
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
		StartX, StartY, tmpXlen, tmpYlen, bufferTmp1, tmpXlen, tmpYlen, GDT_Byte, 0, 0);


	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 
		StartX1, StartY1, tmpXlen1, tmpYlen1, bufferTmp_1red, tmpXlen1, tmpYlen1, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 
		StartX2, StartY2, tmpXlen2, tmpYlen2, bufferTmp_2red, tmpXlen2, tmpYlen2, GDT_Byte, 0, 0);

	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 
		StartX1, StartY1, tmpXlen1, tmpYlen1, bufferTmp_1green, tmpXlen1, tmpYlen1, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 
		StartX2, StartY2, tmpXlen2, tmpYlen2, bufferTmp_2green, tmpXlen2, tmpYlen2, GDT_Byte, 0, 0);

	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 
		StartX1, StartY1, tmpXlen1, tmpYlen1, bufferTmp_1blue, tmpXlen1, tmpYlen1, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 
		StartX2, StartY2, tmpXlen2, tmpYlen2, bufferTmp_2blue, tmpXlen2, tmpYlen2, GDT_Byte, 0, 0);

	//清除缓存
	CPLFree(bufferTmp);
	CPLFree(bufferTmp1);
	CPLFree(bufferTmp_1red);
	CPLFree(bufferTmp_1green);
	CPLFree(bufferTmp_1blue);
	CPLFree(bufferTmp_2red);
	CPLFree(bufferTmp_2green);
	CPLFree(bufferTmp_2blue);
	//关闭输入输出流
	GDALClose(poSrcDS);
	GDALClose(poDstDS);

	system("pause");
	return 0;


}