#pragma once
#include "PlayStrategyBase.h"
#include "PlayStrategyHandle.h"
#include <memory>
class LordPlayStrategy :public PlayStrategyBase
{
private:
	std::shared_ptr<SplitStrategy>m_keepBigSplitStrategy;

public:
	virtual void Init()override;
	virtual CardStyle Play() override;
	virtual CardStyle Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle) override;
	virtual bool OtherCanTake(const CardStyle& style)const override;
	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level, int param1, void* param2=nullptr) const override;
	virtual std::vector<ECardStyle::ECardStyle_> AvoidPlayStyle();
	virtual EIdentity::EIdentity_ Identity()const override;

	LordPlayStrategy(const CardVector & cardsValue, GameTable* table);
	LordPlayStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table);
	~LordPlayStrategy();
};

