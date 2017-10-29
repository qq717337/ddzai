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
	std::vector<uint8_t>isolateCardIndex[3];
	uint8_t firstChainIndex[3];
	void SwapWithEmpty(int left, int right);
	std::vector<uint8_t> ExchangeValue(int playerIndex);
public:
	std::string ToString()override;
	OptimizedCard();
	void Optimized(DecorateDealStrategy * strategy, int preRandomCount);
	virtual ~OptimizedCard();
};

