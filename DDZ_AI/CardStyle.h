#pragma once
#include <vector>
#include <set>
#include<unordered_map>
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
	CardStyle(int style, uint8_t startValue);
	CardStyle(int style, uint8_t startValue, uint8_t endValue);
	CardStyle(int style, uint8_t startValue, uint8_t endValue, const std::vector< uint8_t>& extra);
	CardStyle(int style, uint8_t startValue, const std::vector<uint8_t>& extra);
	CardStyle(int style, uint8_t startValue, uint8_t endValue, uint8_t extra);
	int  GetCardsCount();
	CardStyle& operator=(const CardStyle& in);
	std::vector<uint8_t>Index();
	static CardStyle SingleStyle(uint8_t value);
	static CardStyle DoubleStyle(uint8_t value);
	static CardStyle TripleZeroStyle(uint8_t value);
	static CardStyle TripleOneStyle(uint8_t value, const std::vector<uint8_t>&extra);
	static CardStyle TripleTwoStyle(uint8_t value, const std::vector<uint8_t>&extra);
	static CardStyle BoomStyle(uint8_t value);
	static CardStyle QuadTwoStyle(uint8_t value, const std::vector<uint8_t>&extra);
	static CardStyle SingleChainStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle DoubleChainStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle TripleChainZeroStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle TripleChainOneStyle(uint8_t startValue, uint8_t endValue, const std::vector<uint8_t>&extra);
	static CardStyle TripleChainTwoStyle(uint8_t startValue, uint8_t endValue, const std::vector<uint8_t>&extra);
	static CardStyle GetCardStyleByCardsValue(const std::vector<uint8_t>&cards);
	int Compare(const CardStyle& other);
	std::string  ToString();
	std::string StyleString();
};

const CardStyle Const_CardStyle_JokerBoom = CardStyle(ECardStyle::Boom, CardIndex_SmallJoker, CardIndex_LargeJoker);
const CardStyle Const_CardStyle_Invalid = CardStyle(ECardStyle::Invalid, 0, 0);