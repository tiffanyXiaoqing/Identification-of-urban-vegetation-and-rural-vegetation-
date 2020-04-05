#include "UGUtility.h"

void emptyGrid(int* grid, int width, int height) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int cell = y * width + x;
			grid[cell] = 0;
		}
	}
}

void CopyGrid(const int* input, int* copy, int width, int height) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int cell = y * width + x;
			copy[cell] = input[cell];
		}
	}
}