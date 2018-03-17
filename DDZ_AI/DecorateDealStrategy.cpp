#include "stdafx.h"
#include "DecorateDealStrategy.h"
#include<algorithm>
DecorateDealStrategy::DecorateDealStrategy(CardSet * card) :_Card(card)
{
}

DecorateDealStrategy::~DecorateDealStrategy()
{
}

void MultiBoomDealStrategy::PreDeal()
{
	int64_t choiceStartPlayer = CommonRandom.NextInt(0, 3);
	_Card->Update();
	auto booms = _Card->DeskCardSet.AvailableBoom();
	extern std::default_random_engine DefaultRandomEngine;
	std::shuffle(booms.begin(), booms.end(), DefaultRandomEngine);
	auto iter = booms.begin();
	for (int i = 0; i < BoomCount; ++i) {
		if (*iter >= CardIndex_JokerBoom) {
			_Card->DealJokerBoom(choiceStartPlayer % 3);
		}
		else {
			_Card->DealIndex(choiceStartPlayer % 3, *iter, 4);
		}
		++choiceStartPlayer;
		++iter;
	}
}

MultiBoomDealStrategy::MultiBoomDealStrategy(CardSet * card, int boomCount) :DecorateDealStrategy(card), BoomCount(boomCount) {

}

MultiBoomDealStrategy::~MultiBoomDealStrategy()
{
}

void SuperiorDealStrategy::PreDeal()
{
	int64_t choiceStartPlayer = CommonRandom.NextInt(0, 5);
	switch (choiceStartPlayer)
	{
	case 0:
		_Card->DealIndex(PlayerId, CardIndex_A, 2);
		break;
	case 1:
		_Card->DealIndex(PlayerId, CardIndex_2, 2);
		break;
	case 2:
		_Card->DealIndex(PlayerId, CardIndex_SmallJoker, 1);
		break;
	case 3:
		_Card->DealIndex(PlayerId, CardIndex_LargeJoker, 1);
		break;
	case 4:
		int64_t boomIndex = CommonRandom.NextInt(CardIndex_3, CardIndex_2);
		_Card->DealIndex(PlayerId, boomIndex, 4);
		break;
	}
}

SuperiorDealStrategy::SuperiorDealStrategy(CardSet * card, int playerId) :DecorateDealStrategy(card), PlayerId(playerId) {

}

SuperiorDealStrategy::~SuperiorDealStrategy()
{
}
