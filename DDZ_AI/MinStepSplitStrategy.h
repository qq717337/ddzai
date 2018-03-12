#ifndef _MINSTEPSPLITSTRATEGY_H_
#define _MINSTEPSPLITSTRATEGY_H_
#include "SplitStrategy.h"
#include <functional>
class MinStepSplitStrategy :public SplitStrategy
{
private:
	std::vector<std::function<void(void)>>m_split_function;
	void singleTripleChainFirst();
	void singleDoubleChainFirst();
	void singleTripleChainSecond();
	void singleDoubleChainSecond();
	void doubleSingleChainFirst();
	void doubleTripleChainFirst();
	void tripleDoubleChainFirst();
	void tripleSingleChainFirst();
	void insertOptimumChain(const SplitType* splitTypeRef, const std::vector<CardRange>& avChains, const CardStyle& inStyle);
	CardVector getTripleExtraIndex(const SplitType* splitTypeRef, const CardStyle& inStyle);
public:
	virtual void Split()override;
	virtual void OptimiumTake(const CardStyle & style)override;
	virtual void AvailableTake(const CardStyle & style)override;
	MinStepSplitStrategy(std::shared_ptr<HandCards> cards);
	MinStepSplitStrategy(const MinStepSplitStrategy&) = delete;
	~MinStepSplitStrategy();
};

#endif
