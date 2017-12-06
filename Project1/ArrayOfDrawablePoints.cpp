#include "ArrayOfDrawablePoints.h"



ArrayOfDrawablePoints::ArrayOfDrawablePoints()
{
}


ArrayOfDrawablePoints::~ArrayOfDrawablePoints()
{
}

void ArrayOfDrawablePoints::RefreshArray(vector<KeyPoint> t)
{
	for (int tCounter = 0; tCounter < t.size(); tCounter++)
	{
		DrawableKeyPoint * k = this->GetMatchingKeyPoint(t[tCounter]);
		if (k == nullptr)
		{
			k = new DrawableKeyPoint(t[tCounter]);
			this->array.push_back(*k);
		}
		else
			k->IncrementRateBy(this->incrementer);
	}
	for (int i = 0; i < this->array.size(); i++)
	{
		this->array[i].IncrementRateBy(this->decrementer);
		
		if (this->array[i].GetRate() < 0)
		{
			this->array.erase(array.begin() + i);
			i--;
		}		
	}
}

vector<KeyPoint> ArrayOfDrawablePoints::GetDrawablePoints()
{
	vector<KeyPoint> result;
	for (int i = 0; i < this->array.size(); i++)
	{
		if (this->array[i].IsDrawable())
			result.push_back(this->array[i].keyPoint);
	}
	return result;
}

DrawableKeyPoint * ArrayOfDrawablePoints::GetMatchingKeyPoint(KeyPoint k)
{
	for (int i = 0; i < this->array.size(); i++)
	{
		if (this->array[i].IsTheSameKeypoint(k))
			return &this->array[i];
	}
	return nullptr;
}
