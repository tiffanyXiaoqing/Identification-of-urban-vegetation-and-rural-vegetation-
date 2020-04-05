#include "UGUtility.h"
#include "UGFloodfill.h"

QueueScanlineFloodFill::QueueScanlineFloodFill(int _width, int _height) {
	width = _width;
	height = _height;
	visited = new int[width * height];
	floodCount = 0;
}

bool QueueScanlineFloodFill::inArea(int* buffer, int x, int y, int y_start, int y_end) {
	if (y < y_start) return false;
	if (y > y_end) return false;
	return true;
}

int QueueScanlineFloodFill::FloodFill(int* buffer, int x, int y, int y_start, int y_end, std::set<int> vegClassSet, int vegClassNum, int floodCount = 0) {

	floodCount++;
	if (!inArea(buffer, x, y, y_start, y_end) )
	{
		return --floodCount;
	}

	int lFillLoc = x;
	int cell = y * width + lFillLoc;
	if(vegClassSet.find(buffer[cell]) != vegClassSet.end())
		return --floodCount;
	if (buffer[cell] == -1) return --floodCount;

	buffer[cell] = -1;   	//回溯算法，cell位置打上标记

	floodCount = FloodFill(buffer, x, y + 1, y_start, y_end, vegClassSet, vegClassNum, floodCount);
	floodCount = FloodFill(buffer, x, y - 1, y_start, y_end, vegClassSet, vegClassNum, floodCount);
	floodCount = FloodFill(buffer, x + 1, y, y_start, y_end, vegClassSet, vegClassNum, floodCount);
	floodCount = FloodFill(buffer, x - 1, y, y_start, y_end, vegClassSet, vegClassNum, floodCount);
	
	buffer[cell] = vegClassNum;       //将标记替换为vegClassNum

	return floodCount;
}