#pragma once

#include"stdafx.h"
#include <vector>
#include<string>
#include <memory>
#include "CardConst.h"
#include"Range.h"
#include"CardStyle.h"
struct CardSetCompare
{
	bool operator()(const uint8_t& lhs, const  uint8_t& rhs)const
	{
		uint8_t cardType1 = lhs & 0xf0;
		uint8_t cardIndex1 = lhs & 0x0f;
		uint8_t cardType2 = rhs & 0xf0;
		uint8_t cardIndex2 = rhs & 0x0f;
		if (cardType1 == 0) {
			if (cardType2 == 0) {
				return cardIndex1 < cardIndex2;
			}
			else {
				return false;
			}
		}
		if (cardType2 == 0) {
			if (cardType1 == 0) {
				return cardIndex1 < cardIndex2;
			}
			else {
				return true;
			}
		}

		if (cardIndex1 < cardIndex2) {
			return true;
		}
		if (cardIndex1 == cardIndex2) {
			return cardType1 < cardType2;
		}
		else {
			return false;
		}
	}
};
class HandCardsFlag
{
protected:
	uint8_t Flags[CARD_VALUE_LEN][4];
public:
	HandCardsFlag(bool createNewCards = true);//显示不进行任何的创建
	uint8_t	GetFlag(uint8_t cardIndex, uint8_t color);
	virtual ~HandCardsFlag();
	static uint8_t  CardValueToIndex(uint8_t cardIndex);
	static void CardValueToColorIndex(uint8_t cardValue, uint8_t * color, uint8_t * cardIndex);
	static uint8_t  CardColorIndexToValue(uint8_t color, uint8_t cardIndex);
	void RemoveChain(uint8_t startIndex, uint8_t endIndex, uint8_t count);
	void RemoveIndex(uint8_t cardIndex, uint8_t count);
	void RemoveCard(uint8_t color, uint8_t cardIndex);
	void RemoveCard(uint8_t cardValue); 
	void RemoveCard(const CardStyle& style);
	void AddCard(uint8_t color, uint8_t cardIndex);
	void AddCard(uint8_t cardValue);
	void AddCards(CardVector cardValues);
	//如果是false则全部置为0，否则置为全新的一副牌
	virtual void Reset(bool createNewCard);
	virtual size_t Size()const;
	std::string FlagString();
	CardVector ToCardValues()const;
	virtual CardVector AvailableBoom()const;
	virtual CardVector AvailableTriple()const;
	virtual CardVector AvailableDouble()const;
	virtual CardVector AvailableSingle()const;
	virtual CardVector AvailableChain(int len, int count)const;
	virtual CardVector AvailableChain(bool isBigger,uint8_t cardIndex,int len, int count)const;

	std::vector<CardRange> AvailableTripleChainRange()const;
	std::vector<CardRange> AvailableDoubleChainRange()const;
	std::vector<CardRange> AvailableSingleChainRange()const;
#pragma region inline function
	inline int Count(int cardIndex)const {
		return Flags[cardIndex][0] + Flags[cardIndex][1] + Flags[cardIndex][2] + Flags[cardIndex][3];
	}
	inline CardVector AvailableSingleChain()const {
		return AvailableChain(5, 1);
	}	
	inline CardVector AvailableDoubleChain() const {
		return AvailableChain(3, 2);
	}
	inline CardVector AvailableTripleChain()const {
		return AvailableChain(2, 3);
	}
	inline CardVector AvailableSingleChain(bool isBigger,uint8_t cardStartIndex,int len) const {
		return AvailableChain(isBigger,cardStartIndex, len, 1);
	}
	inline CardVector AvailableDoubleChain(bool isBigger, uint8_t cardStartIndex, int len) const {
		return AvailableChain(isBigger, cardStartIndex, len, 2);
	}
	inline CardVector AvailableTripleChain(bool isBigger, uint8_t cardStartIndex, int len)const {
		return AvailableChain(isBigger, cardStartIndex, len, 3);
	}
	inline uint8_t* Flag_Ptr() {return &Flags[0][0] ; }
#pragma endregion

};

