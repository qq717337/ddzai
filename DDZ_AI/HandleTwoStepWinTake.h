#pragma once
#include "PlayStrategyHandle.h"
class HandleTwoStepWinTake :
	public PlayStrategyHandle
{
public:
	HandleTwoStepWinTake();
	~HandleTwoStepWinTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleTwoStepWinTake(const HandleTwoStepWinTake&) = delete;
};

