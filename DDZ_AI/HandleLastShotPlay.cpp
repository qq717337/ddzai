#include "stdafx.h"
#include "HandleLastShotPlay.h"


HandleLastShotPlay::~HandleLastShotPlay()
{
}

bool HandleLastShotPlay::Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	const SplitType& split = splitStrategy->MinStepSplit();
	return split.GetLastShotCardStyle(&result);
}
