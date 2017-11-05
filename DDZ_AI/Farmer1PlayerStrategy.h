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
	virtual CardStyle Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)override;
	virtual bool OtherCanTake(const CardStyle& style)const override;
	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const;
	virtual std::vector<ECardStyle::ECardStyle_> AvoidPlayStyle();
	virtual int EIdentity()override;
	virtual void Init()override;
};

