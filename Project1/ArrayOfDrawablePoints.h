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
	void RefreshArray(vector<Vec3f> t);
	vector<Vec3f> GetDrawablePoints();
	DrawableKeyPoint * GetMatchingKeyPoint(Vec3f k);
private:
	vector<DrawableKeyPoint> array;
	static const int incrementer = 3;
	static const int decrementer = -1;

};

