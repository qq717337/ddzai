#pragma once
#include "PlayStrategyHandle.h"
class HandleCanOptimiumTake :
	public PlayStrategyHandle
{
public:public:
	HandleCanOptimiumTake() = default; 
	~HandleCanOptimiumTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleCanOptimiumTake(const HandleCanOptimiumTake&) = delete;
};

