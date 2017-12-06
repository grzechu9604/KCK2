#pragma once
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <cmath>

using namespace cv;
using namespace std;

class DrawableKeyPoint
{
public:
	DrawableKeyPoint(KeyPoint k);
	~DrawableKeyPoint();
	const static int delta = 2;
	const static int minimalRate = 5;
	const static int maxRate = 10;
	bool IsTheSameKeypoint(KeyPoint k);
	bool IsDrawable();
	void RefreshRate(vector<KeyPoint> keyPoints);
	KeyPoint keyPoint;
	void IncrementRateBy(int value);
	int GetRate();
private:
	int rate;
};

