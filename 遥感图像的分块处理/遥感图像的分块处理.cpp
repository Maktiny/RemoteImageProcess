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
   
	//14848 11264	
	//ͼ��·��
	char * srcpath_M = "./Image/Mul_large.tif";
	char * srcpath_P ="./Image/Pan_large.tif" ;
	char *dstpath_Fus = "./Image/American_Fus.tif";


	//ͼ��Ŀ�Ⱥ͸߶ȣ�ͨ����
	long int imgx, imgy, bandNum;

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
	
	cout<<imgx<<' '<<imgy<<endl;
	//���仺����

	bandR  = (float*)CPLMalloc(512*512*sizeof(float));
	bandG = (float*)CPLMalloc(512*512*sizeof(float));
	bandB  = (float*)CPLMalloc(512*512*sizeof(float));
	bandH  = (float*)CPLMalloc(512*512*sizeof(float));
	bandS  = (float*)CPLMalloc(512*512*sizeof(float));
	bandP  = (float*)CPLMalloc(512*512*sizeof(float));

	bandR  = (float*)CPLMalloc(imgx*255*sizeof(float));
	bandG = (float*)CPLMalloc(imgx*255*sizeof(float));
	bandB  = (float*)CPLMalloc(imgx*255*sizeof(float));
	bandH  = (float*)CPLMalloc(imgx*255*sizeof(float));
	bandS  = (float*)CPLMalloc(imgx*255*sizeof(float));
	bandP  = (float*)CPLMalloc(imgx*255*sizeof(float));



	//�������ͼ��
	 poDstDS = GetGDALDriverManager()->GetDriverByName("Gtiff")->Create(
		dstpath_Fus, imgx, imgy, bandNum, GDT_Byte,NULL);


	for(int k = 1; k <= imgy / 256 +1 ; k++)
	{
		
	  poSrcDS_M->GetRasterBand( 1)->RasterIO(GF_Read,
			0,255*(k -1)  ,imgx, 255,  bandR, imgx, 255  ,GDT_Float32 ,0 ,0 ) ;
	   poSrcDS_M->GetRasterBand( 2)->RasterIO(GF_Read,
			0,255*(k -1)   ,imgx, 255,  bandG, imgx, 255  ,GDT_Float32 ,0 ,0 ) ;
	   poSrcDS_M->GetRasterBand( 3)->RasterIO(GF_Read,
			0,255*(k -1)   ,imgx, 255 ,  bandB, imgx, 255  ,GDT_Float32 ,0 ,0 ) ;
	  poSrcDS_P->GetRasterBand( 1)->RasterIO(GF_Read,
			0, 255*(k -1)   ,imgx, 255 ,  bandP, imgx, 255  ,GDT_Float32 ,0 ,0 ) ;

	 for(int i = 0; i < imgx*255;i++)
	   {
		   //���� RGB ==> IHS ���任
		bandH[i] = -sqrt(2.0)/6 * bandR[i] + (-sqrt(2.0)/6 * bandG[i]) + sqrt(2.0)/3* bandB[i];
		bandS[i] = 1/sqrt(2.0) * bandR[i] + (-1/sqrt(2.0))*bandG[i];

		//���� IHS ==> RGB ����任
		
		bandR[i] = bandP[i] - 1/sqrt(2.0) *bandH[i] + 1/sqrt(2.0) *bandS[i];
		bandG[i] = bandP[i] - 1/sqrt(2.0) *bandH[i] - 1/sqrt(2.0) *bandS[i];
		bandB[i] = bandP[i] + sqrt(2.0) *bandH[i] ;
	   }
		 

	poDstDS->GetRasterBand(1)->RasterIO(GF_Write,
		0,255*(k -1)   ,imgx, 255 ,  bandR, imgx, 255 ,GDT_Float32 ,0 ,0 );
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write,
		0,255*(k -1)   ,imgx, 255 ,  bandG, imgx, 255 ,GDT_Float32 ,0 ,0 );
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write,
		0,255*(k -1)   ,imgx, 255 ,  bandB, imgx, 255  ,GDT_Float32 ,0 ,0 );
	cout<<imgx<<" "<<255*(k -1)<<endl; 
	}
	
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);

	GDALClose(poSrcDS_M);
	GDALClose(poSrcDS_P);
	GDALClose(poDstDS);
	system("Pause");
	return 0;
}

