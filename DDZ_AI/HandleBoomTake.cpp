#include "stdafx.h"
#include "HandleBoomTake.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"


HandleBoomTake::~HandleBoomTake()
{
}

bool HandleBoomTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	CardStyle bestBoom;
	bool hasOptBoom = false;//�Ƿ�����ѵ�ը��
	auto& lastStyle = playStrategy->GetLastCardStyle();
	auto& booms = splitStrategy->GetOptimiumBoomStyle();
	for (auto& v : booms) {
		if (v.Style().Compare(lastStyle) > 0) {//�ҵ��˿���
			bestBoom = v;
			hasOptBoom = true;
		}
	}
	if (!bestBoom.Valid()) {
		const HandCards* cards = playStrategy->GetHandCards();
		auto booms = cards->AvailableBoom();
		if (booms.size() > 0) {
			bestBoom = CardStyle(ECardStyle::Boom, booms[0]);
		}
	}
	if (!bestBoom.Valid()) {
		return false;
	}

	bool mustTake = false;
	if (playStrategy->IsSafeSituation(ESituationSafeLevel::AllPlayerBoomCountLess, 1)) {
		mustTake = true;
	}
	if (mustTake) {
		if (booms.size() > 0) {
			result = bestBoom;
			return true;
		}
		else {
			return false;
		}
	}
	if (!mustTake && !hasOptBoom) {
		return false;
	}
	if (lastStyle.Style == ECardStyle::Boom) {
		//����Է���ը����С��2�ҳ�ը����С��������2,��ȫ״̬�²�ʹ��ը��
		if (playStrategy->IsSafeSituation(ESituationSafeLevel::AllPlayerBoomCountLess, 2) && playStrategy->IsSafeSituation(ESituationSafeLevel::AllPlayerMinStepWithOutBoomGreater, 2)) {
			return false;
		}
		else {
			result = bestBoom;
			return true;
		}
	}
	else {
		if (!playStrategy->IsSafeSituation(ESituationSafeLevel::AllPlayerMinStepWithOutBoomGreater, 2)) {
			result = bestBoom;
			return true;
		}
	}
	return false;
}
