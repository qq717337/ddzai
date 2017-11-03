#pragma once
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
public:
	virtual void Split()override;
	virtual std::vector<CardStyle> OptimizeTake(const CardStyle & style)override;
	MinStepSplitStrategy(std::shared_ptr<HandCards> cards);
	~MinStepSplitStrategy();
};

