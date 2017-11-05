#pragma once
#include "PlayStrategyHandle.h"
class HandleCanTake :
	public PlayStrategyHandle
{
public:
	HandleCanTake()=default;
	~HandleCanTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleCanTake(const HandleCanTake&) = delete;
};

