#include "stdafx.h"
#include "HandCardsFlag.h"

HandCardsFlag::HandCardsFlag()
{
	Flags = new uint8_t*[CARD_VALUE_LEN];
	uint8_t i;
	for (i = 0; i < CARD_VALUE_LEN; i++) {
		Flags[i] = new uint8_t[4]{ 1,1,1,1 };
	}
	for (i = 1; i < 4; i++) {
		Flags[CardIndex_SmallJoker][i] = 0;
	}
}

HandCardsFlag::HandCardsFlag(bool x)
{
	Flags = new uint8_t*[CARD_VALUE_LEN];
	uint8_t i;
	for (i = 0; i < CARD_VALUE_LEN; i++) {
		Flags[i] = new uint8_t[4]{ 0,0,0,0 };
	}
}

uint8_t HandCardsFlag::GetFlag(uint8_t color, uint8_t index)
{
	return Flags[index][color];
}

uint8_t HandCardsFlag::CardValueToIndex(uint8_t cardValue) {
	if (cardValue < 0x10) {
		return cardValue + CardIndex_2;
	}
	else { 
		uint8_t r = (cardValue & 0xf) - 3;
		return r;
	}
}

uint8_t HandCardsFlag::CardColorIndexToValue(uint8_t color, uint8_t index) {
	if (color == 0 || index < 0x10) {
		return index - CardIndex_2;
	}
	else {
		return color * 0x10 + (index + 3);
	}
}

void HandCardsFlag::RemoveChain(uint8_t startIndex, uint8_t endIndex, uint8_t count)
{
	for (int cardIndex = startIndex; cardIndex <= endIndex; cardIndex++) {
		int	sum = 0;
		for (int i = 0; i < 4; i++) {
			if (sum == count) {
				break;
			}
			if (Flags[cardIndex][i] == 1) {
				sum++;
				Flags[cardIndex][i] = 0;
			}
		}
		
			_ASSERT(sum == count);
	}
}

void HandCardsFlag::RemoveIndex(uint8_t index, uint8_t count)
{
	int sum = 0;
	for (int i = 0; i < 4; i++) {
		if (sum == count) {
			break;
		}
		if (Flags[index][i] == 1) {
			sum++;
			Flags[index][i] = 0;
		}
	}
	_ASSERT(sum == count);
}

size_t HandCardsFlag::Size()
{
	int sum = 0;
	for (int i = 0; i < CARD_VALUE_LEN; i++) {
		for (int j = 0; j < 4; j++) {
			sum += Flags[i][j];
		}
	}
	return sum;
}



std::string HandCardsFlag::FlagString()
{
	std::string sr;

	for (int i = 0; i < CARD_VALUE_LEN; i++) {
		for (int j = 0; j < 4; j++) {
			sr.append("[ ");
			sr.push_back(i + '0');
			sr.push_back(',');
			sr.push_back(j + '0');
			sr.push_back('=');
			sr.push_back(Flags[i][j] + '0');

			sr.append(" ]");
		}
		sr.append("\n");
	}
	return sr;
}


HandCardsFlag::~HandCardsFlag()
{
	for (int i = 0; i < CARD_VALUE_LEN; i++) {
		delete Flags[i];
	}
	delete Flags;
}
