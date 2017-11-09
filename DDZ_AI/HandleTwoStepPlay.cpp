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
			result = CardStyle::BoomStyle(booms[1]);
			return true;
		}

		auto allCardStyle = minSplit.GetAllSplitStyle();
		for (auto & style : allCardStyle) {
			if (!playStrategy->OtherCanTake(style)) {//����ҵ���������Ҫ������ƣ������ȳ�����Ҫ�����
				result = style;
				return true;
			}
		}
	}
	return false;
}
