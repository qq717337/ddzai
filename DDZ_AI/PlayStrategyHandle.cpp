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
	//ת��Ϊ�Լ���Ҫ��SplitStrategy���������麯���ж����һϵ�еĽӿ�
	return false;
}
