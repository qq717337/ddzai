#pragma once
#include "PlayStrategyHandle.h"
//��ѽӷ���������ը����ը��������HandleBoomTake���洦��
class HandleOptimiumTake :
	public PlayStrategyHandle
{
public:public:
	HandleOptimiumTake() = default; 
	~HandleOptimiumTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleOptimiumTake(const HandleOptimiumTake&) = delete;
};

