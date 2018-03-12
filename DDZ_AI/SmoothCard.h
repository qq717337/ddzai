#ifndef SMOOTHCARD_H
#define SMOOTHCARD_H
#include "DecorateDealStrategy.h"
#include "MinStepSplitStrategy.h"
class SmoothCard :
    public CardSet
{
public:
    SmoothCard();
    void Optimized(DecorateDealStrategy * strategy, std::vector<int>Epch);
    void Process(int swapCount);
    void ExchangeSet(MinStepSplitStrategy* splitStrategy, HandCards* addSet, HandCards* removeSet, int swapCount);
    void ProcessDesk();
    ~SmoothCard();
};

#endif // SMOOTHCARD_H
