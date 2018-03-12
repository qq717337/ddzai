#include "stdafx.h"
#include "StateInfo.h"


StateInfo::StateInfo()
{
}


StateInfo::~StateInfo()
{
}

Position * StateInfo::getCurPosition()
{
	return handCards[sideToMove].get();
}
