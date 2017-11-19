#include "stdafx.h"
#include "HandleLetFarmer2WinPlay.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"
bool HandleLetFarmer2WinPlay::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	auto farmer2_strategy = playStrategy->GetStrategyPtr(EIdentity::Farmer2);
	HandCards& farmer2_handcards = const_cast<HandCards&> (farmer2_strategy->GetHandCards());//��ȡFarmer2����һ���߷�
	CardStyle farmer2_lastShot = farmer2_handcards.LastShot();
	if (!farmer2_lastShot.Valid()) {//Farmer2�Ҳ������һ����������Handle
		return false;
	}

	CardStyle lessStyle = playStrategy->GetHandCards().FindLessCardStyle(farmer2_lastShot);
	if (lessStyle.Valid()) {//������û���ҵ���Farmer2���һ���Ƹ�С�������
		result = lessStyle;
		return true;
	}
	return false;
}
