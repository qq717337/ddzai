#include "stdafx.h"
#include "HandleTwoStepPlay.h"


HandleTwoStepPlay::~HandleTwoStepPlay()
{
}

bool HandleTwoStepPlay::Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	auto& minSplit = splitStrategy->MinStepSplit();
	if (minSplit.MinStepCount() == 2) {
		auto& booms = minSplit.GetBoom();
		if (booms.size() == 2) {
			result = CardStyle::BoomStyle(booms[0]);
			return true;
		}
		//if (booms.size() == 1) {
		//	auto allCardStyle = minSplit.GetAllSplitStyle();
		//	uint8_t boomIndex = booms[0];
		//	for (auto & style : allCardStyle) {
		//		if (style.Style != ECardStyle::Boom) {
		//			result = style;
		//		}
		//	}
		//	return true;
		//}
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
