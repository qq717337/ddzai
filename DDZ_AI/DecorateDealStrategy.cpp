#include "stdafx.h"
#include "DecorateDealStrategy.h"
#include<algorithm>
DecorateDealStrategy::DecorateDealStrategy(CardSet * card):_Card(card)
{
}

DecorateDealStrategy::~DecorateDealStrategy()
{
}

void MultiBoomDealStrategy::PreDeal()
{
	int64_t choiceStartPlayer = CommonRandom.NextInt(0, 3);
	auto booms = _Card->DeskCardSet.AvailableBoom();
	std::random_shuffle(booms.begin(), booms.end());
	auto iter = booms.begin();
	for (int i = 0; i < BoomCount; ++i) {
		_Card->DealIndex(choiceStartPlayer % 3, *iter,4);
		++choiceStartPlayer;
		++iter;
	}
}

MultiBoomDealStrategy::MultiBoomDealStrategy(CardSet * card,int boomCount) :DecorateDealStrategy(card),BoomCount(boomCount) {

}

MultiBoomDealStrategy::~MultiBoomDealStrategy()
{
}
