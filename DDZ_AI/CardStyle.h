#pragma once
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include"CardConst.h"
class CardStyle
{
public:
	int Style;
	uint8_t StartValue;
	uint8_t EndValue;
	std::vector< uint8_t> Extra;
	CardStyle();
	~CardStyle();
	int  GetCardsCount();
	CardStyle(int style, uint8_t startValue, uint8_t endValue);
	CardStyle(int style,uint8_t startValue,uint8_t endValue, std::vector< uint8_t>& extra);
	CardStyle(int style, uint8_t startValue, uint8_t endValue, uint8_t extra);
	std::vector<uint8_t>GetCardsIndex();
	static CardStyle GetCardsStyleByCardsValue(std::vector<uint8_t>&cards);

	int Compare(const CardStyle& other);
};

const CardStyle Const_CardStyle_JokerBoomCardStyle=CardStyle(ECardStyle::Boom, CardIndex_SmallJoker, CardIndex_LargeJoker);