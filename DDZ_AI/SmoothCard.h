#pragma once
#include "DecorateDealStrategy.h"
#include "MinStepSplitStrategy.h"
class SmoothCard :
	public CardSet
{
public:
	SmoothCard();
	void Optimized(DecorateDealStrategy * strategy, int firstEpch, int secondEpch = 0);
	void Process(int swapCount);
	void ExchangeSet(MinStepSplitStrategy* splitStrategy, HandCards* addSet, HandCards* removeSet, int swapCount);
	void ProcessDesk();
	~SmoothCard();
};

