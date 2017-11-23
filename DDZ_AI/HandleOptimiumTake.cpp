#include "stdafx.h"
#include "HandleOptimiumTake.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"

HandleOptimiumTake::~HandleOptimiumTake()
{
}

bool HandleOptimiumTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	if (playStrategy->GetLastCardStyle().Style == ECardStyle::Boom) {
		return false;
	}
	auto&optStyle = splitStrategy->GetOptimiumStyle();
	auto& avoidStyles = playStrategy->GetRealAvoidStyle();
	for (auto& opt : optStyle) {
		bool okStyle = true;
		for (auto& avoid : avoidStyles) {
			if (avoid.Compare(opt) > 0) {//如果需要避免的出牌中出现了，则忽略这个style
				okStyle = false;
				break;
			}
		}
		if (okStyle) {
			result = optStyle[0];
			return true;
		}
	}

	return false;
}
