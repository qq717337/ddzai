#include "stdafx.h"
#include "HandleLetFarmer2WinPlay.h"
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"
bool HandleLetFarmer2WinPlay::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	auto farmer2_strategy = playStrategy->GetStrategyPtr(EIdentity::Farmer2);
	HandCards& farmer2_handcards = const_cast<HandCards&> (farmer2_strategy->GetHandCards());//获取Farmer2最后的一个走法
	CardStyle farmer2_lastShot = farmer2_handcards.LastShot();
	if (!farmer2_lastShot.Valid()) {//Farmer2找不到最后一步则忽略这个Handle
		return false;
	}

	CardStyle lessStyle = playStrategy->GetHandCards().FindLessCardStyle(farmer2_lastShot);
	if (lessStyle.Valid()) {//手牌中没有找到比Farmer2最后一手牌更小的则忽略
		result = lessStyle;
		return true;
	}
	return false;
}
