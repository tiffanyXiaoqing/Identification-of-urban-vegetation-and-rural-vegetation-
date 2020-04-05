#pragma once

#include <queue>
#include <set>

typedef struct _UGFloodFillRange {
	int left, right, y;
} UGFloodFillRange;

#define LINEFILL_SUCCESS			0
#define LINEFILL_EXCEED_MAX			1
#define FLOODFILL_SUCCESS			0
#define FLOODFILL_EXCEED_MAX		2

class QueueScanlineFloodFill {
public:
public:

	std::queue<UGFloodFillRange> rangeQueue;
	int* visited;
	int width, height;
	int floodCount;

	QueueScanlineFloodFill(int _width, int _height);
	int FloodFill(int* buffer, int x, int y, int y_start, int y_end, std::set<int> vegClassSet, int vegClassNum, int FloodCount);
	int LineFill(int* buffer, int x, int y, std::set<int> vegClassSet, int vegClassNum, int maxFloodCount);
	bool inArea(int* buffer, int x, int y, int y_start, int y_end);

};