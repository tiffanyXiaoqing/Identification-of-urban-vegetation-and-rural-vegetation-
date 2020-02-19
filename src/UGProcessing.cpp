#include "UGUtility.h"
#include "UGDensityMap.h"
#include "UGFloodfill.h"
#include "UGProcessing.h"

void EliminateShadow(int* buffer, int width, int height, std::set<int> shadowClassSet, UGAspect aspect = UG_North) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (shadowClassSet.find(buffer[y * width + x]) != shadowClassSet.end()) {
				int x1 = x, y1 = y;
				int classNum;
				switch (aspect) {

				case UG_North:

					// Scan toward north-east, until meets with a non-shadow pixel
					// If scan out of image border, do nothing
					do {
						y1--;
						if (y1 < 0) break;
					} while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end());

					// If scan out of image border, do nothing
					if (y1 < 0) break;

					// Get the class number of the pixel that is found by scanning
					classNum = buffer[y1 * width + x1];

					// Scan back and assign class number to shadow pixels
					y1++;
					while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end()) {
						buffer[y1 * width + x1] = classNum;
						y1++;
						if (y1 >= height) break;
					}
					break;

				case UG_NorthWest:
					do {
						x1--;
						y1--;
						if (x1 < 0 || y1 < 0) break;
					} while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end());
					if (x1 < 0 || y1 < 0) break;
					classNum = buffer[y1 * width + x1];
					x1++;
					y1++;
					while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end()) {
						buffer[y1 * width + x1] = classNum;
						x1++;
						y1++;
						if (x1 >= width || y1 >= height) break;
					}
					break;
				case UG_West:
					do {
						x1--;
						if (x1 < 0) break;
					} while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end());
					if (x1 < 0) break;
					classNum = buffer[y1 * width + x1];
					x1++;
					while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end()) {
						buffer[y1 * width + x1] = classNum;
						x1++;
						if (x1 >= width) break;
					}
					break;
				case UG_SouthWest:
					do {
						x1--;
						y1++;
						if (x1 < 0 || y1 >= height) break;
					} while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end());
					if (x1 < 0 || y1 >= height) break;
					classNum = buffer[y1 * width + x1];
					x1++;
					y1--;
					while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end()) {
						buffer[y1 * width + x1] = classNum;
						x1++;
						y1--;
						if (x1 >= width || y1 < 0) break;
					}
					break;
				case UG_South:
					do {
						y1++;
						if (y1 >= height) break;
					} while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end());
					if (y1 >= height) break;
					classNum = buffer[y1 * width + x1];
					y1--;
					while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end()) {
						buffer[y1 * width + x1] = classNum;
						y1--;
						if (y1 < 0) break;
					}
					break;
				case UG_SouthEast:
					do {
						x1++;
						y1++;
						if (x1 >= width || y1 >= height) break;
					} while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end());
					if (x1 >= width || y1 >= height) break;
					classNum = buffer[y1 * width + x1];
					x1--;
					y1--;
					while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end()) {
						buffer[y1 * width + x1] = classNum;
						x1--;
						y1--;
						if (x1 < 0 || y1 < 0) break;
					}
					break;
				case UG_East:
					do {
						x1++;
						if (x1 >= width) break;
					} while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end());
					if (x1 >= width) break;
					classNum = buffer[y1 * width + x1];
					x1--;
					while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end()) {
						buffer[y1 * width + x1] = classNum;
						x1--;
						if (x1 < 0) break;
					}
					break;
				case UG_NorthEast:
					do {
						x1++;
						y1--;
						if (x1 >= width || y1 < 0) break;
					} while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end());
					if (x1 >= width || y1 < 0) break;
					classNum = buffer[y1 * width + x1];
					x1--;
					y1++;
					while (shadowClassSet.find(buffer[y1 * width + x1]) != shadowClassSet.end()) {
						buffer[y1 * width + x1] = classNum;
						x1--;
						y1++;
						if (x1 < 0 || y1 >= height) break;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void CreateUrbanDensityMap(const int* inputBuffer, int* urbanDensity, int width, int height, std::set<int> urbanClassSet, UGWindow window) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			urbanDensity[y * width + x] = 0;
		}
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (urbanClassSet.find(inputBuffer[y * width + x]) != urbanClassSet.end()) {
				for (int y1 = y - window.radius; y1 <= y + window.radius; y1++) {
					if (y1 < 0 || y1 >= height) continue;
					for (int x1 = x - window.radius; x1 <= x + window.radius; x1++) {
						if (x1 < 0 || x1 >= width) continue;
						urbanDensity[y1 * width + x1]++;
					}
				}
			}
		}
	}
}

void ClassifyVegetation(int* buffer, const int* urbanDensity, int width, int height, std::set<int> vegClassSet, int urbanVegThres, int maxFloodCount, int urbanVegClassNum, int ruralVegClassNum) {
	QueueScanlineFloodFill floodfiller(width, height);
	int* bufferCopy = new int[width * height];
	int count = 0;
	for (int y = 0; y < height; y++) {
		printf("Scanning Line %d\n", y);
		for (int x = 0; x < width; x++) {
			int cell = y * width + x;

			// Iterating each vegetation pixel
			if (vegClassSet.find(buffer[cell]) != vegClassSet.end()) {

				// If the pixel is located at high value area in UDM (means it is more likely to be in urban area)
				if (urbanDensity[cell] >= urbanVegThres) {

					// Make a copy of current grid to try floodfilling
					CopyGrid(buffer, bufferCopy, width, height);

					// If the pixel is in an open area (floodfilled too many pixels, exceeding the threshold)
					// Then abandon the copy, and floodfill this area as rural vegetation
					if (floodfiller.FloodFill(bufferCopy, x, y, vegClassSet, urbanVegClassNum, maxFloodCount) == FLOODFILL_EXCEED_MAX) {
						floodfiller.FloodFill(buffer, x, y, vegClassSet, ruralVegClassNum, -1);
					}

					// Or floodfill this area as urban vegetation
					else {
						CopyGrid(bufferCopy, buffer, width, height);
					}

					// Count and print the number of area floodfilled
					count++;
					printf("Filled %d areas.\n", count);

				}

				// If the pixel is located at low value area in UDM
				else {

					// Make a copy of current grid to try floodfilling
					CopyGrid(buffer, bufferCopy, width, height);

					// If the pixel is in an open area (floodfilled too many pixels, exceeding the threshold)
					// Then floodfill this area as rural vegetation
					if (floodfiller.FloodFill(bufferCopy, x, y, vegClassSet, ruralVegClassNum, maxFloodCount) == FLOODFILL_EXCEED_MAX) {
						floodfiller.FloodFill(buffer, x, y, vegClassSet, ruralVegClassNum, -1);
						count++;
						printf("Filled %d areas.\n", count);
					}

					// Else we do nothing
					// For we cannot discriminate that if it is just surrounded by bare soils,
					// or it is just located at the low value area in UDM but still in urban vegetation (e.g. the center of a lawn)

				}

			}

		}
	}
	delete bufferCopy;
}
