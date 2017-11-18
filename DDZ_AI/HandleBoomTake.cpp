#include "stdafx.h"
#include "HandleBoomTake.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"


HandleBoomTake::~HandleBoomTake()
{
}

bool HandleBoomTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	if (splitStrategy->GetAvailableStyle().size() == 0) {

		if (!playStrategy->IsSafeSituation(ESituationSafeLevel::AllPlayerMinStepGreater2) &&
			playStrategy->GetLastCardStyle().Style != ECardStyle::Boom) {
			const HandCards& cards = playStrategy->GetHandCards();
			auto booms = cards.AvailableBoom();
			if (booms.size() > 0) {
				result = CardStyle(ECardStyle::Boom, booms[0]);
				return  true;
			}
		}
	}
	return false;
}
