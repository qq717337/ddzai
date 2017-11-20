#include "stdafx.h"
#include "SplitStrategy.h"


void SplitStrategy::sortSplitType()
{
	std::sort(m_splitTypeVector.begin(), m_splitTypeVector.end(), [](auto x, auto y) {return x.MinStepCount() < y.MinStepCount(); });
	std::sort(m_splitMinStepVector.begin(), m_splitMinStepVector.end());
}

void SplitStrategy::Reset()
{
	m_hasSplit = false;
	m_hasGetAvailable = false;
	m_hasGetOptimium = false;
	m_splitTypeVector.clear();
	m_splitMinStepVector.clear();
	m_optimiumStyle.clear();
	m_availableStyle.clear();
}

const SplitType & SplitStrategy::MinStepSplit() const
{
	if (!m_hasSplit) {
		const_cast<SplitStrategy*>(this)->Split();
	}
	//VECTOR_MIN_INDEX(index, m_splitMinStepVector);
	return m_splitTypeVector[0];
}

void SplitStrategy::Split()
{
	m_hasSplit = true;
}

void SplitStrategy::OptimiumTake(const CardStyle & style)
{
	m_hasGetOptimium = true;
}

void SplitStrategy::AvailableTake(const CardStyle & style)
{
	m_hasGetAvailable = true;
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

SplitStrategy::SplitStrategy(std::shared_ptr<HandCards> cards) :SplitStrategyBase(cards), m_hasSplit(false),m_hasGetOptimium(false),m_availableStyle(false)
{
}

SplitStrategy::~SplitStrategy()
{
}
