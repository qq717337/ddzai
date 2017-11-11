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
			if (!playStrategy->OtherCanTake(style)) {//����ҵ���������Ҫ������ƣ������ȳ�����Ҫ�����
				result = style;
				return true;
			}
		}
	}
	return false;
}
