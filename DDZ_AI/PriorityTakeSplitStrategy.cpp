#include "stdafx.h"
#include "PriorityTakeSplitStrategy.h"


PriorityTakeSplitStrategy::PriorityTakeSplitStrategy(std::shared_ptr<HandCards> cards, const CardStyle & takeStyle) :SplitStrategy(cards)
{
}

PriorityTakeSplitStrategy::~PriorityTakeSplitStrategy()
{
}

void PriorityTakeSplitStrategy::Split()
{
}

void PriorityTakeSplitStrategy::OptimiumTake(const CardStyle & style)
{
}

void PriorityTakeSplitStrategy::AvailableTake(const CardStyle & style)
{
}

bool PriorityTakeSplitStrategy::CanTake()
{
	return m_availableStyle.size() > 0 || m_optimiumStyle.size() > 0;
}
