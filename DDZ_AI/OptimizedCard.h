#pragma once
#include<vector>
#include<set>
#include<string>
#include"CardSet.h"
#include "HandCards.h"
#include "DecorateDealStrategy.h"

class OptimizedCard :public CardSet
{
private:
	CardVector isolateCardIndex[3];
	uint8_t firstChainIndex[3];
	void SwapWithEmpty(int left, int right);
	CardVector  ExchangeValue(int playerIndex);
public:
	std::string ToString()override;
	OptimizedCard();
	void Optimized(DecorateDealStrategy * strategy, int preRandomCount=0);
	virtual ~OptimizedCard();
};

