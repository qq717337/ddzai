#include "stdafx.h"
#include "HandleTwoStepPlay.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"


HandleTwoStepPlay::~HandleTwoStepPlay()
{
}

bool HandleTwoStepPlay::Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	auto& minSplit = splitStrategy->MinStepSplit();
	CardStyle lastShot;
	//����Լ����һ���պýӹ����ƣ���ô�Ϳ���ʤ��
	if (minSplit.GetLastShotCardStyle(&lastShot))
	{
		auto& lastPlayCardStyle = playStrategy->GetLastCardStyle();
		if (lastShot.Compare(lastPlayCardStyle) > 0) {
			result = lastShot;
			return true;
		}
	}
	if (minSplit.MinStepCount() == 2) {
		auto& booms = minSplit.GetBoom();
		if (booms.size() == 2) {
			result = CardStyle::BoomStyle(booms[0]);
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
