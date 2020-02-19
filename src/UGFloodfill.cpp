#include "UGUtility.h"
#include "UGFloodfill.h"

QueueScanlineFloodFill::QueueScanlineFloodFill(int _width, int _height) {
	width = _width;
	height = _height;
	pixelClassified = new int[width * height];
	floodCount = 0;
}

int QueueScanlineFloodFill::LineFill(int* buffer, int x, int y, std::set<int> vegClassSet, int vegClassNum, int maxFloodCount) {
	int lFillLoc = x;
	int cell = y * width + lFillLoc;
	while (lFillLoc >= 0 && !pixelClassified[cell] && vegClassSet.find(buffer[cell]) != vegClassSet.end()) {
		buffer[cell] = vegClassNum;
		pixelClassified[cell] = 1;
		lFillLoc--;
		cell--;
		floodCount++;
		if (maxFloodCount > 0) {
			if (floodCount > maxFloodCount) {
				return LINEFILL_EXCEED_MAX;
			}
		}
	}
	lFillLoc++;
	int rFillLoc = x + 1;
	cell = y * width + rFillLoc;
	while (rFillLoc < width && !pixelClassified[cell] && vegClassSet.find(buffer[cell]) != vegClassSet.end()) {
		buffer[cell] = vegClassNum;
		pixelClassified[cell] = 1;
		rFillLoc++;
		cell++;
		floodCount++;
		if (maxFloodCount > 0) {
			if (floodCount > maxFloodCount) {
				return LINEFILL_EXCEED_MAX;
			}
		}
	}
	rFillLoc--;
	UGFloodFillRange r = {lFillLoc, rFillLoc, y};
	rangeQueue.push(r);
	return LINEFILL_SUCCESS;
}

int QueueScanlineFloodFill::FloodFill(int* buffer, int x, int y, std::set<int> vegClassSet, int vegClassNum, int maxFloodCount) {
	emptyGrid(pixelClassified, width, height);
	floodCount = 0;
	if (LineFill(buffer, x, y, vegClassSet, vegClassNum, maxFloodCount) == LINEFILL_EXCEED_MAX) {
		return FLOODFILL_EXCEED_MAX;
	}
	while (!rangeQueue.empty()) {
		UGFloodFillRange range = rangeQueue.front();
		rangeQueue.pop();
		for (int x1 = range.left; x1 <= range.right; x1++) {
			int y1 = range.y - 1;
			int above = y1 * width + x1;
			if (y1 >= 0 && (!pixelClassified[above]) && vegClassSet.find(buffer[above]) != vegClassSet.end()) {
				if (LineFill(buffer, x1, y1, vegClassSet, vegClassNum, maxFloodCount) == LINEFILL_EXCEED_MAX) {
					return FLOODFILL_EXCEED_MAX;
				}
			}
			int y2 = range.y + 1;
			int below = y2 * width + x1;
			if (y2 < height && (!pixelClassified[below]) && vegClassSet.find(buffer[below]) != vegClassSet.end()) {
				if (LineFill(buffer, x1, y2, vegClassSet, vegClassNum, maxFloodCount) == LINEFILL_EXCEED_MAX) {
					return FLOODFILL_EXCEED_MAX;
				}
			}
		}
	}
	return FLOODFILL_SUCCESS;
}