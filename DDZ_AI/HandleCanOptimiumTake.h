#pragma once
#include "PlayStrategyHandle.h"
//��ѽӷ���������ը����ը��������HandleBoomTake���洦��
class HandleCanOptimiumTake :
	public PlayStrategyHandle
{
public:public:
	HandleCanOptimiumTake() = default; 
	~HandleCanOptimiumTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleCanOptimiumTake(const HandleCanOptimiumTake&) = delete;
};

