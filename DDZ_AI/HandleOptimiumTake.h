#pragma once
#include "PlayStrategyHandle.h"
//最佳接法，不处理炸弹，炸弹单独在HandleBoomTake里面处理
class HandleOptimiumTake :
	public PlayStrategyHandle
{
public:public:
	HandleOptimiumTake() = default; 
	~HandleOptimiumTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleOptimiumTake(const HandleOptimiumTake&) = delete;
};

