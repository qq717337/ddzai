#pragma once
#include"CardStyle.h"
class HandleResult
{
public:
	inline const CardStyle&  Style()const { return m_resultStyle; }
	HandleResult() = default;
	HandleResult(const CardStyle& style);
	HandleResult& AddBonusStep(int step);
	HandleResult& OtherTakeThisBonusStep(int step);
	HandleResult& OtherTakeLeftStep(int step);
	
private:
	//增加的步数
	int m_selfBonusStep;
	//其他人要的起的情况下造成的步数增加,0代表完美接住，如果接不住，则没有这个vector
	std::vector<int> m_otherTakeThisBonusStep;
	//其他人接这个牌之后需要多少步才可以赢
	std::vector<int> m_otherTakeLeftStep;

	//有几个玩家可以接这个牌
	const size_t OtherCanTakeCount()const {
		return m_otherTakeThisBonusStep.size();
	}
	CardStyle m_resultStyle;
};

