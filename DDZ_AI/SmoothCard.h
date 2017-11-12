#pragma once
#include "CardSet.h"
#include "DecorateDealStrategy.h"
class SmoothCard :
	public CardSet
{
public:
	SmoothCard(); 
	void Optimized(DecorateDealStrategy * strategy, int firstEpch, int secondEpch = 0);
	void Process( int swapCount);
	~SmoothCard();
};

