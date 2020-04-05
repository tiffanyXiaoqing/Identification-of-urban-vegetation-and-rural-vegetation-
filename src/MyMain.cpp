﻿#include <cstdio>
#include <gdal_priv.h>
#include <cpl_conv.h>
#include <iostream>
#include <thread>
#include "UGUtility.h"
#include "UGDensityMap.h"
#include "UGProcessing.h"

int main() {

	// Register GDAL drivers

	
	GDALAllRegister();
	//CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	// Open classified image (cluster image)
	GDALDataset* src = (GDALDataset*)GDALOpen("F:\\UG\\dodo\\scc.img", GA_ReadOnly);
	if (src == NULL) {
		printf("Open file failed.");
	}

	// Print some basic information
	GDALDriver* srcDriver = src->GetDriver();
	printf("Driver     : %s/%s\n", srcDriver->GetDescription(), src->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
	printf("Size       : %d x %d x %d\n", src->GetRasterXSize(), src->GetRasterYSize(), src->GetRasterCount());
	if (src->GetProjectionRef() != NULL) {
		printf("Projection : %s\n", src->GetProjectionRef());
	}
	double adfGeoTransform[6];
	if (src->GetGeoTransform(adfGeoTransform) == CE_None) {
		printf("Origin     : (%.6f, %.6f)\n", adfGeoTransform[0], adfGeoTransform[3]);
		printf("Pixel Size : (%.6f, %.6f)\n", adfGeoTransform[1], adfGeoTransform[5]);
	}

	// Fetch the only layer (cluster information)
	GDALRasterBand* srcBand = src->GetRasterBand(1);
	int nBlockXSize, nBlockYSize;
	srcBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
	printf("Block = %d x %d  Type = %s, ColorInterp = %s\n", nBlockXSize, nBlockYSize, GDALGetDataTypeName(srcBand->GetRasterDataType()), GDALGetColorInterpretationName(srcBand->GetColorInterpretation()));
	double adfMinMax[2];
	int bGotMin, bGotMax;
	adfMinMax[0] = srcBand->GetMinimum(&bGotMin);
	adfMinMax[1] = srcBand->GetMaximum(&bGotMax);
	if (!(bGotMin && bGotMax)) {
		GDALComputeRasterMinMax((GDALRasterBandH)srcBand, TRUE, adfMinMax);
	}
	printf("Min = %.3f, Max = %.3f\n", adfMinMax[0], adfMinMax[1]);
	if (srcBand->GetOverviewCount() > 0) {
		printf("Band has %d overviews.\n", srcBand->GetOverviewCount());
	}
	if (srcBand->GetColorTable() != NULL) {
		printf("Band has a color table with %d entries.\n", srcBand->GetColorTable()->GetColorEntryCount());
	}

	// Reading data
	int nXSize = srcBand->GetXSize();
	int nYSize = srcBand->GetYSize();
	int* buffer = (int*)CPLMalloc(sizeof(int) * nXSize * nYSize);
	srcBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize, buffer, nXSize, nYSize, GDT_Int32, 0, 0);

	// Check reading first 8 rows/columns
	/*
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			printf("%d ", buffer[y * nXSize + x]);
		}
		printf("\n");
	}
	*/

	// Assigning Clusters to C++ Sets
	// HARDCODED MAGIC NUMBERS
	int shadowClassNums[] = { 1, 2 };
	int urbanClassNums[] = { 3, 8, 15 };
	int bareClassNums[] = { 4, 7, 13, 14 };
	int vegClassNums[] = { 5, 6, 9, 10, 11, 12 };
	std::set<int> shadowClassSet(shadowClassNums, shadowClassNums + 2);
	std::set<int> urbanClassSet(urbanClassNums, urbanClassNums + 3);
	std::set<int> bareClassSet(bareClassNums, bareClassNums + 4);
	std::set<int> vegClassSet(vegClassNums, vegClassNums + 6);

	// Temporary variables for calculating band statistics
	double min, max, mean, stddev;

	// Eliminate shadow
	printf("Eliminating shadow ... ");
	EliminateShadow(buffer, nXSize, nYSize, shadowClassSet, UG_NorthEast);
	printf("Done.\n");

	// Check the result of shadow eliminating
	/*
	GDALDataset* dst_1 = srcDriver->CreateCopy("E:\\Work\\FinalProjects\\RSDIP\\data\\Output\\shadowElimNE.img", src, FALSE, NULL, NULL, NULL );
	GDALRasterBand* dstBand_1 = dst_1->GetRasterBand(1);
	dstBand_1->RasterIO(GF_Write, 0, 0, nXSize, nYSize, buffer, nXSize, nYSize, GDT_Int32, 0, 0);
	dstBand_1->ComputeStatistics(FALSE, &min, &max, &mean, &stddev, NULL, NULL);
	GDALClose(dst_1);
	*/

	int* densityMap = new int[nXSize * nYSize];
	//get urban density map
	GDALDataset* urban_src = (GDALDataset*)GDALOpen("F:\\UG\\dodo\\urbanDensity.img", GA_ReadOnly);
	if (urban_src != NULL) {
		std::cout << "Urban density already exists." << std::endl;
	
	GDALRasterBand* urban_srcBand = src->GetRasterBand(1);
	urban_srcBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize, densityMap, nXSize, nYSize, GDT_Int32, 0, 0);
	GDALClose(urban_src);
	}
	else{
	// TODO: Calculate urban density map
	// HARDCODED MAGIC NUMBERS
	printf("Creating urban density map ... ");
	UGWindow window = GenerateDensityWindow(10);
	// PrintDensityWindow(window);
	CreateUrbanDensityMap(buffer, densityMap, nXSize, nYSize, urbanClassSet, window);
	std::cout << "\n" << "Done.\n" << std::endl;
	}

	// Check the urban density map
	GDALDataset* dst_2 = srcDriver->CreateCopy("F:\\UG\\dodo\\urbanDensity.img", src, FALSE, NULL, NULL, NULL);
	GDALRasterBand* dstBand_2 = dst_2->GetRasterBand(1);
	dstBand_2->SetColorInterpretation(GCI_GrayIndex);
	dstBand_2->RasterIO(GF_Write, 0, 0, nXSize, nYSize, densityMap, nXSize, nYSize, GDT_Int32, 0, 0);
	dstBand_2->SetColorTable(NULL);
	dstBand_2->ComputeStatistics(FALSE, &min, &max, &mean, &stddev, NULL, NULL);
	GDALClose(dst_2);

	// Classify Vegetation Area
	// HARDCODED MAGIC NUMBERS
	std::cout << "Begin processing vegetation ... " << std::endl;
	int dev_num = 2;
	int dev_size = nYSize / dev_num;
	std::vector<std::thread> t;
	for (int i = 0; i < dev_num; i++)  //前三个进程
	{
		std::cout << i << " thread begin" << std::endl;
		t.push_back(std::thread(ClassifyVegetation, buffer, densityMap, nXSize, nYSize, dev_size*i, dev_size*(i + 1), vegClassSet, 60, 8500, 16, 17));
	}
	
	//最后一个进程一位可能不能被整除，所以单独拿出来
	//t.push_back(std::thread(ClassifyVegetation, buffer, densityMap, nXSize, dev_size*(dev_num-1), nYSize, vegClassSet, 60, 8500, 16, 17));

	t[0].join(); std::cout << 0 << " thread end" << std::endl;
	t[1].join(); std::cout << 1 << " thread end" << std::endl;
	//t[2].join(); std::cout << 2 << " thread end" << std::endl;
	//t[3].join(); std::cout << 3 << " thread end" << std::endl;
	//t[4].join(); std::cout << 4 << " thread end" << std::endl;
	//t[5].join(); std::cout << 4 << " thread end" << std::endl;
	
	//for (int i = 0; i < dev_num - 1; i++) {
	//	t[i].join();
	//	std::cout << i << " thread end" << std::endl;
	//}
		
	// Get result
	GDALDataset* dst_3 = srcDriver->CreateCopy("F:\\UG\\dodo\\result.img", src, FALSE, NULL, NULL, NULL);
	GDALRasterBand* dstBand_3 = dst_3->GetRasterBand(1);
	GDALColorTable* colorTable_3 = dstBand_3->GetColorTable();
	dstBand_3->RasterIO(GF_Write, 0, 0, nXSize, nYSize, buffer, nXSize, nYSize, GDT_Int32, 0, 0);
	dstBand_3->ComputeStatistics(FALSE, &min, &max, &mean, &stddev, NULL, NULL);
	GDALClose(dst_3);

	// Freeing memory
	CPLFree(buffer);
	GDALClose(src);

	// Exiting program
	system("PAUSE");
	return 0;
}