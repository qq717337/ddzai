#include "stdafx.h"
#include "SplitStrategy.h"


const SplitType & SplitStrategy::MinStepSplit() const
{
	VECTOR_MIN_INDEX(index, m_splitMinStepVector);
	return m_splitTypeVector[index];
}

void SplitStrategy::Split()
{
	SplitIsolate();
	SplitBoom();
	SplitDoubleChain();
	SplitTripleChain();
	SplitSingleChain();
	SplitTriple();
	SplitIsolate();
}

std::vector< CardStyle>  SplitStrategy::OptimizeTake(const CardStyle & style)
{
	return { CardStyle::Invalid };
}

SplitStrategy::SplitStrategy(std::shared_ptr<HandCards> cards):SplitStrategyBase(cards)
{
}

SplitStrategy::~SplitStrategy()
{
}
