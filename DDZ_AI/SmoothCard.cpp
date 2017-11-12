#include "stdafx.h"
#include "SmoothCard.h"
#include "MinStepSplitStrategy.h"
#include <memory>
SmoothCard::SmoothCard()
{
}

void SmoothCard::Optimized(DecorateDealStrategy * strategy, int firstEpch, int secondEpch)
{
	ResetPlayerCard(0);
	if (strategy != nullptr)
		strategy->PreDeal();

	RandomFillLeft();

	Process(firstEpch);
#if _DEBUG
	auto firstTime = ToString();
#endif
	if (secondEpch != 0)
		Process(secondEpch);
#if _DEBUG
	auto secondTime = ToString();
#endif
}
void requireFilter(CardVector&a, CardVector&b, CardVector&c) {
	for (auto iter = a.begin(); iter != a.end();) {
		if ((*iter) > CardIndex_2) {
			iter = a.erase(iter);
		}
		else {
			++iter;
		}
	}
}
void SmoothCard::Process(int swapCount)
{
	MinStepSplitStrategy* splitStrategy[3];
	for (int k = 0; k < 3; k++) {
		int nextK = (k + 1) % 3;
		splitStrategy[k] = new MinStepSplitStrategy(std::make_shared<HandCards>(PlayerCardSet[k]->Data()));
		splitStrategy[k]->Split();

		SplitType& splitType = const_cast<SplitType&>(splitStrategy[k]->MinStepSplit());
		CardVector _single, _double, _triple;
		splitType.RequireFromAll(swapCount, _single, _double, _triple, requireFilter);
		for (uint8_t cardIndex : _single) {
			CardVector subValue = PlayerCardSet[k]->GetCardsValue(cardIndex, 1);
			for (uint8_t v : subValue) {
				PlayerCardSet[k]->RemoveCard(v);
				PlayerCardSet[nextK]->AddCard(v);
			}
		}
		for (uint8_t cardIndex : _double) {
			CardVector subValue = PlayerCardSet[k]->GetCardsValue(cardIndex, 2);
			for (uint8_t v : subValue) {
				PlayerCardSet[k]->RemoveCard(v);
				PlayerCardSet[nextK]->AddCard(v);
			}
		}
		for (uint8_t cardIndex : _triple) {
			CardVector subValue = PlayerCardSet[k]->GetCardsValue(cardIndex, 3);
			for (uint8_t v : subValue) {
				PlayerCardSet[k]->RemoveCard(v);
				PlayerCardSet[nextK]->AddCard(v);
			}
		}
	}
	Update();
}


SmoothCard::~SmoothCard()
{
}
