#pragma once

typedef struct _UGWindow {
	int* arr;
	int radius;
} UGWindow;

UGWindow GenerateDensityWindow(int radius);
void PrintDensityWindow(UGWindow window);