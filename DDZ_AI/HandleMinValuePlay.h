#pragma once
#include "PlayStrategyHandle.h"

//������ƽ׶���С��ֵ����
class HandleMinValuePlay :
	public PlayStrategyHandle
{
public:
	HandleMinValuePlay()=default;
	~HandleMinValuePlay();
	virtual bool Handle(SplitStrategy * strategy, CardStyle & result)override;
	HandleMinValuePlay(const HandleCanOptimiumTakeCard&) = delete;
};

