#pragma once
#include "PlayStrategyBase.h"
class Farmer2PlayerStrategy :
	public PlayStrategyBase
{
public:
	~Farmer2PlayerStrategy();
	Farmer2PlayerStrategy(const std::vector<uint8_t>& cardsValue, GameTable* table);
	Farmer2PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table);
	virtual CardStyle Play() override;
	virtual CardStyle Take(Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle)override;
	virtual int Identity()override; 
	virtual void Init()override;
};

