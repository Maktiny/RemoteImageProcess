#include "StdAfx.h"
#include<iostream>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//ͼ������
	GDALDataset*  poSrcDS;
	//ͼ�����
	GDALDataset* poDstDS;
   //ͼ��ĸ߶ȣ����
	int imgx,imgy;
	//����ͼ��·��
	char* srcPath  = "./Image/qingdao.jpg";
	//���ͼ��·��
	char * dstPath = "./Image/q.tif";
	//ͼ����ڴ�ռ�
	GByte* bufferTmp;
	GByte* bufferTmp1;

	GByte* bufferTmp_1red;
	GByte* bufferTmp_1green;
	GByte* bufferTmp_1blue;

	GByte* bufferTmp_2red;
	GByte* bufferTmp_2green;
	GByte*bufferTmp_2blue;

	int i, j, bandNum;

	//��ȡͼƬƬ�ε���ʼ����
	int StartX = 100;
	int StartY = 100;

	int StartX1= 300;
	int StartY1 = 300;

	int StartX2= 500;
	int StartY2= 500;

	//��Ҫ��ȡ����ĸ߶ȺͿ��
	int tmpXlen = 200;
	int tmpYlen = 150;

	int tmpXlen1 = 100;
	int tmpYlen1 = 50;

	int tmpXlen2 = 50;
	int tmpYlen2 = 100;

	//ע������
	GDALAllRegister();

	//��ͼ��
	poSrcDS = (GDALDataset *)GDALOpenShared(srcPath,GA_ReadOnly);

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
	bufferTmp1= (GByte*)CPLMalloc(tmpXlen *tmpYlen *sizeof(GByte));

	bufferTmp_1red= (GByte*)CPLMalloc(tmpXlen1 *tmpYlen1 *sizeof(GByte));
	bufferTmp_1green = (GByte*)CPLMalloc(tmpXlen1 *tmpYlen1*sizeof(GByte));
	bufferTmp_1blue = (GByte*)CPLMalloc(tmpXlen1 *tmpYlen1*sizeof(GByte));

	bufferTmp_2red= (GByte*)CPLMalloc(tmpXlen2 *tmpYlen2 *sizeof(GByte));
	bufferTmp_2green = (GByte*)CPLMalloc(tmpXlen2 *tmpYlen2*sizeof(GByte));
	bufferTmp_2blue = (GByte*)CPLMalloc(tmpXlen2*tmpYlen2*sizeof(GByte));

	//��ȡ��bufferTmp�еĺ�ɫͨ�����棨��ɫͨ�����1����2����3��
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
	
	

	//�������ͼ��
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgx, imgy, bandNum, GDT_Byte,NULL);

	//�������������ѽ�ȡ��ͼƬ��������Ϊ255
	for(i = 0; i < tmpYlen ; i++)
	{
		for(j = 0; j < tmpXlen; j++)
		{
			 bufferTmp1[i * tmpXlen + j ] = (GByte)255;
		}
	}

	//�������������ѽ�ȡ��ͼƬ��������Ϊ255
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

	//�������
	CPLFree(bufferTmp);
	CPLFree(bufferTmp1);
	CPLFree(bufferTmp_1red);
	CPLFree(bufferTmp_1green);
	CPLFree(bufferTmp_1blue);
	CPLFree(bufferTmp_2red);
	CPLFree(bufferTmp_2green);
	CPLFree(bufferTmp_2blue);
	//�ر����������
	GDALClose(poSrcDS);
	GDALClose(poDstDS);

	system("pause");
	return 0;


}