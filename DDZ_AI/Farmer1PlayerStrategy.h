#pragma once
#include "PlayStrategyBase.h"
#include<memory>
class Farmer1PlayerStrategy :
	public PlayStrategyBase
{
private:
	std::unique_ptr<PlayStrategyHandle>m_handlerLetFarmer2WinPlay;
	CardStyle takeLord(const CardStyle & lastStyle);
	CardStyle takeFarmer(const CardStyle & lastStyle);

public:
	~Farmer1PlayerStrategy();
	Farmer1PlayerStrategy(const CardVector & cardsValue, GameTable* table);
	Farmer1PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table);
	virtual CardStyle Play() override;
	virtual CardStyle Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)override;

	virtual bool OtherCanTake(const CardStyle& style)const override;
	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level, int param1, void* param2 = nullptr) const override;
	virtual std::vector<ECardStyle::ECardStyle_> AvoidPlayStyle();
	virtual EIdentity::EIdentity_ Identity()const override;
	virtual void Init()override;
};

