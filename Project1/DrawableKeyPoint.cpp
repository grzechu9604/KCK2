#include "DrawableKeyPoint.h"



DrawableKeyPoint::DrawableKeyPoint(Vec3f k)
{
	this->keyPoint = k;
	this->rate = 3;
}


DrawableKeyPoint::~DrawableKeyPoint()
{
}

bool DrawableKeyPoint::IsTheSameKeypoint(Vec3f k)
{
	return abs(k[0] - this->keyPoint[0]) <= delta && abs(k[1] - this->keyPoint[1]) <= delta;
}

bool DrawableKeyPoint::IsDrawable()
{
	return this->rate > this->minimalRate;
}

void DrawableKeyPoint::RefreshRate(vector<Vec3f> keyPoints)
{
	for (int i = 0; i < keyPoints.size(); i++)
	{
		if (this->IsTheSameKeypoint(keyPoints[i])) 
		{
			this->rate++;
			return;
		}
	}
	this->rate--;
}

void DrawableKeyPoint::IncrementRateBy(int value)
{
	this->rate += value;
	if (this->rate < 0)
		this->rate = 0;
	else if (this->rate > maxRate)
		this->rate = maxRate;
}

int DrawableKeyPoint::GetRate()
{
	return this->rate;
}

