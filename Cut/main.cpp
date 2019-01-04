#include "StdAfx.h"
#include<iostream>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//ͼ������
	GDALDataset*  poSrcDS;
	GDALDataset*  poSrcDSE;
	//ͼ�����
	GDALDataset* poDstDS;
   //ͼ��ĸ߶ȣ����
	int imgx,imgy;
	//����ͼ��·��
	char* srcPath  = "./Image/superman.jpg";
	char* srcPathE  = "./Image/earth.jpg";
	//���ͼ��·��
	char * dstPath = "./Image/thregreg.jpg";
	//ͼ����ڴ�ռ�
	GByte* bufferTmp;
	GByte* bufferTmpMRed;
	GByte* bufferTmpMGreen;
	GByte* bufferTmpMBlue;

	GByte* bufferTmpERed;
	GByte* bufferTmpEGreen;
	GByte* bufferTmpEBlue;

	int i, j, bandNum;

	//ע������
	GDALAllRegister();

	//��ͼ��
	poSrcDS = (GDALDataset *)GDALOpenShared(srcPath,GA_ReadOnly);
	poSrcDSE = (GDALDataset *)GDALOpenShared(srcPathE,GA_ReadOnly);

	//��ȡͼ��ĸ߶ȣ���ȣ�������
	imgx = poSrcDS->GetRasterXSize();
	imgy = poSrcDS->GetRasterYSize();
    bandNum = poSrcDS->GetRasterCount();

	//�����ȡ���
	cout<<"Image x width:"<<imgx<<endl;
	cout<<"Image y height:"<<imgy<<endl;
	cout<<"Band Number:"<<bandNum<<endl;

	//�����ڴ�
	bufferTmp = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpMRed = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpMGreen = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpMBlue = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));

    bufferTmpERed = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpEGreen = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));
	bufferTmpEBlue = (GByte*)CPLMalloc(imgx*imgy*sizeof(GByte));

	//��ȡ��ɫͨ��������bufferTmp�У���ɫͨ�����1����2����3��
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

	//�������ͼ��
	poDstDS= GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgx, imgy, bandNum, GDT_Byte,NULL);

	//�������������ѽ�ȡ��ͼƬ��������Ϊ255
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

	//�ֲ������
for( i  = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read,
			0,0, imgx, imgy ,bufferTmp, imgx, imgy, GDT_Byte, 0,0);

		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write,
			0,0, imgx, imgy ,bufferTmp, imgx, imgy, GDT_Byte, 0,0);
		cout<<"band  "<<i<<"   processing....."<<endl;
	}

	//�޸ĺ�ܵ�1������д��.tifͼƬ��,
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 
		0, 0, imgx, imgy, bufferTmpMRed, imgx, imgy, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 
		0, 0, imgx, imgy, bufferTmpMGreen, imgx, imgy, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 
		0, 0, imgx, imgy, bufferTmpMBlue, imgx, imgy, GDT_Byte, 0, 0);

	//�������
	CPLFree(bufferTmp);
	CPLFree(bufferTmpMRed);
	CPLFree(bufferTmpMGreen);
	CPLFree(bufferTmpMBlue);

	CPLFree(bufferTmpERed);
	CPLFree(bufferTmpEGreen);
	CPLFree(bufferTmpEBlue);

	//�ر����������
	GDALClose(poSrcDS);
	GDALClose(poSrcDSE);
	GDALClose(poDstDS);

	system("pause");
	return 0;


}