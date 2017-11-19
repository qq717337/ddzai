#include "stdafx.h"
#include "HandleCanOptimiumTake.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"

HandleCanOptimiumTake::~HandleCanOptimiumTake()
{
}

bool HandleCanOptimiumTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	if (playStrategy->GetLastCardStyle().Style == ECardStyle::Boom) {
		return false;
	}
	auto&optStyle = splitStrategy->GetOptimiumStyle();

	if (optStyle.size() > 0) {
		result = optStyle[0];
		return true;
	}

	return false;
}
