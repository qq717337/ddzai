#include "stdafx.h"
#include "HandCards.h"
#include <sstream>  
#include <iostream> 

HandCards::HandCards() :HandCardsFlag()
{
	UpdateByFlag();
}

HandCards::HandCards(const std::vector<uint8_t>& cardValues, bool sort) : HandCardsFlag(true)
{
	CardsSet = std::set<uint8_t, CardSetCompare>(cardValues.begin(), cardValues.end());

	CardCount = std::vector<uint8_t>(CARD_VALUE_LEN);
	int  j = 0;
	for (auto &v : cardValues) {
		if (v < 0x10) {
			int index = 12 + v;
			CardCount[index]++;
			Flags[index][0] = 1;
		}
		else {
			uint8_t cardType = (v & 0xf0) >> 4;
			uint8_t	cardValue = v & 0x0f;
			uint8_t	valueIndex = cardValue - 3;
			Flags[valueIndex][cardType - 1] = 1;
			CardCount[valueIndex]++;
		}
	}
}

bool HandCards::HasCard(uint8_t value)
{
	return CardsSet.find(value) != CardsSet.end();
}

void HandCards::UpdateByFlag()
{
	CardsSet.clear();
	int i, k = 0;
	for (i = 0; i < CARD_VALUE_LEN; i++) {
		CardCount[i] = 0;
	}
	for (i = 0; i <= CardIndex_2; i++) {
		int count = 0;
		for (k = 0; k < 4; k++) {
			if (Flags[i][k] == 1) {
				++count;
				CardsSet.insert(((k + 1) << 4) + i + 3);
			}
		}
		CardCount[i] = count;
	}
	if (Flags[CardIndex_SmallJoker][0] == 1) {
		CardsSet.insert(0x01);
	}
	CardCount[CardIndex_SmallJoker] = Flags[CardIndex_SmallJoker][0];

	if (Flags[CardIndex_LargeJoker][0] == 1) {
		CardsSet.insert(0x02);
	}
	CardCount[CardIndex_LargeJoker] = Flags[CardIndex_LargeJoker][0];
}

std::string HandCards::CardSetString()
{
	std::ostringstream ostr;
	std::cout.setf(std::ios::showbase);
	std::cout.setf(std::ios::hex, std::ios::basefield);
	ostr << "{";
	for (const auto &i : CardsSet) {
		ostr <<"0x"<<std::hex<< int(i) << ",";
	}
	std::string x = ostr.str();
	x.pop_back();
	x.push_back('}');
	return x;
}

void HandCards::RemoveJokerBoom(bool updateFlag) {
	RemoveIndex(CardIndex_SmallJoker, 1);
	RemoveIndex(CardIndex_LargeJoker, 1);
}

size_t HandCards::Size()
{
	int sum = 0;
	for (auto & i : CardCount) {
		sum += i;
	}
	return sum;
}

HandCards::~HandCards()
{
}
