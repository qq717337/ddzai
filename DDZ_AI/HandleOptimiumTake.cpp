#include "stdafx.h"
#include "HandleOptimiumTake.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"

HandleOptimiumTake::~HandleOptimiumTake()
{
}

bool HandleOptimiumTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
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
