#ifndef _FARMER2PLAYERSTRATEGY_H_
#define _FARMER2PLAYERSTRATEGY_H_
#include "PlayStrategyBase.h"
class Farmer2PlayerStrategy :
	public PlayStrategyBase
{
private:
	CardStyle takeLord(const CardStyle & lastStyle);
	CardStyle takeFarmer(const CardStyle & lastStyle);

public:
	~Farmer2PlayerStrategy();
	Farmer2PlayerStrategy(const CardVector & cardsValue, GameTable* table);
	Farmer2PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table);
	virtual CardStyle Play() override;
	virtual CardStyle Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)override;
	virtual bool OtherCanTake(const CardStyle& style)const override;
	virtual uint8_t OtherBiggestCardValue(int compareCount)const override;
	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level, int param1, void* param2 = nullptr) const override;
	virtual std::vector<ECardStyle::ECardStyle_> AvoidPlayStyle();
	virtual EIdentity::EIdentity_ Identity()const override;
	virtual void Init()override;
};

#endif
