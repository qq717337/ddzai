#pragma once
#include "PlayStrategyHandle.h"

//处理出牌阶段最小的值出发
class HandleMinValuePlay :
	public PlayStrategyHandle
{
public:
	HandleMinValuePlay()=default;
	~HandleMinValuePlay();
	virtual bool Handle(SplitStrategy * strategy, CardStyle & result)override;
	HandleMinValuePlay(const HandleCanOptimiumTakeCard&) = delete;
};

