#pragma once
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
//#include <vector>
#include "DrawableKeyPoint.h"
using namespace std;
using namespace cv;
class ArrayOfDrawablePoints
{
public:
	ArrayOfDrawablePoints();
	~ArrayOfDrawablePoints();
	void RefreshArray(vector<KeyPoint> t);
	vector<KeyPoint> GetDrawablePoints();
	DrawableKeyPoint * GetMatchingKeyPoint(KeyPoint k);
private:
	vector<DrawableKeyPoint> array;
	static const int incrementer = 2;
	static const int decrementer = -1;

};

