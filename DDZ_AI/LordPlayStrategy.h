#pragma once
#include "PlayStrategyBase.h"
#include "PlayStrategyHandle.h"
#include "HandleCanOptimiumTake.h"
#include "HandleCanTake.h"
#include "HandleMinValuePlay.h"
#include "HandleLastShotPlay.h"
#include "HandleAvoidOtherWinPlay.h"
#include "HandleTwoStepPlay.h"
#include "HandleTwoStepWinTake.h"
#include <memory>
class LordPlayStrategy :public PlayStrategyBase
{
private:
	std::shared_ptr<SplitStrategy>m_minStepSplitStrategy;
	std::shared_ptr<SplitStrategy>m_keepBigSplitStrategy;

	std::unique_ptr<PlayStrategyHandle>m_handlerMinStepPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerLastShotPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerTwoStepPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerAvoidOtherWinPlay;

	std::unique_ptr<PlayStrategyHandle>m_handlerCanTake;
	std::unique_ptr<PlayStrategyHandle>m_handlerOptimiumTake;
	//检测两部可以走完的情况下是否可以压死获取胜利
	std::unique_ptr<PlayStrategyHandle>m_handlerCheckTwoStepWinTake;
public:
	virtual void Init()override;
	virtual CardStyle Play() override;
	virtual CardStyle Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle) override;
	virtual bool OtherCanTake(const CardStyle& style)const override;
	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const;
	virtual std::vector<ECardStyle::ECardStyle_> AvoidPlayStyle();
	virtual EIdentity::EIdentity_ Identity()const override;

	LordPlayStrategy(const CardVector & cardsValue, GameTable* table);
	LordPlayStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table);
	~LordPlayStrategy();
};

