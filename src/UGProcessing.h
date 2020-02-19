#pragma once

#include <set>

typedef enum _UGAspect {
	UG_North, UG_NorthWest, UG_West, UG_SouthWest, UG_South, UG_SouthEast, UG_East, UG_NorthEast
} UGAspect;

void EliminateShadow(int* buffer, int width, int height, std::set<int> shadowClassNum, UGAspect aspect);
void CreateUrbanDensityMap(const int* inputBuffer, int* urbanDensity, int width, int height, std::set<int> urbanClassNum, UGWindow window);
void ClassifyVegetation(int* buffer, const int* urbanDensity, int width, int height, std::set<int> vegClassSet, int urbanVegThres, int maxFloodCount, int urbanVegClassNum, int ruralVegClassNum);