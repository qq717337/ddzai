#include "stdafx.h"
#include "HandleLastShotPlay.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"


bool HandleLastShotPlay::Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	const SplitType& split = splitStrategy->MinStepSplit();
	return split.GetLastShotCardStyle(&result);
}
