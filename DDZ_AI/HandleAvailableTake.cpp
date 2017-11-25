#include "stdafx.h"
#include "HandleAvailableTake.h"

#include "SplitStrategy.h"
#include "PlayStrategyBase.h"

HandleAvailableTake::~HandleAvailableTake()
{
}
bool HandleAvailableTake::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	auto& lastStyle = playStrategy->GetLastCardStyle();
	if (lastStyle.Style == ECardStyle::Single) {
		const CardVector& double_ = playStrategy->GetHandCards()->AvailableDouble(true, lastStyle.StartValue);//先从double里面获取
		for (auto v : double_) {
			if (v == CardIndex_2) {
				result = CardStyle::SingleStyle(CardIndex_2);
				return true;
			}
			if (v >= playStrategy->OtherBiggestCardValue(1)) {//如果别人最大的牌也压制不了我这张，那么也可以
				result = CardStyle::SingleStyle(v);
				return true;
			}
		}
		const CardVector& triple_ = playStrategy->GetHandCards()->AvailableTriple(true, lastStyle.StartValue);
		for (auto v : triple_) {
			if (v == CardIndex_2) {
				result = CardStyle::SingleStyle(CardIndex_2);
				return true;
			}
			if (v >= playStrategy->OtherBiggestCardValue(1)) {//如果别人最大的牌也压制不了我这张，那么也可以
				result = CardStyle::SingleStyle(v);
				return true;
			}
		};

	}
	if (lastStyle.Style == ECardStyle::Double) {
		const CardVector& triple_ = playStrategy->GetHandCards()->AvailableTriple(true, lastStyle.StartValue);
		for (auto v : triple_) {
			if (v == CardIndex_2) {
				result = CardStyle::DoubleStyle(CardIndex_2);
				return true;
			}
			if (v >= playStrategy->OtherBiggestCardValue(2)) {//如果别人最大的牌也压制不了我这张，那么也可以
				result = CardStyle::DoubleStyle(v);
				return true;
			}
		}
	}
	auto& avStyle = splitStrategy->GetAvailableStyle();
	if (avStyle.size() > 0) {
		result = avStyle[0];
		return true;
	}
	return false;
}
