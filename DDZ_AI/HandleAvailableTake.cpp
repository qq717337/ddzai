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
		for (auto& sp : splitStrategy->GetSplitType(1)) {
			const CardVector& double_= sp->GetDouble();//�ȴ�double�����ȡ
			for (auto v : double_) {
				if (v == CardIndex_2) {
					result = CardStyle::SingleStyle(CardIndex_2);
					return true;
				}
				if (v >= playStrategy->OtherBiggestCardValue(1)) {//�������������Ҳѹ�Ʋ��������ţ���ôҲ����
					result = CardStyle::SingleStyle(v);
					return true;
				}
			}		
			const CardVector& triple_ = sp->GetTriple();
			for (auto v : triple_) {
				if (v == CardIndex_2) {
					result = CardStyle::SingleStyle(CardIndex_2);
					return true;
				}
				if (v >= playStrategy->OtherBiggestCardValue(1)) {//�������������Ҳѹ�Ʋ��������ţ���ôҲ����
					result = CardStyle::SingleStyle(v);
					return true;
				}
			}
		};
		
	}
	if (lastStyle.Style == ECardStyle::Double) {
		for (auto& sp : splitStrategy->GetSplitType(1)) {
			const CardVector& triple_ = sp->GetTriple();
			for (auto v : triple_) {
				if (v == CardIndex_2) {
					result = CardStyle::SingleStyle(CardIndex_2);
					return true;
				}
				if (v >= playStrategy->OtherBiggestCardValue(1)) {//�������������Ҳѹ�Ʋ��������ţ���ôҲ����
					result = CardStyle::SingleStyle(v);
					return true;
				}
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
