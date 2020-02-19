#include <cstdio>

#include "UGDensityMap.h"

UGWindow GenerateDensityWindow(int radius) {
	int width, height;
	width = height = 2 * radius + 1;
	int assign;
	int* arr = new int[(2 * radius + 1) * (2 * radius + 1)];
	for (int y = 0; y <= radius; y++) {
		for (int x = 0; x <= radius; x++) {
			assign = (radius - x) * (radius - x) + (radius - y) * (radius - y) <= radius * radius ? 1 : 0;
			arr[y * width + x] = assign;
			arr[y * width + (width - x - 1)] = assign;
			arr[(height - y - 1) * width + x] = assign;
			arr[(height - y - 1) * width + (width - x - 1)] = assign;
		}
	}
	UGWindow window = {arr, radius};
	return window;
}

void PrintDensityWindow(UGWindow window) {
	int width, height;
	width = height = 2 * window.radius + 1;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("%d ", window.arr[y * width + x]);
		}
		printf("\n");
	}
}