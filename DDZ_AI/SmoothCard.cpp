#include "stdafx.h"
#include "SmoothCard.h"
#include "MinStepSplitStrategy.h"
#include <memory>
SmoothCard::SmoothCard()
{
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

void SmoothCard::ExchangeSet(MinStepSplitStrategy* splitStrategy, HandCards* addSet, HandCards* removeSet, int swapCount)
{
    splitStrategy->Split(); int cc = 0;

    SplitType& splitType = const_cast<SplitType&>(splitStrategy->MinStepSplit());
    CardVector _single, _double, _triple;
    splitType.RequireFromAll(swapCount, _single, _double, _triple, requireFilter);
    for (uint8_t cardIndex : _single) {
        CardVector subValue = removeSet->GetCardsValue(cardIndex, 1);
        for (auto v : subValue) {
            if (cc < swapCount) {
                ++cc;
                removeSet->RemoveCard(v);
                addSet->AddCard(v);
            }
        }
    }
    for (uint8_t cardIndex : _double) {
        CardVector subValue = removeSet->GetCardsValue(cardIndex, 2);
        for (auto v : subValue) {
            if (cc < swapCount) {
                ++cc;
                removeSet->RemoveCard(v);
                addSet->AddCard(v);
            }
        }
    }
    for (uint8_t cardIndex : _triple) {
        CardVector subValue = removeSet->GetCardsValue(cardIndex, 3);
        for (auto v : subValue) {
            if (cc < swapCount) {
                ++cc;
                removeSet->RemoveCard(v);
                addSet->AddCard(v);
            }
        }
    }
    _ASSERT(cc == swapCount);
}
void SmoothCard::Optimized(DecorateDealStrategy * strategy, std::vector <int>Epch)
{
    ResetPlayerCard(0);
    if (strategy != nullptr)
        strategy->PreDeal();

    RandomFillLeft();
    for (auto v : Epch) {
        if (v > 0) {
            Process(v);
        }
        else {
            ProcessDesk();
        }
#if _DEBUG
        auto secondTime = ToString();
#endif
    }
    if (PlayerCardSet[0]->Size() != PlayerCardSet[1]->Size() || PlayerCardSet[0]->Size() != PlayerCardSet[2]->Size()) {
        throw std::runtime_error("���ص�������һ��");
    }
}
void SmoothCard::Process(int swapCount)
{
    for (int k = 0; k < 3; k++) {
        int nextK = (k + 1) % 3;
        Update();
        MinStepSplitStrategy splitStrategy(std::make_shared<HandCards>(PlayerCardSet[k]->Data()));
        ExchangeSet(&splitStrategy, PlayerCardSet[nextK], PlayerCardSet[k], swapCount);
    }

    Update();
}

void SmoothCard::ProcessDesk()
{
    for (auto v : ExtraCard.Data()) {
        PlayerCardSet[0]->AddCard(v);
    }
    for (int k = 0; k < 3; ++k) {
        Update();
        MinStepSplitStrategy splitStrategy(std::make_shared<HandCards>(PlayerCardSet[k]->Data()));
        ExchangeSet(&splitStrategy, PlayerCardSet[(k + 1) % 3], PlayerCardSet[k], 3);
    }
    ExtraCard.Reset(false);
    Update();
    MinStepSplitStrategy splitStrategy(std::make_shared<HandCards>(PlayerCardSet[0]->Data()));
    ExchangeSet(&splitStrategy, &ExtraCard, PlayerCardSet[0], 3);
    Update();
}


SmoothCard::~SmoothCard()
{
}
