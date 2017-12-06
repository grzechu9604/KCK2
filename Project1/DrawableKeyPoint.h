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
	DrawableKeyPoint(Vec3f k);
	~DrawableKeyPoint();
	const static int delta = 7;
	const static int minimalRate = 6;
	const static int maxRate = 15;
	bool IsTheSameKeypoint(Vec3f k);
	bool IsDrawable();
	void RefreshRate(vector<Vec3f> keyPoints);
	Vec3f keyPoint;
	void IncrementRateBy(int value);
	int GetRate();
private:
	int rate;
};

