#include "stdafx.h"
#include "HandleTwoStepWinTake.h"


HandleTwoStepWinTake::HandleTwoStepWinTake()
{
}


HandleTwoStepWinTake::~HandleTwoStepWinTake()
{
}

bool HandleTwoStepWinTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	auto& minSplit = splitStrategy->MinStepSplit();
	if (minSplit.MinStepCount() == 2) {
		auto allCardStyle = minSplit.GetAllSplitStyle();
		for (auto & style : allCardStyle) {
			if (!playStrategy->OtherCanTake(style)) {//如果找到了其他人要不起的牌，则优先出别人要不起的
				result = style;
				return true;
			}
		}
	}
	return false;
}
