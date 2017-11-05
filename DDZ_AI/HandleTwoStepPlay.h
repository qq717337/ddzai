#pragma once
#include "PlayStrategyHandle.h"
class HandleTwoStepPlay :
	public PlayStrategyHandle
{
public:
	HandleTwoStepPlay()=default;
	~HandleTwoStepPlay();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleTwoStepPlay(const HandleTwoStepPlay&) = delete;
};

