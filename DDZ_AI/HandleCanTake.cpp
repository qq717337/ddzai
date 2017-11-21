#include "stdafx.h"
#include "HandleCanTake.h"

#include "SplitStrategy.h"
#include "PlayStrategyBase.h"

HandleCanTake::~HandleCanTake()
{
}

bool HandleCanTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	auto& avStyle = splitStrategy->GetAvailableStyle();
	if (avStyle.size() > 0) {
		result = avStyle[0];
		return true;
	}
}
