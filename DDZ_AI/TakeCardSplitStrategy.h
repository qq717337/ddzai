#pragma once
#include "SplitStrategy.h"
//接续的牌怎样拆分，一开始按照最小拆分，然后在这个的基础上进行进一步的拆分

class TakeCardSplitStrategy :
	public SplitStrategy
{
public:
	TakeCardSplitStrategy(std::shared_ptr<HandCards> cards,const CardStyle& style);
	~TakeCardSplitStrategy();
};

