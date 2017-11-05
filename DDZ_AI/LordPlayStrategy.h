#pragma once
#include "PlayStrategyBase.h"
#include "PlayStrategyHandle.h"
#include "HandleCanOptimiumTake.h"
#include "HandleCanTake.h"
#include "HandleMinValuePlay.h"
#include "HandleLastShot.h"

#include <memory>
class LordPlayStrategy :
	public PlayStrategyBase
{
private:
	std::shared_ptr<SplitStrategy>m_minStepSplitStrategy;
	std::shared_ptr<SplitStrategy>m_keepBigSplitStrategy;

	std::unique_ptr<PlayStrategyHandle>m_handlerMinStepPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerLastShot;
	std::unique_ptr<PlayStrategyHandle>m_handlerCanTake;
	std::unique_ptr<PlayStrategyHandle>m_handlerOptimiumTake;
public:
	virtual void Init()override;
	virtual CardStyle Play() override;
	virtual CardStyle Take(Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle) override;
	virtual bool OtherCanTake(const CardStyle& style)const override;
	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const ;
	virtual int Identity() override;

	LordPlayStrategy(const std::vector<uint8_t>& cardsValue, GameTable* table);
	LordPlayStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table);
	~LordPlayStrategy();
};

