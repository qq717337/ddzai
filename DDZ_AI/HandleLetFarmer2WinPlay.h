#pragma once
#include "PlayStrategyHandle.h"
//Farmer1�ڼ�⵽Farmer2����ʤ��������´����Farmer2ʤ������
class HandleLetFarmer2WinPlay :
	public PlayStrategyHandle
{
public:
	HandleLetFarmer2WinPlay() = default;
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleLetFarmer2WinPlay(const HandleLetFarmer2WinPlay&) = delete;
};

