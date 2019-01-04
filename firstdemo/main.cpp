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
	char* srcPath  = "daxiang.jpg";
	//输出图像路径
	char * dstPath = "two.tif";
	//图像的内存空间
	GByte* bufferTmp;
	int i, bandNum;

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

	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgx, imgy, bandNum, GDT_Byte,NULL);

	//分波段输出
	for( i  = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read,
			0,0, imgx, imgy ,bufferTmp, imgx, imgy, GDT_Byte, 0,0);

		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write,
			0,0, imgx, imgy ,bufferTmp, imgx, imgy, GDT_Byte, 0,0);
		cout<<"band  "<<i<<"   processing....."<<endl;
	}

	//清除缓存
	CPLFree(bufferTmp);
	//关闭输入输出流
	GDALClose(poSrcDS);
	GDALClose(poDstDS);

	system("pause");
	return 0;


}