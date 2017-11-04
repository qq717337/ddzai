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
		//�Ƿ���Ҫǿ�ƽ��ƵȲ�������Ҫ�ŵ��������д���
		return true;
	}
	//ת��Ϊ�Լ���Ҫ��SplitStrategy���������麯���ж����һϵ�еĽӿ�
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
