#include "stdafx.h"
#include "HandleCanTake.h"

#include "SplitStrategy.h"
#include "PlayStrategyBase.h"

HandleCanTake::~HandleCanTake()
{
}

bool HandleCanTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	return splitStrategy->GetAvailableStyle().size()>0;
}
