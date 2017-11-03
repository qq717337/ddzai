#include "stdafx.h"
#include "PlayStrategyHandle.h"

HandleResult HandleResult::Failure = { false,CardStyle::Invalid };

PlayStrategyHandle::PlayStrategyHandle()
{
}

PlayStrategyHandle::~PlayStrategyHandle()
{
}

bool HandleCanTakeCard::CanTake(SplitStrategy*  strategy)
{
	return false;
}

HandleCanTakeCard::HandleCanTakeCard()
{
}

bool HandleCanTakeCard::Handle(SplitStrategy * strategy, CardStyle & result)
{
	strategy->Split();
	strategy->OptimizeTake(result);
	if (CanTake(strategy)) {
		return true;
	}
	else {

	}
	//转换为自己需要的SplitStrategy，或者在虚函数中定义好一系列的接口
	return false;
}
