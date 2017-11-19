#pragma once
#include "PlayStrategyHandle.h"
//最佳接法，不处理炸弹，炸弹单独在HandleBoomTake里面处理
class HandleCanOptimiumTake :
	public PlayStrategyHandle
{
public:public:
	HandleCanOptimiumTake() = default; 
	~HandleCanOptimiumTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleCanOptimiumTake(const HandleCanOptimiumTake&) = delete;
};

