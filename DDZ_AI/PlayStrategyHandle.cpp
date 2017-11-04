#include "stdafx.h"
#include "PlayStrategyHandle.h"

PlayStrategyHandle::PlayStrategyHandle()
{
}

PlayStrategyHandle::~PlayStrategyHandle()
{
}

bool HandleCanTakeCard::CanTake(SplitStrategy*  strategy)
{
	return strategy->GetAvailableStyle().size()>0;
}

HandleCanTakeCard::HandleCanTakeCard()
{
}

bool HandleCanTakeCard::Handle(SplitStrategy * strategy, CardStyle & result)
{
	if (CanTake(strategy)) {
		//是否需要强制接牌等操作还是要放到后续进行处理
		return true;
	}
	//转换为自己需要的SplitStrategy，或者在虚函数中定义好一系列的接口
	return false;
}

bool HandleCanOptimiumTakeCard::CanOptimiumTake(SplitStrategy * strategy)
{
	return strategy->GetOptimiumStyle().size()>0;
}

bool HandleCanOptimiumTakeCard::Handle(SplitStrategy * strategy, CardStyle & result)
{
	if (CanOptimiumTake(strategy)) {
		result = strategy->GetOptimiumStyle()[0];
		return true;
	}

	return false;
}
