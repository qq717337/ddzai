#include "stdafx.h"
#include "OptimizedCard.h"
#include <algorithm>
void OptimizedCard::SwapWithEmpty(int left, int right)
{
	auto& leftdata = PlayerCardSet[left]->Data();
	auto& rightdata = PlayerCardSet[right]->Data();
	if (isolateCardIndex[left].size() > 1 && isolateCardIndex[right].size() == 0) {
		auto  swapOtherIndex = ExchangeValue(right);
		if (swapOtherIndex.size() > 0) {
			uint8_t otherSwapIndex = swapOtherIndex[CommonRandom.NextInt(0, swapOtherIndex.size())];
			uint8_t	choice = isolateCardIndex[left][CommonRandom.NextInt(0, isolateCardIndex[left].size())];
			auto iterFinder = std::find_if(leftdata.begin(), leftdata.end(), [choice](uint8_t x) {return (x  & 0xf )== (choice+3); });
			//auto rightFinder = std::find(rightdata.begin(),rightdata.end(), otherSwapIndex);
			//std::swap(iterFinder, rightFinder);
			if (iterFinder != leftdata.end()) {
			uint8_t value = (*iterFinder);
			leftdata.erase(iterFinder);
			leftdata.insert(otherSwapIndex);
			rightdata.insert(value);
			rightdata.erase(otherSwapIndex);
			}
		}
	}
}

std::vector<uint8_t> OptimizedCard::ExchangeValue(int playerIndex)
{
	std::vector<uint8_t>r;
	auto &data = PlayerCardSet[playerIndex]->Data();
	auto& count = PlayerCardSet[playerIndex]->Count();
	for (const auto & v : data) {
		uint8_t	cardValue = v & 0x0f;
		if (cardValue < 3) {
			continue;
		}
		uint8_t  valueCount = count[cardValue - 3];
		if (valueCount == 0 || valueCount == 4) {
			continue;
		}
		if (firstChainIndex[playerIndex] == 0 || firstChainIndex[playerIndex] > 7) {
			if (cardValue > 7) {
				r.push_back(v);
				continue;
			}
		}
		else {
			if (cardValue > firstChainIndex[playerIndex] && cardValue < firstChainIndex[playerIndex] + 5) { //卡的值不在那个之间
				r.push_back(v);
				continue;
			}
			else {
				if (cardValue > firstChainIndex[playerIndex] + 5) {
					r.push_back(v);
					continue;
				}
			}
		}
	}
	return r;
}

std::string OptimizedCard::ToString()
{
	return CardSet::ToString();
}

OptimizedCard::OptimizedCard()
{
	
}

void OptimizedCard::Optimized(DecorateDealStrategy * strategy, int preRandomCount)
{
	ResetPlayerCard(preRandomCount);
	if (strategy != NULL)
		strategy->PreDeal();
	RandomFillLeft();

	size_t isolateCardCount = 0;
	for (int i = 0; i < PlayerCardSet.size(); ++i) {
		auto isolate = PlayerCardSet[i]->IsolateCards();
		auto singleChain = PlayerCardSet[i]->AvailableSingleChain();
		firstChainIndex[i] = -1;
		if (singleChain.size() > 0) {
			firstChainIndex[i] = singleChain[0];
		}
		auto str = PlayerCardSet[i]->ToString();
		for (int index = CardIndex_3; index <= CardIndex_7; ++index) {
			int sum = isolate[index][0] + isolate[index][1] + isolate[index][2] + isolate[index][3];
			if (sum == 1 && index < firstChainIndex[i]) {
				isolateCardIndex[i].push_back(index);
			}
		}
		isolateCardCount += isolateCardIndex[i].size();
	}
	{
		SwapWithEmpty(0, 1);
		SwapWithEmpty(0, 2);
		SwapWithEmpty(1, 0);
		SwapWithEmpty(1, 2);
		SwapWithEmpty(2, 0);
		SwapWithEmpty(2, 1);
	}
}

OptimizedCard::~OptimizedCard()
{
}
