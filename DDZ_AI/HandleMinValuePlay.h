#pragma once
#include "PlayStrategyHandle.h"

//������ƽ׶���С��ֵ����
class HandleMinValuePlay :
	public PlayStrategyHandle
{
public:
	HandleMinValuePlay()=default;
	~HandleMinValuePlay();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleMinValuePlay(const HandleMinValuePlay&) = delete;
};

