#include "stdafx.h"
#include "GameTable.h"


GameTable::GameTable()
{
}


GameTable::~GameTable()
{
}

void GameTable::Play(Identity::EIdentity_ identity)
{
	getStrategy(identity)->Play();
}

void GameTable::Take(Identity::EIdentity_ identity, Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	getStrategy(identity)->Take(lastIdentity, lastStyle);
}
