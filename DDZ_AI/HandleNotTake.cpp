#include "stdafx.h"
#include "HandleNotTake.h"


HandleNotTake::~HandleNotTake()
{
}

bool HandleNotTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	return false;
}
