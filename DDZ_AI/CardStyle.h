#pragma once
#include <vector>
#include <set>
#include<unordered_map>
#include <algorithm>
#include"CardConst.h"

class CardStyle
{
private:
	std::string StyleString();
public:
	int Style;
	uint8_t StartValue;
	uint8_t EndValue;
	std::vector< uint8_t> Extra;
	CardStyle();
	~CardStyle();
	CardStyle(const CardStyle& inStyle);
	CardStyle(int style, uint8_t startValue);
	CardStyle(int style, uint8_t startValue, uint8_t endValue);
	CardStyle(int style, uint8_t startValue, uint8_t endValue, const CardVector& extra);
	CardStyle(int style, uint8_t startValue, const CardVector & extra);
	CardStyle(int style, uint8_t startValue, uint8_t endValue, uint8_t extra);
	int  GetCardsCount();
	CardStyle& operator=(const CardStyle& in);
	bool operator==(const CardStyle& in)const;
	bool operator!=(const CardStyle & in)const;
	CardVector Index();
	static CardStyle SingleStyle(uint8_t value);
	static CardStyle DoubleStyle(uint8_t value);
	static CardStyle TripleZeroStyle(uint8_t value);
	static CardStyle TripleOneStyle(uint8_t value, const CardVector &extra);
	static CardStyle TripleTwoStyle(uint8_t value, const CardVector &extra);
	static CardStyle BoomStyle(uint8_t value);
	static CardStyle QuadTwoStyle(uint8_t value, const CardVector &extra);
	static CardStyle SingleChainStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle DoubleChainStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle TripleChainZeroStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle TripleChainOneStyle(uint8_t startValue, uint8_t endValue, const CardVector &extra);
	static CardStyle TripleChainTwoStyle(uint8_t startValue, uint8_t endValue, const CardVector &extra);
	static CardStyle FromCardsValue(const CardVector &cards);
	int Compare(const CardStyle& other)const;
	std::string  ToString();
	const static CardStyle Invalid ;
	const static CardStyle JokerBoom;
	inline int Length()const { return EndValue - StartValue + 1; }
	inline bool Valid()const { return Style!=ECardStyle::Invalid; }
};