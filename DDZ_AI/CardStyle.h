#ifndef _CARDSTYLE_H_
#define _CARDSTYLE_H_
#include <vector>
#include <set>
#include<unordered_map>
#include <algorithm>
#include"CardConst.h"
#include "stdafx.h"
class CardStyle
{
private:
	std::string StyleString()const;
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
	int  GetCardsCount()const;
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
	static CardStyle QuadSingleStyle(uint8_t value, const CardVector &extra);
	static CardStyle QuadDoubleStyle(uint8_t value, const CardVector &extra);
	static CardStyle SingleChainStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle DoubleChainStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle TripleChainZeroStyle(uint8_t startValue, uint8_t endValue);
	static CardStyle TripleChainOneStyle(uint8_t startValue, uint8_t endValue, const CardVector &extra);
	static CardStyle TripleChainTwoStyle(uint8_t startValue, uint8_t endValue, const CardVector &extra); 
	
	static CardVector GetCardCount(CardVector cards);
	static CardStyle FromCardsValue(const CardVector &cards); 
	static CardStyle FromCardsValueWithLaizi(CardVector cards, uint8_t & laiZiIndex, ECardStyle::ECardStyle_ style);
	

	static CardStyle CheckIfLaiziSingleChain(CardVector& cardCount, CardVector& cards, uint8_t& laiZi);
	static CardStyle CheckIfLaiziDoubleChain(CardVector& cardCount, CardVector& cards, uint8_t& laiZi);
	static CardStyle CheckIfLaiziTripleChainZero(CardVector& cardCount, CardVector& cards, uint8_t& laiZi);
	 static CardStyle CheckIfLaiziTripleChainOne(CardVector& cardCount, CardVector& cards, uint8_t& laiZi);
	 static CardStyle CheckIfLaiziTripleChainTwo(CardVector& cardCount, CardVector& cards, uint8_t& laiZi);

	int Compare(const CardStyle& other)const;
	std::string  ToString()const;
	const static CardStyle Invalid ;
	const static CardStyle JokerBoom;
	inline int Length()const { return EndValue - StartValue + 1; }
	inline bool Valid()const { return Style!=ECardStyle::Invalid; }
	// «∑Ò «¥Û≈∆
	bool IsBigCard()const;
};
#endif
