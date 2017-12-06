#include "DrawableKeyPoint.h"



DrawableKeyPoint::DrawableKeyPoint(KeyPoint k)
{
	this->keyPoint = k;
	this->rate = 0;
}


DrawableKeyPoint::~DrawableKeyPoint()
{
}

bool DrawableKeyPoint::IsTheSameKeypoint(KeyPoint k)
{
	return abs(k.pt.x - this->keyPoint.pt.x) <= delta && abs(k.pt.y - this->keyPoint.pt.y) <= delta;
}

bool DrawableKeyPoint::IsDrawable()
{
	return this->rate > this->minimalRate;
}

void DrawableKeyPoint::RefreshRate(vector<KeyPoint> keyPoints)
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

