#pragma once
#include "HandCardsFlag.h"
#include <set>
#include <string>
struct CardSetCompare
{
	bool operator()(const uint8_t& lhs, const  uint8_t& rhs)const
	{
		uint8_t cardType1 = lhs & 0xf0;
		uint8_t cardValue1 = lhs & 0x0f;
		uint8_t cardType2 = rhs & 0xf0;
		uint8_t cardValue2 = rhs & 0x0f;
		if (cardType1 == 0) {
			if (cardType2 == 0) {
				return cardValue1 < cardValue2;
			}
			else {
				return false;
			}
		}
		if (cardType2 == 0) {
			if (cardType1 == 0) {
				return cardValue1 < cardValue2;
			}
			else {
				return true;
			}
		}

		if (cardValue1 < cardValue2) {
			return true;
		}
		if (cardValue1 == cardValue2) {
			return cardType1 < cardType2;
		}
		else {
			return false;
		}
	}
};
class HandCards :
	public HandCardsFlag
{
private:
	std::vector<uint8_t>CardCount;
	std::set<uint8_t, CardSetCompare> CardsSet;
public:
	HandCards();
	HandCards(const  std::vector<uint8_t>&  cardValues, bool sort = false);
	bool HasCard(uint8_t value);
	void RemoveJokerBoom(bool updateFlag);
	virtual void UpdateByFlag();
	std::string CardSetString();
	virtual size_t Size() override;
	~HandCards();
};

