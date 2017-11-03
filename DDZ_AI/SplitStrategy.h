#pragma once
#include "SplitStrategyBase.h"
#include <functional>
//每一个继承的子类负责额外的Split信息，例如在地主出牌且自己的牌比较多的时候，
//采取MinStepSplitStrategy ,接牌的时候采用另外的拆分策略，需要把需要接的牌给传进去
class SplitStrategy :
	public SplitStrategyBase
{
protected:
	std::vector<SplitType> m_splitTypeVector;
	std::vector<size_t> m_splitMinStepVector;
public:
	const SplitType& MinStepSplit()const;
	virtual void Split();
	virtual std::vector<CardStyle> OptimizeTake(const CardStyle & style);
	SplitStrategy(std::shared_ptr< HandCards>cards);
	virtual ~SplitStrategy();
};