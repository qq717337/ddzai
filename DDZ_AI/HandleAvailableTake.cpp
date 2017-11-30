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
	auto splitType = splitStrategy->GetSplitType(0)[0];
	if (lastStyle.Style == ECardStyle::Single) {
		uint8_t otherBiggestCardValue = playStrategy->OtherBiggestCardValue(1);
		for (auto& v : splitType->GetSingleChain()) {
			if (v.End > lastStyle.EndValue && v.End >= otherBiggestCardValue) {
				result = CardStyle::SingleStyle(v.End);
				return true;
			}
		}
		const CardVector& double_ = playStrategy->GetHandCards()->AvailableDouble(true, lastStyle.StartValue);//�ȴ�double�����ȡ
		for (auto v : double_) {
			if (v == CardIndex_2) {
				result = CardStyle::SingleStyle(CardIndex_2);
				return true;
			}
			if (v >= otherBiggestCardValue) {//�������������Ҳѹ�Ʋ��������ţ���ôҲ����
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
			if (v >= otherBiggestCardValue) {//�������������Ҳѹ�Ʋ��������ţ���ôҲ����
				result = CardStyle::SingleStyle(v);
				return true;
			}
		};

	}
	if (lastStyle.Style == ECardStyle::Double) {
		uint8_t otherBiggestCardValue = playStrategy->OtherBiggestCardValue(2);

		for (auto& v : splitType->GetDoubleChain()) {
			if (v.End > lastStyle.EndValue && v.End >= otherBiggestCardValue) {
				result = CardStyle::DoubleStyle(v.End);
				return true;
			}
		}
		const CardVector& triple_ = playStrategy->GetHandCards()->AvailableTriple(true, lastStyle.StartValue);
		for (auto v : triple_) {
			if (v == CardIndex_2) {
				result = CardStyle::DoubleStyle(CardIndex_2);
				return true;
			}
			if (v >= otherBiggestCardValue) {//�������������Ҳѹ�Ʋ��������ţ���ôҲ����
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
