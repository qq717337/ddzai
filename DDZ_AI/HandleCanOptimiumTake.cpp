#include "stdafx.h"
#include "HandleCanOptimiumTake.h"

HandleCanOptimiumTake::~HandleCanOptimiumTake()
{
}

bool HandleCanOptimiumTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	auto&optStyle = splitStrategy->GetOptimiumStyle();
	if (optStyle.size() > 0) {
		result = optStyle[0].Style();
		return true;
	}

	return false;
}
