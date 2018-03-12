#include "stdafx.h"
#include "CardSet.h"
#include <iostream>
#include <algorithm>
CardSet::CardSet() :player_num(3), PlayerCardSet(3)
{
	for (int i = 0; i < player_num; ++i) {
		PlayerCardSet[i] = new HandCards();
	}
	DeskCardSet.Reset(true);
}

CardSet::~CardSet()
{
	for (auto v : PlayerCardSet) {
		if (v != nullptr)	delete v;
	}
}

uint8_t* CardSet::NewCards()
{
	uint8_t* r = new uint8_t[53];

	for (int c = 1; c < 5; c++) {
		for (int i = 3; i < 16; i++) {
			r[(c - 1) * 13 + (i - 3)] = (16 * c + i);
		}
	}
	r[52] = 1;
	r[53] = 2;
	return r;
}

void CardSet::ResetPlayerCard(int randomDealCount) {
	DeskCardSet.Reset(true);
	for (auto & v : PlayerCardSet) {
		v->Reset(false);
	}
	uint8_t* newCards = NewCards();
	extern std::default_random_engine DefaultRandomEngine;
	std::shuffle(newCards, newCards + 54, DefaultRandomEngine);

	int i, index = 0;
	for (i = 0; i < player_num; ++i) {
		for (int j = 0; j < randomDealCount; ++j) {
			DealCard(i, newCards[index]);
			++index;
		}
	}
}

void CardSet::DealIndex(int playerIndex, uint8_t cardIndex, int count)
{
	int dealCount = 0;
	for (int i = 0; i < 4; ++i) {
		if (dealCount == count) {
			break;
		}
		if (DeskCardSet.GetFlag(cardIndex, i) == 0) {
			continue;
		}
		else {
			DeskCardSet.RemoveCard(i, cardIndex);
			PlayerCardSet[playerIndex]->AddCard(i, cardIndex);
			++dealCount;
		}
	}

    _ASSERT(dealCount == count);
}

void CardSet::DealCard(int playerIndex, uint8_t cardValue)
{
	uint8_t color, index;
	HandCardsFlag::CardValueToColorIndex(cardValue, &color, &index);
	DeskCardSet.RemoveCard(color, index);
	PlayerCardSet[playerIndex]->AddCard(cardValue);
}

size_t CardSet::LeftCount(int playerId)
{
	return (54 - 3) / player_num - PlayerCardSet[playerId]->Size();
}
//∑µªÿ £”‡µƒ3’≈≈∆
void CardSet::RandomFillLeft()
{
	Update();
	for (uint8_t i = 0; i <= CardIndex_2; i++) {
		int count = 0;
		for (uint8_t k = 0; k < 4; k++) {
			if (DeskCardSet.GetFlag(i, k) == 1) {
				ExtraCard.AddCard(((k + 1) << 4) + i + 3);
			}
		}
	}
	if (DeskCardSet.GetFlag(CardIndex_SmallJoker, 0) == 1) {
		ExtraCard.AddCard(0x01);
	}
	if (DeskCardSet.GetFlag(CardIndex_LargeJoker, 0) == 1) {
		ExtraCard.AddCard(0x02);
	}
	ExtraCard.UpdateByFlag();
	extern std::default_random_engine DefaultRandomEngine;
	auto newCard = std::vector<uint8_t>(ExtraCard.Data().begin(), ExtraCard.Data().end());
	std::shuffle(newCard.begin(), newCard.end(), DefaultRandomEngine);
	auto iter_pos = newCard.begin();
	for (int i = 0; i < player_num; ++i) {
		size_t leftCount = LeftCount(i);
		for (int j = 0; j < leftCount; ++j) {
			PlayerCardSet[i]->AddCard(*iter_pos);
			ExtraCard.RemoveCard(*iter_pos);
			++iter_pos;
		}
	}
	Update();
}

std::string CardSet::ToString()
{
	std::ostringstream ostr;
	std::cout.setf(std::ios::showbase);
	std::cout.setf(std::ios::hex, std::ios::basefield);
	for (int i = 0; i < player_num; ++i) {
		ostr << "Player" << i << " = ";
		auto player = PlayerCardSet[i]->ToString();
		ostr << player << "\n\t";
	}
	if (ExtraCard.Size() == 3) {
		auto cardValues = ExtraCard.ToCardValues();
		ostr << "Extra ={0x" << std::hex << int(cardValues[0]) << ",0x" << int(cardValues[1]) << ",0x" << int(cardValues[2]) << "}";
	}
	return ostr.str();
}
std::string CardSet::ToString2()
{
	std::ostringstream ostr;
	std::cout.setf(std::ios::showbase);
	std::cout.setf(std::ios::hex, std::ios::basefield);
	for (int i = 0; i < player_num; ++i) {
		auto player = PlayerCardSet[i]->ToString();
		ostr << player << "#";
	}
	if (ExtraCard.Size() == 3) {
		auto cardValues = ExtraCard.ToCardValues();
		ostr << "{" << std::hex << int(cardValues[0]) << "," << int(cardValues[1]) << "," << int(cardValues[2]) << "}";
	}
	return ostr.str();
}
void CardSet::DealChain(int playerIndex, uint8_t startIndex, uint8_t endIndex, int count)
{
	for (int i = startIndex; i <= endIndex; ++i) {
		DealIndex(playerIndex, i, count);
	}
}

void CardSet::Update()
{
	for (auto i : PlayerCardSet) {
		i->UpdateByFlag();
	}
	ExtraCard.UpdateByFlag();
}
