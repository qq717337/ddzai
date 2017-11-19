#include "stdafx.h"
#include "HandCardsFlag.h"


HandCardsFlag::HandCardsFlag(bool createNewCard)
{
	Reset(createNewCard);
}

uint8_t HandCardsFlag::GetFlag(uint8_t cardIndex, uint8_t color)
{
	return Flags[cardIndex][color];
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
void HandCardsFlag::CardValueToColorIndex(uint8_t cardValue, uint8_t * color, uint8_t * index) {
	if (cardValue < 0x10) {
		*index = cardValue + CardIndex_2;
		*color = 0;
	}
	else {
		*index = (cardValue & 0xf) - 3;
		*color = ((cardValue & uint8_t(0xf0)) >> 4) - 1;
	}
}
uint8_t HandCardsFlag::CardColorIndexToValue(uint8_t color, uint8_t index) {
	if (index > CardIndex_2) {
		return index - CardIndex_2;
	}
	else {
		return ((color+1) <<4) | (index + 3);
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

void HandCardsFlag::RemoveIndex(uint8_t cardIndex, uint8_t count)
{
	int sum = 0;
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
void HandCardsFlag::RemoveCard(uint8_t cardValue)
{
	uint8_t index, color;

	CardValueToColorIndex(cardValue, &color, &index);
	_ASSERT(Flags[index][color] != 0);
	Flags[index][color] = 0;
}

void HandCardsFlag::RemoveCard(const CardStyle & style)
{
	switch (style.Style) {
	case ECardStyle::Single:
		RemoveIndex(style.StartValue, 1);
		break;
	case ECardStyle::Double:
		RemoveIndex(style.StartValue, 2);
		break;
	case ECardStyle::Triple_Zero:
		RemoveIndex(style.StartValue, 3);
		break;
	case ECardStyle::Triple_One:
		RemoveIndex(style.StartValue, 3);
		RemoveIndex(style.Extra[0], 1);
		break;
	case ECardStyle::Triple_Two:
		RemoveIndex(style.StartValue, 3);
		RemoveIndex(style.Extra[0], 2);
		break;
	case ECardStyle::Triple_Chain_Zero:
		RemoveChain(style.StartValue, style.EndValue, 3);
		break;
	case ECardStyle::Triple_Chain_One:
		RemoveChain(style.StartValue, style.EndValue, 3);
		for (auto v : style.Extra) {
			RemoveIndex(v, 1);
		}
		break;
	case ECardStyle::Triple_Chain_Two:
		RemoveChain(style.StartValue, style.EndValue, 3);
		for (auto v : style.Extra) {
			RemoveIndex(v, 2);
		}
		break;
	case ECardStyle::Single_Chain:
		RemoveChain(style.StartValue, style.EndValue, 1);
		break;
	case ECardStyle::Double_Chain:
		RemoveChain(style.StartValue, style.EndValue, 2);
		break;
	case ECardStyle::Boom:
		if (style.StartValue == CardIndex_JokerBoom) {
			RemoveIndex(CardIndex_SmallJoker, 1);
			RemoveIndex(CardIndex_LargeJoker, 1);
		}
		else {
			RemoveIndex(style.StartValue, 4);
		}
		break;
	}
}

void HandCardsFlag::RemoveCard(uint8_t color, uint8_t cardIndex)
{
	_ASSERT(Flags[cardIndex][color] != 0);
	Flags[cardIndex][color] = 0;
}

void HandCardsFlag::AddCard(uint8_t color, uint8_t cardIndex)
{
	_ASSERT(Flags[cardIndex][color] == 0);
	Flags[cardIndex][color] = 1;
}
void HandCardsFlag::AddCard(uint8_t cardValue)
{
	uint8_t index, color;
	CardValueToColorIndex(cardValue, &color, &index);

	_ASSERT_EXPR(Flags[index][color] == 0, " value is expect 0");
	Flags[index][color] = 1;
}

void HandCardsFlag::Reset(bool createNewCard)
{
	int i, j = 0;
	if (createNewCard) {
		for (i = 0; i < CARD_VALUE_LEN; i++) {
			for (j = 0; j < 4; j++) {
				Flags[i][j] = 1;
				Flags[i][j] = 1;
			}
		}
		for (i = CardIndex_SmallJoker; i < CARD_VALUE_LEN; i++) {
			for (j = 1; j < 4; j++) {
				Flags[i][j] = 0;
				Flags[i][j] = 0;
			}
		}
	}
	else {
		for (i = 0; i < CARD_VALUE_LEN; i++) {
			for (j = 0; j < 4; j++) {
				Flags[i][j] = 0;
				Flags[i][j] = 0;
			}
		}
	}
}

size_t HandCardsFlag::Size()const
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

CardVector HandCardsFlag::ToCardValues() const
{
	int i, j;
	CardVector r;
	for (i = 0; i < CARD_VALUE_LEN; i++) {
		for (j = 0; j < 4; j++) {
			if (Flags[i][j] == 1) {
				r.push_back(CardColorIndexToValue(j, i));
			}
		}
	}
	return r;
}

CardVector  HandCardsFlag::AvailableBoom()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (Count(i) == 4) {
			r.push_back(i);
		};
	}
	return r;
}

CardVector  HandCardsFlag::AvailableTriple()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (Count(i) >= 3) {
			r.push_back(i);
		};
	}
	return r;
}

CardVector  HandCardsFlag::AvailableDouble()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (Count(i) >= 2) {
			r.push_back(i);
		};
	}
	return r;
}

CardVector  HandCardsFlag::AvailableSingle()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; i++) {
		if (Count(i) >= 1) {
			r.push_back(i);
		};
	}
	return r;
}

CardVector  HandCardsFlag::AvailableChain(int len, int count)const
{
	_ASSERT(count <= 3);
	CardVector  validChain;
	CardVector  notEnoughIndexArray;//牌没有达到指定数目的下标
	int i, k;
	for (i = 0; i < CardIndex_2; ++i) {
		if (Count(i) < count) {
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

CardVector  HandCardsFlag::AvailableChain(bool isBigger, uint8_t cardIndex, int len, int count)const
{
	_ASSERT(count <= 3);
	CardVector  validChain;
	CardVector  notEnoughIndexArray;//牌没有达到指定数目的下标
	int i, k;
	for (i = 0; i < CardIndex_2; ++i) {
		if (Count(i) < count) {
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
				uint8_t startIndex = startCard + k;
				if (isBigger) {
					if (startIndex > cardIndex) {
						validChain.push_back(startIndex); //+3用于从牌3开始推算,不加代表按照下标返回
					}
				}
				else {
					if (startIndex < cardIndex) {
						validChain.push_back(startIndex); //+3用于从牌3开始推算,不加代表按照下标返回
					}
				}
			}
		}
		startCard = notEnoughIndexArray[i] + 1; //将从此处开始的下一张牌作为新的开始
	}
	return validChain;
}

std::vector<CardRange> HandCardsFlag::AvailableTripleChainRange()const {
	std::vector<CardRange> r;
	CardVector  chain = AvailableTripleChain();
	if (chain.size() > 0) {
		uint8_t head = chain[0];
		uint8_t tail = head;
		for (auto &v : chain) {
			if (v - tail > 1) {
				r.push_back(CardRange(head, tail + 1));
				head = v;
				tail = v;
			}
			if (v - tail == 1) {
				tail = v;
			}
		}
		r.push_back(CardRange(head, tail + 1));
	}
	return r;
}

std::vector<CardRange> HandCardsFlag::AvailableDoubleChainRange()const {
	std::vector<CardRange> r;
	CardVector  chain = AvailableDoubleChain();
	if (chain.size() > 0) {
		uint8_t head = chain[0];
		uint8_t tail = head;
		for (auto &v : chain) {
			if (v - tail > 1) {
				r.push_back(CardRange(head, tail + 2));
				head = v;
				tail = v;
			}
			if (v - tail == 1) {
				tail = v;
			}
		}
		r.push_back(CardRange(head, tail + 2));
	}
	return r;
}

std::vector<CardRange> HandCardsFlag::AvailableSingleChainRange() const {
	std::vector<CardRange> r;
	CardVector  chain = AvailableSingleChain();
	if (chain.size() > 0) {
		uint8_t head = chain[0];
		uint8_t tail = head;
		for (auto &v : chain) {
			if (v - tail > 1) {
				r.push_back(CardRange(head, tail + 4));
				head = v;
				tail = v;
			}
			if (v - tail == 1) {
				tail = v;
			}
		}
		r.push_back(CardRange(head, tail + 4));
	}
	return r;
}

HandCardsFlag::~HandCardsFlag()
{
	//for (int i = 0; i < CARD_VALUE_LEN; i++) {
	//delete[] Flags[i];
	//}
}
