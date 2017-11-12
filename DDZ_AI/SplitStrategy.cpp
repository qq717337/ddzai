#include "stdafx.h"
#include "SplitStrategy.h"


void SplitStrategy::sortSplitType()
{
	std::sort(m_splitTypeVector.begin(), m_splitTypeVector.end(), [](auto x, auto y) {return x.MinStepCount() < y.MinStepCount(); });
	std::sort(m_splitMinStepVector.begin(), m_splitMinStepVector.end());
}

void SplitStrategy::Reset()
{
	m_splitTypeVector.clear();
	m_splitMinStepVector.clear();
	m_optimiumStyle.clear();
	m_availableStyle.clear();
}

const SplitType & SplitStrategy::MinStepSplit() const
{
	//VECTOR_MIN_INDEX(index, m_splitMinStepVector);
	return m_splitTypeVector[0];
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

void SplitStrategy::OptimiumTake(const CardStyle & style)
{
}

void SplitStrategy::AvailableTake(const CardStyle & style)
{
}

std::vector<const SplitType*> SplitStrategy::GetSplitType(int moreStepThanMinStep)
{
	std::vector<const SplitType*> r;
	int lowerBoundStep = m_splitMinStepVector[0] + moreStepThanMinStep;
	for (int i = 0; i < m_splitMinStepVector.size(); i++) {
		if (m_splitMinStepVector[i] > lowerBoundStep) {
			break;
		}
		r.push_back(&m_splitTypeVector[i]);
	}
	return r;
}

SplitStrategy::SplitStrategy(std::shared_ptr<HandCards> cards) :SplitStrategyBase(cards)
{
}

SplitStrategy::~SplitStrategy()
{
}
