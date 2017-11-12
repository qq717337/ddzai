#include "stdafx.h"
#include "HandCards.h"
#include <sstream>  
#include <iostream> 

HandCards::HandCards() :HandCardsFlag(false),
CardsSet(std::set<uint8_t, CardSetCompare>()),
CardCount(CardVector (CARD_VALUE_LEN))
{
	UpdateByFlag();
}

HandCards::HandCards(const CardVector & cardValues, bool updateSet) :
	HandCardsFlag(false),
	CardsSet(std::set<uint8_t, CardSetCompare>(cardValues.begin(), cardValues.end())),
	CardCount(CardVector (CARD_VALUE_LEN))
{
	int  j = 0;
	for (auto &v : cardValues) {
		if (v < 0x10) {
			int index = 12 + v;
			Flags[index][0] = 1;
		}
		else {
			uint8_t cardType = (v & 0xf0) >> 4;
			uint8_t	cardValue = v & 0x0f;
			uint8_t	valueIndex = cardValue - 3;
			Flags[valueIndex][cardType - 1] = 1;
		}
	}
	if (updateSet)
		UpdateByFlag();
}
HandCards::HandCards(const std::set<uint8_t, CardSetCompare>& cardValues, bool updateSet) :
	HandCardsFlag(false),
	CardsSet(cardValues),
	CardCount(CardVector (CARD_VALUE_LEN))
{
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
	if (updateSet)
		UpdateByFlag();
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

std::string HandCards::ToString()
{
	std::ostringstream ostr;
	ostr << "{";
	for (const auto &i : CardsSet) {
		ostr << "0x" << std::hex << int(i) << ",";
	}
	long pos = ostr.tellp();
	ostr.seekp(pos - 1);
	ostr << '}';
	return ostr.str();
}

void HandCards::Reset(bool createNewCard)
{
	HandCardsFlag::Reset(createNewCard);
	UpdateByFlag();
}

void HandCards::RemoveJokerBoom() {
	RemoveIndex(CardIndex_SmallJoker, 1);
	RemoveIndex(CardIndex_LargeJoker, 1);
}

size_t HandCards::Size()const
{
	int sum = 0;
	for (auto & i : CardCount) {
		sum += i;
	}
	return sum;
}

CardVector HandCards::AvailableBoom()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (CardCount[i] == 4) {
			r.push_back(i);
		}
	}
	return r;
}

CardVector HandCards::AvailableTriple()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (CardCount[i] >= 3) {
			r.push_back(i);
		}
	}
	return r;
}

CardVector  HandCards::AvailableDouble()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (CardCount[i] >= 2) {
			r.push_back(i);
		}
	}
	return r;
}
CardVector  HandCards::AvailableSingle()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (CardCount[i] >= 1) {
			r.push_back(i);
		}
	}
	return r;
}
CardVector  HandCards::AvailableBoom(bool bigger, uint8_t cardIndex)const
{
	CardVector r;
	for (uint8_t i = 0; i <= CardIndex_2; i++) {
		if (CardCount[i] == 4) {
			if (bigger) {
				if (i > cardIndex) r.push_back(i);
			}
			else {
				if (i < cardIndex) r.push_back(i);
			}
		}
	}
	if (CardCount[CardIndex_SmallJoker] == 1 && CardCount[CardIndex_LargeJoker] == 1) {
		r.push_back(CardIndex_JokerBoom);
	}
	return r;
}
CardVector  HandCards::AvailableTriple(bool bigger, uint8_t cardIndex)const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (CardCount[i] >= 3) {
			if (bigger) {
				if (i > cardIndex) r.push_back(i);
			}
			else {
				if (i < cardIndex) r.push_back(i);
			}
		}
	}
	return r;
}
CardVector  HandCards::AvailableDouble(bool bigger, uint8_t cardIndex)const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (CardCount[i] >= 2) {
			if (bigger) {
				if (i > cardIndex) r.push_back(i);
			}
			else {
				if (i < cardIndex) r.push_back(i);
			}
		}
	}
	return r;
}
CardVector  HandCards::AvailableSingle(bool bigger, uint8_t cardIndex)const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (CardCount[i] >= 1) {
			if (bigger) {
				if (i > cardIndex) r.push_back(i);
			}
			else {
				if (i < cardIndex) r.push_back(i);
			}
		}
	}
	return r;
}
CardVector  HandCards::AvailableChain(int len, int count)const
{
	_ASSERT(count <= 3);
	CardVector  validChain;
	CardVector  notEnoughIndexArray;//牌没有达到指定数目的下标
	int i, k;
	for (i = 0; i < CardIndex_2; ++i) {
		if (CardCount[i] < count) {
			notEnoughIndexArray.push_back(i);
		}
	}

	notEnoughIndexArray.push_back(CardIndex_2);     //牌2不能归入顺子里面
	notEnoughIndexArray.push_back(CardIndex_SmallJoker);//牌2不能归入顺子里面
	notEnoughIndexArray.push_back(CardIndex_LargeJoker);   //牌2不能归入顺子里面

	int startCard = 0;//得到第一个0点,下标为0，代表3这张牌
	for (i = 0; i < notEnoughIndexArray.size(); ++i) {
		int	diff = notEnoughIndexArray[i] - startCard - len;
		if (diff >= 0) { //第一个出现无牌的下标可以让从3开始组成顺子
			for (k = 0; k <= diff; ++k) {
				validChain.push_back(startCard + k); //+3用于从牌3开始推算,不加代表按照下标返回
			}
		}
		startCard = notEnoughIndexArray[i] + 1; //将从此处开始的下一张牌作为新的开始
	}
	return validChain;
}

CardVector  HandCards::AvailableChain(int len, int count, bool bigger, uint8_t cardIndex)const
{
	CardVector  validChain;
	auto allChain = AvailableChain(len, count);
	for (auto & chain : allChain) {
		if (bigger) {
			if (chain > cardIndex) {
				validChain.push_back(chain);
			}
		}
		else {
			if (chain < cardIndex) {
				validChain.push_back(chain);
			}
		}
	}
	return validChain;
}

std::vector<CardVector > HandCards::IsolateCards(bool sub)
{
	CardVector ();
	std::unordered_set<uint8_t> notIsolateCards;
	auto singleChain = AvailableSingleChain();
	int i;
	for (auto &v : singleChain) {
		for (i = 0; i < 5; i++) {
			notIsolateCards.insert(v + i);
		}
	}
	auto doubleChain = AvailableDoubleChain();
	for (auto & v : doubleChain) {
		for (i = 0; i < 3; i++) {
			notIsolateCards.insert(v + i);
		}
	}
	auto  tripleChain = AvailableTripleChain();
	for (auto &v : tripleChain) {
		for (i = 0; i < 2; i++) {
			notIsolateCards.insert(v + i);
		}
	}
	std::vector<CardVector > isolateCards(15);

	for (uint8_t i = 0; i < CARD_VALUE_LEN; ++i) {
		if (notIsolateCards.find(i) != notIsolateCards.end()) { //非孤立元素包含当前index
			isolateCards[i] = CardVector (4);
		}
		else {
			isolateCards[i] = { Flags[i][0],  Flags[i][1],  Flags[i][2],  Flags[i][3] };
		}
	}
	if (sub) {
		for (i = 0; i < CARD_VALUE_LEN; ++i) {
			for (int k = 0; k < 4; ++k) {
				Flags[i][k] = Flags[i][k] - isolateCards[i][k];
			}
		}

		UpdateByFlag();
	}
	return isolateCards;
}

bool HandCards::CanTake(const CardStyle & lastStyle)const
{
	auto booms = AvailableBoom();
	if (booms.size() > 0 && lastStyle.Style != ECardStyle::Boom) {
		return true;
	}
	switch (lastStyle.Style)
	{
	case ECardStyle::Boom: {
		auto booms = AvailableBoom(true, lastStyle.StartValue);
		return booms.size() > 0;
		break;
	}
	case ECardStyle::Triple_One: {
		auto triples = AvailableTriple(true, lastStyle.StartValue);
		return triples.size() > 0;
		break;
	}
	case ECardStyle::Triple_Two: {
		auto triples = AvailableTriple(true, lastStyle.StartValue);
		return triples.size() > 0;
		break;
	}
	case ECardStyle::Double: {
		auto doubles = AvailableDouble(true, lastStyle.StartValue);
		return doubles.size() > 0;
		break;
	}
	case ECardStyle::Single: {
		auto singles = AvailableSingle(true, lastStyle.StartValue);
		return singles.size() > 0;
		break;
	}
	case ECardStyle::Single_Chain: {
		auto singleChains = AvailableSingleChain(true, lastStyle.StartValue, lastStyle.Length());
		return singleChains.size() > 0;
		break;
	}
	case ECardStyle::Double_Chain: {
		auto singleChains = AvailableDoubleChain(true, lastStyle.StartValue, lastStyle.Length());
		return singleChains.size() > 0;
		break;
	}
	case ECardStyle::Triple_Chain_Zero: {
		auto singleChains = AvailableTripleChain(true, lastStyle.StartValue, lastStyle.Length());
		return singleChains.size() > 0;
		break;
	}
	case ECardStyle::Triple_Chain_One: {
		auto singleChains = AvailableTripleChain(true, lastStyle.StartValue, lastStyle.Length());
		return singleChains.size() > 0 && Size() > lastStyle.Length() * 4;
		break;
	}
	case ECardStyle::Triple_Chain_Two: {
		auto singleChains = AvailableTripleChain(true, lastStyle.StartValue, lastStyle.Length());
		return singleChains.size() > 0 && Size() > lastStyle.Length() * 5;
		break;
	}
	default:
		break;
	}
	return false;
}

CardVector  HandCards::GetCardsValue(uint8_t cardIndex, int count)
{
	 CardVector  r;
	 int sum = 0;
	 for (int i = 0; i < 4; ++i) {
		 if (Flags[cardIndex][i] == 1) {
			 r.push_back(CardColorIndexToValue(i, cardIndex));
			 sum++;
		 }
		 else {
			 continue;
		 }
		 if (sum == count)break;
	 }
	_ASSERT(sum == count);
	 return r;
}

HandCards::~HandCards()
{
}
