#pragma once
#include "SplitStrategy.h"
//优先接某个Style的拆分策略,用于自己出牌判断会不会被其他人接住，或者自己最小拆分无法接牌时，强制接牌采取的拆分策略
class PriorityTakeSplitStrategy :
	public SplitStrategy
{
public:
	PriorityTakeSplitStrategy(std::shared_ptr<HandCards> cards, const CardStyle& takeStyle);
	~PriorityTakeSplitStrategy();
	virtual void Split()override;
	virtual void OptimiumTake(const CardStyle & style)override;
	virtual void AvailableTake(const CardStyle & style)override;
	bool CanTake();
};

