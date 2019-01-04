// IHSң��ͼ���ں�.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")
#include<iostream>
#include<math.h>

using namespace std;

int main()
{
	//ͼ���������
	GDALDataset * poSrcDS_M;
	GDALDataset* poSrcDS_P;
	GDALDataset * poDstDS;

	//ͼ��·��
	char * srcpath_M = "./Image/American_MUL.bmp";
	char * srcpath_P ="./Image/American_PAN.bmp" ;
	char *dstpath_Fus = "./Image/American_Fus.tiff";

	//ͼ��Ŀ�Ⱥ͸߶ȣ�ͨ����
	int imgx, imgy, bandNum;

	float* bandR;
	float*bandG;
	float*bandB;
	float*bandI;
	float*bandH;
	float*bandS;
	float*bandP;
	
	//ע������
	GDALAllRegister();

	//��ͼ��
	poSrcDS_M  = (GDALDataset*)GDALOpenShared(srcpath_M,GA_ReadOnly);
	poSrcDS_P = (GDALDataset*)GDALOpenShared(srcpath_P,GA_ReadOnly);

	//��ȡͼ���Ⱥ͸߶ȣ�ͨ��
	imgx= poSrcDS_M->GetRasterXSize();
	imgy= poSrcDS_M->GetRasterYSize();
	bandNum = poSrcDS_M->GetRasterCount();
	

	//���仺����
	bandR  = (float*)CPLMalloc(imgx*imgy*sizeof(float));
	bandG = (float*)CPLMalloc(imgx*imgy*sizeof(float));
	bandB  = (float*)CPLMalloc(imgx*imgy*sizeof(float));
	bandH  = (float*)CPLMalloc(imgx*imgy*sizeof(float));
	bandS  = (float*)CPLMalloc(imgx*imgy*sizeof(float));
	bandP  = (float*)CPLMalloc(imgx*imgy*sizeof(float));


	poSrcDS_M->GetRasterBand( 1)->RasterIO(GF_Read,
			0, 0, imgx, imgy,  bandR, imgx ,imgy ,GDT_Float32 ,0 ,0 ) ;
	poSrcDS_M->GetRasterBand( 2)->RasterIO(GF_Read,
			0, 0, imgx, imgy,  bandG, imgx ,imgy ,GDT_Float32 ,0 ,0 ) ;
	poSrcDS_M->GetRasterBand( 3)->RasterIO(GF_Read,
			0, 0, imgx, imgy,  bandB, imgx ,imgy ,GDT_Float32 ,0 ,0 ) ;
	poSrcDS_P->GetRasterBand( 1)->RasterIO(GF_Read,
			0, 0, imgx, imgy,  bandP, imgx ,imgy ,GDT_Float32 ,0 ,0 ) ;
	//�������ͼ��
	poDstDS = GetGDALDriverManager()->GetDriverByName("Gtiff")->Create(
		dstpath_Fus, imgx,imgy, bandNum, GDT_Byte,NULL);

	for(int i = 0; i < imgx*imgy;i++)
	{
		//���� RGB ==> IHS ���任
		bandH[i] = -sqrt(2.0)/6 * bandR[i] + (-sqrt(2.0)/6 * bandG[i]) + sqrt(2.0)/3* bandB[i];
		bandS[i] = 1/sqrt(2.0) * bandR[i] + (-1/sqrt(2.0))*bandG[i];

		//���� IHS ==> RGB ����任
		
		bandR[i] = bandP[i] - 1/sqrt(2.0) *bandH[i] + 1/sqrt(2.0) *bandS[i];
		bandG[i] = bandP[i] - 1/sqrt(2.0) *bandH[i] - 1/sqrt(2.0) *bandS[i];
		bandB[i] = bandP[i] + sqrt(2.0) *bandH[i] ;

	}
	//���޸ĺ������д��
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write,
		0, 0, imgx, imgy,  bandR, imgx ,imgy ,GDT_Float32 ,0 ,0 );
	//GDT_Float32Ϊ������bandG������
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write,
		0, 0, imgx, imgy,  bandG, imgx ,imgy ,GDT_Float32 ,0 ,0 );
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write,
		0, 0, imgx, imgy,  bandB, imgx ,imgy ,GDT_Float32 ,0 ,0 );


	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);

	GDALClose(poSrcDS_M);
	GDALClose(poSrcDS_P);
	GDALClose(poDstDS);
	return 0;
}

