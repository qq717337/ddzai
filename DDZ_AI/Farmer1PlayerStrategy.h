#pragma once
#include "PlayStrategyBase.h"
class Farmer1PlayerStrategy :
	public PlayStrategyBase
{
public:
	~Farmer1PlayerStrategy();
	Farmer1PlayerStrategy(const std::vector<uint8_t>& cardsValue, GameTable* table);
	Farmer1PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table);
	virtual CardStyle Play() override;
	virtual CardStyle Take(Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle)override;
	virtual int Identity()override;
	virtual void Init()override;
};

