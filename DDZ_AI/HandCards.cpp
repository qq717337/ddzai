#include "stdafx.h"
#include "HandCards.h"
#include <sstream>  
#include <iostream> 
#include <functional>

HandCards::HandCards() :HandCardsFlag(false),
CardsSet(std::set<uint8_t, CardSetCompare>()),
CardCount(CardVector(CARD_VALUE_LEN))
{
	UpdateByFlag();
}

HandCards::HandCards(HandCardsFlag & handCardsFlag)
{
	std::memcpy(&Flags[0][0], handCardsFlag.Flag_Ptr(), CARD_VALUE_LEN * 4 * sizeof(uint8_t));
	UpdateByFlag();
}

HandCards::HandCards(const CardVector & cardValues, bool updateSet) :
	HandCardsFlag(false),
	CardsSet(std::set<uint8_t, CardSetCompare>(cardValues.begin(), cardValues.end())),
	CardCount(CardVector(CARD_VALUE_LEN))
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
	CardCount(CardVector(CARD_VALUE_LEN))
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

std::string HandCards::ToString()const
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

void HandCards::Reset(const CardVector & cardValues)
{
	Reset(false);

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
	for (uint8_t i = 0; i <= CardIndex_2; ++i) {
		if (CardCount[i] == 4) {
			r.push_back(i);
		}
	}
	if (CardCount[CardIndex_SmallJoker] + CardCount[CardIndex_LargeJoker] == 2) {
		r.push_back(CardIndex_JokerBoom);
	}
	return r;
}

CardVector HandCards::AvailableTriple()const
{
	CardVector r;
	for (uint8_t i = 0; i <= CardIndex_2; ++i) {
		if (CardCount[i] >= 3) {
			r.push_back(i);
		}
	}
	return r;
}

CardVector  HandCards::AvailableDouble()const
{
	CardVector r;
	for (uint8_t i = 0; i <= CardIndex_2; ++i) {
		if (CardCount[i] >= 2) {
			r.push_back(i);
		}
	}
	return r;
}
CardVector  HandCards::AvailableSingle()const
{
	CardVector r;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; ++i) {
		if (CardCount[i] >= 1) {
			r.push_back(i);
		}
	}
	return r;
}
CardVector HandCards::PerfectTriple(int startIndex) const
{
	CardVector r;
	for (uint8_t i = startIndex + 1; i <= CardIndex_2; ++i) {
		if (CardCount[i] == 3) {
			r.push_back(i);
		}
	}
	return r;
}
CardVector HandCards::PerfectDouble(int startIndex) const
{
	CardVector r;
	for (uint8_t i = startIndex + 1; i <= CardIndex_2; ++i) {
		if (CardCount[i] == 2) {
			r.push_back(i);
		}
	}
	return r;
}
CardVector HandCards::PerfectSingle(int startIndex) const
{
	CardVector r;
	for (uint8_t i = startIndex + 1; i < CARD_VALUE_LEN; ++i) {
		if (CardCount[i] == 1) {
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

std::vector<CardVector> HandCards::FindIsolateCards()const
{
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
			isolateCards[i] = CardVector(4);
		}
		else {
			isolateCards[i] = { Flags[i][0],  Flags[i][1],  Flags[i][2],  Flags[i][3] };
		}
	}
	return isolateCards;
}
CardVector HandCards::FindIsolateCardsIndex() const
{
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
	CardVector isolateCardsIndex;
	for (uint8_t i = 0; i < CARD_VALUE_LEN; ++i) {
		if (notIsolateCards.find(i) != notIsolateCards.end()) { //非孤立元素包含当前index
			isolateCardsIndex.push_back(i);
		}
	}
	return isolateCardsIndex;
}
std::vector<CardVector> HandCards::SubIsolateCards()
{
	auto isolateCards = FindIsolateCards();
	for (uint8_t i = 0; i < CARD_VALUE_LEN; ++i) {
		for (uint8_t k = 0; k < 4; ++k) {
			Flags[i][k] = Flags[i][k] - isolateCards[i][k];
		}
	}

	UpdateByFlag();
	return isolateCards;
}
static CardRange GetLongestChain(std::function<std::vector<CardRange>(void)> AvailableChainRangeFunc)
{
	auto m_singleChains = AvailableChainRangeFunc();
	if (m_singleChains.empty()) {
		return CardRange(-1, -1);
	}
	int max_index = -1;
	int max_v = 0;
	for (int i = 0; i < m_singleChains.size(); ++i) {
		CardRange v = m_singleChains[i];
		if (v.Length() > max_v) {
			max_v = v.Length();
			max_index = i;
		}
	}
	return m_singleChains[max_index];
}
std::vector<CardStyle> HandCards::FindLeastCardPlay(bool useLaiZi)const
{
	auto inputList = CardVector(CardsSet.begin(), CardsSet.end());
	if (useLaiZi) {
		inputList.push_back(0x50);
		uint8_t index = 0;
		auto desStyle = CardStyle::FromCardsValueWithLaizi(inputList, index, ECardStyle::Invalid);
		if (desStyle.Valid())
			return { desStyle };
	}
	else {
		auto desStyle = CardStyle::FromCardsValue(inputList);
		if (desStyle.Valid())
			return { desStyle };
	}
	std::vector<CardStyle> r;

	auto _singleChain = AvailableSingleChainRange(useLaiZi);
	auto _doubleChain = AvailableDoubleChainRange(useLaiZi);
	auto _tripleChain = AvailableTripleChainRange(useLaiZi);

	auto _booms = AvailableBoom(); 
	std::remove_if(_tripleChain.begin(), _tripleChain.end(), [_booms](CardRange& x)->bool {
		for (auto b : _booms) {
			if (b >= x.Start && b <= x.End) {
				return true;
			}
		}
		return false;
	});
	auto _isolateIndex = FindIsolateCardsIndex();
	CardVector _isolate_triple;
	CardVector _isolate_double;
	CardVector _isolate_single;
	for (auto v : _isolateIndex) {
		switch (CardCount[v])
		{
		case 1:
			_isolate_single.push_back(v);
			break;
		case 2:
			_isolate_double.push_back(v);
			break;
		case 3:
			_isolate_triple.push_back(v);
			break;
		}
	}
	auto _triple = PerfectTriple(CardIndex_3 - 1);
	if (_isolate_triple.empty()) {
		_isolate_triple.insert(_isolate_triple.end(), _triple.begin(), _triple.end());
	}
	if (_singleChain.size() > 0)
		r.emplace_back(CardStyle::SingleChainStyle(_singleChain[0].Start, _singleChain[0].End));
	if (_doubleChain.size() > 0)
		r.emplace_back(CardStyle::DoubleChainStyle(_doubleChain[0].Start, _doubleChain[0].End));
	if (_tripleChain.size() > 0)
		r.emplace_back(CardStyle::TripleChainZeroStyle(_tripleChain[0].Start, _tripleChain[0].End));

	if (_isolate_single.size() > 0)
	{
		for (auto v : _isolate_triple)
			r.emplace_back(CardStyle::TripleOneStyle(v, { _isolate_single[0] }));
	}
	if (_isolate_double.size() > 0)
	{
		for (auto v : _isolate_triple)
			r.emplace_back(CardStyle::TripleTwoStyle(v, { _isolate_double[0] }));
	}
	if (_isolate_double.size() + _isolate_single.size() == 0)
	{
		for (auto v : _isolate_triple)
			r.emplace_back(CardStyle::TripleZeroStyle(v));
	}

	if (!(_isolate_triple.size() == 1 && _isolate_single.size() == 1))
	{
		for (auto v : _isolate_single)
			r.emplace_back(CardStyle::SingleStyle(v));
	}
	if (!(_isolate_triple.size() == 1 && _isolate_double.size() == 1))
	{
		for (auto v : _isolate_double)
			r.emplace_back(CardStyle::DoubleStyle(v));
	}
	if (r.size() == 0 && _booms.size() > 0)
	{
		r.emplace_back(CardStyle::BoomStyle(_booms[0]));
	}

	std::stable_sort(r.begin(), r.end(), [](const CardStyle a, const CardStyle b)->bool {
		if (a.Style == ECardStyle::Boom) {
			if (b.Style == ECardStyle::Boom) {
				return  a.StartValue < b.StartValue;
			}
			else {
				return false;
			}
		}
		if (b.Style == ECardStyle::Boom) {
			if (a.Style == ECardStyle::Boom) {
				return a.StartValue < b.StartValue;
			}
			else {
				return true;
			}
		}
#undef min
		if (a.Style == b.Style && a.StartValue == b.StartValue && a.EndValue == b.EndValue) {
			if (a.Extra.size() > 0) {
				auto x = std::min(a.Extra.begin(), a.Extra.end());
				auto y = std::min(b.Extra.begin(), b.Extra.end());
				return x < y;
			}
		}

		if (a.StartValue < b.StartValue) {
			return true;
		}else if (a.StartValue == b.StartValue) {
			auto x = a.GetCardsCount();
			auto y = b.GetCardsCount();//牌起始值相等的时候，将长度大的排到上面
			return x < y;
		}else {
			return false;
		}

	});
	if (useLaiZi) {
		for (int i = 0; i < r.size(); ++i) {
			auto v = r[i];
			if (v.GetCardsCount() == CardsSet.size())
			{
				switch (v.Style)
				{
				case ECardStyle::Single:
				{
					if (v.StartValue >= CardIndex_SmallJoker)
					{
						r[i] = CardStyle::JokerBoom;
					}
					else
					{
						r[i] = CardStyle::DoubleStyle(v.StartValue);
					}
					break;
				}
				case ECardStyle::Double:
				{
					r[i] = CardStyle::TripleZeroStyle(v.StartValue);
					break;
				}
				case ECardStyle::Triple_Zero:
				{
					r[i] = CardStyle::BoomStyle(v.StartValue);
					break;
				}
				case ECardStyle::Boom:
					break;
				}
			}
		}
	}
	return r;
}
std::vector<CardStyle> HandCards::LongestStyle(bool useLaiZi)const
{
	std::vector<CardStyle> r;
	auto _tripleChain = AvailableTripleChainRange(useLaiZi);
	auto _doubleChain = AvailableDoubleChainRange(useLaiZi);
	auto _singleChain = AvailableSingleChainRange(useLaiZi);
	auto _booms = AvailableBoom();
	auto _triple = PerfectTriple(CardIndex_3 - 1);
	auto _double = PerfectDouble(CardIndex_3 - 1);
	auto _single = PerfectSingle(CardIndex_3 - 1);
	auto tripleLen = _triple.size();
	auto doubleLen = _double.size();
	auto singleLen = _single.size();
	auto tripleChainLen = _tripleChain.size();
	auto doubleChainLen = _doubleChain.size();
	auto singleChainLen = _singleChain.size();
	int minIndex = CardIndex_LargeJoker + 1;
	if (tripleChainLen > 0)
	{
		minIndex = _tripleChain[0].Start;
		tripleChainLen = _tripleChain[0].Length();
		bool hasTripleChain = false;
		if (singleLen >= tripleChainLen)
		{
			hasTripleChain = true;
			CardVector headSingle(_single.begin(), _single.begin() + tripleChainLen);
			r.push_back(CardStyle::TripleChainOneStyle(_tripleChain[0].Start, _tripleChain[0].End, headSingle));
		}
		if (doubleLen >= tripleChainLen)
		{
			hasTripleChain = true;
			CardVector headDouble(_double.begin(), _double.begin() + tripleChainLen);
			r.push_back(CardStyle::TripleChainTwoStyle(_tripleChain[0].Start, _tripleChain[0].End, headDouble));
		}
		if (!hasTripleChain)
		{
			r.push_back(CardStyle::TripleChainZeroStyle(_tripleChain[0].Start, _tripleChain[0].End));
		}
	}
	if (doubleChainLen > 0)
	{
		r.push_back(CardStyle::DoubleChainStyle(_doubleChain[0].Start, _doubleChain[0].End));
	}
	if (singleChainLen > 0)
	{
		r.push_back(CardStyle::SingleChainStyle(_singleChain[0].Start, _singleChain[0].End));
	}
	if (tripleLen > 0)
	{
		for (auto t : _triple)
		{
			if (_tripleChain.size() > 0)
			{
				if (t >= _tripleChain[0].Start && t <= _tripleChain[0].End)
				{
					continue;
				}
			}
			bool hasTriple = false;
			if (singleLen >= 1)
			{
				hasTriple = true;
				CardVector headSingle(_single.begin(), ++_single.begin());
				r.push_back(CardStyle::TripleOneStyle(t, headSingle));
			}
			if (doubleLen >= 1)
			{
				hasTriple = true;
				CardVector headDouble(_double.begin(), ++_double.begin());
				r.push_back(CardStyle::TripleTwoStyle(t, headDouble));
			}
			if (!hasTriple)
			{
				r.push_back(CardStyle::TripleZeroStyle(t));
			}
		}
	}
	if (r.size() == 0)
	{
		_double.insert(_double.end(), _single.begin(), _single.end());
		std::sort(_double.begin(), _double.end());
		for (auto v : _double)
		{
			if (CardCount[v] == 1)
			{
				r.push_back(CardStyle::SingleStyle(v));
			}
			if (CardCount[v] == 2)
			{
				r.push_back(CardStyle::DoubleStyle(v));
			}
		}
	}
	if (r.size() == 0)
	{
		for (auto v : _booms)
		{
			r.push_back(CardStyle::BoomStyle(v));
		}
	}


	std::sort(r.begin(), r.end(), [](const CardStyle& x, const CardStyle& y) {return x.StartValue < y.StartValue; });
	return r;
}

std::vector< CardStyle> HandCards::MinValueStyle(bool useLaiZi)const
{
	std::vector< CardStyle> r;
	auto inputList = CardVector(CardsSet.begin(), CardsSet.end());
	if (useLaiZi) {
		inputList.push_back(0x50);
		uint8_t index = 0;
		auto desStyle = CardStyle::FromCardsValueWithLaizi(inputList, index, ECardStyle::Invalid);

		if (desStyle.Valid()) {
			r.emplace_back(desStyle);
			return r;
		}
	}
	else {
		auto desStyle = CardStyle::FromCardsValue(inputList);

		if (desStyle.Valid()) {
			r.emplace_back(desStyle);
			return r;
		}
	}

	auto _booms = AvailableBoom();
	auto _tripleChain = AvailableTripleChainRange();
	auto _doubleChain = AvailableDoubleChainRange();
	auto _singleChain = AvailableSingleChainRange();
	constexpr int before3Index = CardIndex_3 - 1;
	auto _triple = PerfectTriple(before3Index);
	auto _double = PerfectDouble(before3Index);
	auto _single = PerfectSingle(before3Index);
	auto tripleLen = _triple.size();
	auto doubleLen = _double.size();
	auto singleLen = _single.size();
	auto tripleChainLen = _tripleChain.size();
	auto doubleChainLen = _doubleChain.size();
	auto singleChainLen = _singleChain.size();

	uint8_t minStartIndex = CardIndex_LargeJoker;

	if (tripleChainLen > 0 && _tripleChain[0].Start < minStartIndex) {
		minStartIndex = _tripleChain[0].Start;
		r.emplace_back(CardStyle::TripleChainZeroStyle(_tripleChain[0].Start, _tripleChain[0].End));
	}
	if (doubleChainLen > 0 && _doubleChain[0].Start < minStartIndex) {
		minStartIndex = _doubleChain[0].Start;
		r.emplace_back(CardStyle::DoubleChainStyle(_doubleChain[0].Start, _doubleChain[0].End));
	}
	if (singleChainLen > 0 && _singleChain[0].Start < minStartIndex) {
		minStartIndex = _singleChain[0].Start;
		r.emplace_back(CardStyle::SingleChainStyle(_singleChain[0].Start, _singleChain[0].End));
	}
	if (tripleLen > 0 && _triple[0] < minStartIndex) {
		minStartIndex = _triple[0];
		r.emplace_back(CardStyle::TripleZeroStyle(_triple[0]));
	}
	if (doubleLen > 0 && _double[0] < minStartIndex) {
		minStartIndex = _double[0];
		r.emplace_back(CardStyle::DoubleStyle(_double[0]));
	}
	if (singleLen > 0 && _single[0] < minStartIndex) {
		minStartIndex = _single[0];
		r.emplace_back(CardStyle::SingleStyle(_single[0]));
	}
	if (!r.empty() && _booms.size() > 0) {
		r.emplace_back(CardStyle::BoomStyle(_booms[0]));
	}

	if (useLaiZi)
	{
		for (int i = 0; i < r.size(); ++i)
		{
			auto& v = r[i];
			if (v.GetCardsCount() == CardsSet.size())
			{
				switch (v.Style)
				{
				case ECardStyle::Single:
				{
					if (v.StartValue >= CardIndex_SmallJoker)
					{
						r[i] = CardStyle::JokerBoom;
					}
					else
					{
						r[i] = CardStyle::DoubleStyle(v.StartValue);
					}
					break;
				}
				case ECardStyle::Double:
				{
					r[i] = CardStyle::TripleZeroStyle(v.StartValue);
					break;
				}
				case ECardStyle::Triple_Zero:
				{
					r[i] = CardStyle::BoomStyle(v.StartValue);
					break;
				}
				case ECardStyle::Boom:
					break;
				}
			}
		}
	}

	return r;
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
		return triples.size() > 0 && Size() >= 4;
		break;
	}
	case ECardStyle::Triple_Two: {
		auto triples = AvailableTriple(true, lastStyle.StartValue);
		return triples.size() > 0 && Size() >= 5;
		break;
	}
	case ECardStyle::Triple_Zero: {
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

bool HandCards::CanTake(const CardStyle & lastStyle, CardStyle &retStyle)const
{
	auto booms = AvailableBoom();
	if (booms.size() > 0 && lastStyle.Style != ECardStyle::Boom) {
		return true;
	}
	switch (lastStyle.Style)
	{
	case ECardStyle::Boom: {
		auto booms = AvailableBoom(true, lastStyle.StartValue);
		if (booms.size() > 0) {
			retStyle = CardStyle::BoomStyle(booms[0]);
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Triple_One: {
		auto triples = AvailableTriple(true, lastStyle.StartValue);
		if (triples.size() > 0 && Size() >= 4) {
			retStyle = CardStyle::TripleOneStyle(triples[0], {});
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Triple_Two: {
		auto triples = AvailableTriple(true, lastStyle.StartValue);
		if (triples.size() > 0 && Size() >= 5) {
			retStyle = CardStyle::TripleTwoStyle(triples[0], {});
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Triple_Zero: {
		auto triples = AvailableTriple(true, lastStyle.StartValue);
		if (triples.size() > 0) {
			retStyle = CardStyle::TripleZeroStyle(triples[0]);
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Double: {
		auto doubles = AvailableDouble(true, lastStyle.StartValue);
		if (doubles.size() > 0) {
			retStyle = CardStyle::DoubleStyle(doubles[0]);
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Single: {
		auto singles = AvailableSingle(true, lastStyle.StartValue);
		if (singles.size() > 0) {
			retStyle = CardStyle::SingleStyle(singles[0]);
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Single_Chain: {
		auto singleChains = AvailableSingleChain(true, lastStyle.StartValue, lastStyle.Length());
		if (singleChains.size() > 0) {
			retStyle = CardStyle::SingleChainStyle(singleChains[0], singleChains[0] + lastStyle.Length() - 1);
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Double_Chain: {
		auto doubleChains = AvailableDoubleChain(true, lastStyle.StartValue, lastStyle.Length());
		if (doubleChains.size() > 0) {
			retStyle = CardStyle::DoubleChainStyle(doubleChains[0], doubleChains[0] + lastStyle.Length() - 1);
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Triple_Chain_Zero: {
		auto tripleChains = AvailableTripleChain(true, lastStyle.StartValue, lastStyle.Length());

		if (tripleChains.size() > 0) {
			retStyle = CardStyle::TripleChainZeroStyle(tripleChains[0], tripleChains[0] + lastStyle.Length() - 1);
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Triple_Chain_One: {
		auto tripleChains = AvailableTripleChain(true, lastStyle.StartValue, lastStyle.Length());
		if (tripleChains.size() > 0 && Size() > lastStyle.Length() * 4) {
			retStyle = CardStyle::TripleChainOneStyle(tripleChains[0], tripleChains[0] + lastStyle.Length() - 1, {});
			return true;
		}
		return false;
		break;
	}
	case ECardStyle::Triple_Chain_Two: {
		auto tripleChains = AvailableTripleChain(true, lastStyle.StartValue, lastStyle.Length());
		if (tripleChains.size() > 0 && Size() > lastStyle.Length() * 5) {
			retStyle = CardStyle::TripleChainOneStyle(tripleChains[0], tripleChains[0] + lastStyle.Length() - 1, {});
			return true;
		}
		return false;
		break;
	}
	default:
		break;
	}
	return false;
}
CardVector HandCards::GetCardsValue(uint8_t cardIndex, int count)const
{
	CardVector  r;
	int sum = 0;
	for (int i = 0; i < 4; ++i) {
		if (Flags[cardIndex][i] == 1) {
			r.push_back(CardColorIndexToValue(i, cardIndex));
			++sum;
		}
		else {
			continue;
		}
		if (sum == count)break;
	}
	_ASSERT(sum == count);
	return r;
}

CardStyle HandCards::FindLessCardStyle(const CardStyle & style)const
{
	switch (style.Style) {
	case ECardStyle::Single: {
		for (int i = 0; i < style.StartValue; i++) {
			int v = CardCount[i];
			if (v > 0) {
				return CardStyle(ECardStyle::Single, i);
			}
		}
		break;
	}
	case ECardStyle::Boom: {
		for (int i = 0; i < style.StartValue; i++) {
			int v = CardCount[i];
			if (v > 3) {
				return CardStyle(ECardStyle::Boom, i);
			}
		}
		break;
	}
	case ECardStyle::Double: {
		for (int i = 0; i < style.StartValue; i++) {
			int v = CardCount[i];
			if (v > 1) {
				return CardStyle(ECardStyle::Double, i);
			}
		}
		break;
	}
	case ECardStyle::Triple_Zero:
	case ECardStyle::Triple_One:
	case ECardStyle::Triple_Two: {
		for (int i = 0; i < style.StartValue; i++) {
			int v = CardCount[i];
			if (v > 0) {
				return CardStyle(ECardStyle::Triple_Zero, i);
			}
		}
		break;
	}
	case ECardStyle::Single_Chain: {
		int chainCount = style.EndValue - style.StartValue + 1;
		auto avChain = AvailableChain(chainCount, 1);
		if (avChain.size() > 0 && avChain[0] < style.StartValue) {
			return CardStyle(ECardStyle::Single_Chain, avChain[0], avChain[0] + chainCount - 1);
		}
		break;
	}
	case ECardStyle::Double_Chain: {
		int  chainCount = style.EndValue - style.StartValue + 1;
		auto avChain = AvailableChain(chainCount, 2);
		if (avChain.size() > 0 && avChain[0] < style.StartValue) {
			return CardStyle(ECardStyle::Double_Chain, avChain[0], avChain[0] + chainCount - 1);
		}
		break;
	}
	case ECardStyle::Triple_Chain_Zero:
	case ECardStyle::Triple_Chain_One:
	case ECardStyle::Triple_Chain_Two:
		int  chainCount = style.EndValue - style.StartValue + 1;
		auto avChain = AvailableChain(chainCount, 2);
		if (avChain.size() > 0 && avChain[0] < style.StartValue) {
			return CardStyle(ECardStyle::Triple_Chain_Zero, avChain[0], avChain[0] + chainCount - 1);
		}
		break;
	}
	return CardStyle::Invalid;
}

std::vector<uint8_t> HandCards::GetCardsValueByStyle(const CardStyle & style) const
{
	std::vector<uint8_t> r(15);
	switch (style.Style) {
	case ECardStyle::Single:
		r[style.StartValue] = 1;
		break;
	case ECardStyle::Double:
		r[style.StartValue] = 2;
		break;
	case ECardStyle::Triple_Zero:
		r[style.StartValue] = 3;
		break;
	case ECardStyle::Triple_One:
		r[style.StartValue] = 3;
		r[style.Extra[0]] += 1;
		break;
	case ECardStyle::Triple_Two:
		r[style.StartValue] = 3;
		r[style.Extra[0]] += 2;
		break;
	case ECardStyle::Triple_Chain_Zero:
		for (int i = style.StartValue; i <= style.EndValue; i++) {
			r[i] = 3;
		}
		break;
	case ECardStyle::Triple_Chain_One:
		for (int i = style.StartValue; i <= style.EndValue; i++) {
			r[i] = 3;
		}
		for (auto v : style.Extra) {
			r[v] += 1;
		}
		break;
	case ECardStyle::Triple_Chain_Two:
		for (int i = style.StartValue; i <= style.EndValue; i++) {
			r[i] = 3;
		}
		for (auto v : style.Extra) {
			r[v] += 2;
		}
		break;
	case ECardStyle::Single_Chain:
		for (int i = style.StartValue; i <= style.EndValue; i++) {
			r[i] = 1;
		}
		break;
	case ECardStyle::Double_Chain:
		for (int i = style.StartValue; i <= style.EndValue; i++) {
			r[i] = 2;
		}
		break;
	case ECardStyle::Quad_Single:
		r[style.StartValue] = 4;
		r[style.Extra[0]] += 1;
		r[style.Extra[1]] += 1;
		break;
	case ECardStyle::Quad_Double:
		r[style.StartValue] = 4;
		r[style.Extra[0]] += 2;
		r[style.Extra[1]] += 2;
		break;
	case ECardStyle::Boom:
		r[style.StartValue] = 4;
		if (style.StartValue == CardIndex_JokerBoom) {
			r[CardIndex_SmallJoker] = 1;
			r[CardIndex_LargeJoker] = 1;
		}
		break;
	}
	return r;
}

std::vector<uint8_t> HandCards::GetCardsByStyle(const CardStyle & style) const
{
	if (style == CardStyle::JokerBoom) {
		return { 0x01, 0x02 };
	}
	std::vector<uint8_t> cardsValue = GetCardsValueByStyle(style);
	std::vector<uint8_t> r;
	for (int i = 0; i < 15; i++) {
		auto v = Flags[i];
		for (int k = 0; k < 4; k++) {
			if (cardsValue[i] == 0) {
				break;
			}
			if (v[k] == 1) {
				uint8_t code = HandCards::CardColorIndexToValue(k, i);
				if (code > 0x10) {
					cardsValue[i]--;
					r.push_back(code);
				}
			}
		}
	}
	if (cardsValue[CardIndex_SmallJoker] == 1 && CardCount[CardIndex_SmallJoker] == 1) {
		r.push_back(0x01);
	}
	if (cardsValue[CardIndex_LargeJoker] == 1 && CardCount[CardIndex_LargeJoker] == 1) {
		r.push_back(0x02);
	}
	return r;
}

CardStyle HandCards::LastShot()
{
	UpdateByFlag();
	auto& sets = Data();
	CardVector v(sets.begin(), sets.end());
	return CardStyle::FromCardsValue(v);
}

HandCards::~HandCards()
{
}

#define SORT_PRIORITY(all,top)	CardVector out;\
std::set_difference(all.begin(), all.end(), top.begin(), top.end(), std::back_inserter(out));\
top.insert(top.end(), out.begin(), out.end())\


#define AVAILABLE_TRIPLES auto triples = AvailableTriple(true, lastStyle.StartValue);\
bool useLaiZi=false;\
if (triples.empty() && hasLaiZi) {\
	useLaiZi = true;\
	triples = AvailableDouble(true, lastStyle.StartValue);\
}\
if (triples.empty()) {\
	break;\
}
#define AVAILABLE_TRIPLECHAINS auto tripleChains = AvailableTripleChain(true, lastStyle.StartValue, lastStyle.Length());\
bool useLaiZi=false;\
if (tripleChains.empty() && hasLaiZi) {\
	useLaiZi = true;\
	tripleChains= AvailableTripleChain_LaiZi(lastStyle.StartValue,lastStyle.Length());\
}

CardVector HandCards::RequireDouble(int count, CardVector& excludeIndex, bool hasLaiZi)const
{
	auto isoDouble = FindIsolateCardsIndex();
	std::remove_if(isoDouble.begin(), isoDouble.end(), [this](uint8_t x) ->bool {return  CardCount[x] != 2; });
	auto singleChain = AvailableSingleChain();
	for (auto v : singleChain) {
		if (CardCount[v] == 3) {
			isoDouble.push_back(v);
		}
	}
	std::remove_if(isoDouble.begin(), isoDouble.end(), [this, excludeIndex](uint8_t x)->bool {return  std::find(excludeIndex.begin(), excludeIndex.end(), x) != excludeIndex.end(); });

	if (isoDouble.size() >= count)
		return isoDouble;

	auto all = AvailableDouble();
	CardVector exclude;
	std::set_difference(all.begin(), all.end(), excludeIndex.begin(), excludeIndex.end(), std::back_inserter(exclude));
	if (exclude.size() < count && hasLaiZi) {
		for (int i = CardIndex_3; i <= CardIndex_2; ++i) {
			if (CardCount[i] == 1) {
				exclude.push_back(i);
			}
		}
	}
	if (exclude.size() < count) {
		return exclude;
	}
	CardVector iso;
	for (auto v : exclude) {
		if (CardCount[v] == 2) {
			iso.push_back(v);
		}
	}
	if (!iso.empty()) {
		SORT_PRIORITY(exclude, iso);
		return iso;
	}
	else {
		return exclude;
	}
}

CardVector HandCards::RequireSingle(int count, CardVector& excludeIndex, bool hasLaiZi)const
{
	CardVector isoSingle;
	auto isolate = FindIsolateCardsIndex();
	for (auto isolateIndex : isolate) {
		if (CardCount[isolateIndex] == 1) {
			isoSingle.push_back(isolateIndex);
		}
	}
	auto singleChain = AvailableSingleChain();
	for (auto v : singleChain) {
		if (CardCount[v] == 2) {
			isoSingle.push_back(v);
		}
	}
	auto doubleChain = AvailableDoubleChain();
	for (auto v : doubleChain) {
		if (CardCount[v] == 3) {
			isoSingle.push_back(v);
		}
	}

	if (isoSingle.empty()) {
		for (auto isolateIndex : isolate) {
			if (CardCount[isolateIndex] == 2) {
				isoSingle.push_back(isolateIndex);
			}
		}
	}

	if (isoSingle.empty()) {
		auto r = PerfectSingle(-1);
		std::remove_if(r.begin(), r.end(), [this, excludeIndex](uint8_t x)->bool {return  std::find(excludeIndex.begin(), excludeIndex.end(), x) != excludeIndex.end(); });
		return r;
	}
	std::remove_if(isoSingle.begin(), isoSingle.end(), [this, excludeIndex](uint8_t x)->bool {return  std::find(excludeIndex.begin(), excludeIndex.end(), x) != excludeIndex.end(); });
	return isoSingle;
}

std::vector<CardVector> HandCards::ListTripleChainExtra(const CardVector & extra, int len)const
{
	if (len == 2) {
		return { { extra[0],extra[1] },{ extra[1],extra[2] } ,{ extra[0],extra[2] } };
	}
	if (len == 3) {
		return { { extra[0],extra[1],extra[2] },{ extra[1],extra[2] ,extra[3] } ,{ extra[0],extra[2],extra[3] } ,{ extra[0],extra[1],extra[3] } };
	}
	return std::vector<CardVector>();
}

bool HandCards::FindAvailableTriple_LaiZi(bool hasLaizi, uint8_t cardIndex, CardVector & triples)const
{
	CardVector doubles;
	for (int i = cardIndex + 1; i <= CardIndex_2; ++i) {
		if (CardCount[i] == 3) {
			triples.push_back(i);
		}
		if (CardCount[i] == 2) {
			doubles.push_back(i);
		}
	}
	bool useLaiZi = hasLaizi && triples.empty() && !doubles.empty();
	if (triples.empty() && hasLaizi) {
		triples.insert(triples.end(), doubles.begin(), doubles.end());
	}
	return useLaiZi;
}

std::vector<CardStyle> HandCards::FindAvailableTake(CardStyle & lastStyle, bool hasLaiZi)const
{
	std::vector<CardStyle> r;

	auto booms = AvailableBoom(true, lastStyle.StartValue);
	if (lastStyle.Style != ECardStyle::Invalid && booms.size()==0) {
		int cardSize = Size() + (hasLaiZi ? 1 : 0);
		if (cardSize < lastStyle.GetCardsCount()) {
			return r;
		}
	}
	switch (lastStyle.Style)
	{
	case ECardStyle::Invalid:
		return { FindLeastCardPlay(hasLaiZi) };
	case ECardStyle::Boom: {
		if (booms.empty() && hasLaiZi) {
			booms = AvailableTriple(true, lastStyle.StartValue);
			if (CardCount[CardIndex_LargeJoker] + CardCount[CardIndex_SmallJoker] == 1) {
				booms.push_back(CardIndex_JokerBoom);
			}
		}
		for (auto&v : booms) {
			r.push_back(std::move(CardStyle::BoomStyle(v)));
		}
		break;
	}
	case ECardStyle::Triple_One: {//如Triple不为0，则不将癞子用于组Triple
		CardVector triples;
		bool useLaiZi = FindAvailableTriple_LaiZi(hasLaiZi, lastStyle.StartValue, triples);
		for (auto & t : triples) {
			auto in = CardVector{ t };
			auto extras = RequireSingle(1, in, hasLaiZi && !useLaiZi);
			if (extras.empty()) {
				r.emplace_back(CardStyle::TripleZeroStyle(t));
			}
			else {
				for (auto & v : extras) {
					r.emplace_back(CardStyle::TripleOneStyle(t, { v }));
				}
			}
		}
		break;
	}
	case ECardStyle::Triple_Two: {
		CardVector triples;
		bool useLaiZi = FindAvailableTriple_LaiZi(hasLaiZi, lastStyle.StartValue, triples);
		for (auto & t : triples) {
			auto in = CardVector{ t };
			auto extras = RequireDouble(1, in, hasLaiZi && !useLaiZi);
			if (extras.empty()) {
				r.emplace_back(CardStyle::TripleZeroStyle(t));
			}
			else {
				for (auto & v : extras) {
					r.push_back(std::move(CardStyle::TripleTwoStyle(t, { v })));
				}
			}
		}
		break;
	}
	case ECardStyle::Triple_Zero: {
		CardVector triples;
		bool useLaiZi = FindAvailableTriple_LaiZi(hasLaiZi, lastStyle.StartValue, triples);
		for (auto & t : triples) {
			r.push_back(std::move(CardStyle::TripleZeroStyle(t)));
		}
		break;
	}
	case ECardStyle::Double: {
		auto perfectDoubles = PerfectDouble(lastStyle.StartValue);
		auto doubles = AvailableDouble(true, lastStyle.StartValue);
		for (auto v : doubles) {
			if (std::any_of(perfectDoubles.begin(), perfectDoubles.end(), [v](uint8_t pv)->bool { return v == pv; })) {
				continue;
			}
			else {
				perfectDoubles.push_back(v);
			}
		}
		if (perfectDoubles.empty() && hasLaiZi) {
			perfectDoubles = AvailableSingle(true, lastStyle.StartValue);
		}
		for (auto & v : perfectDoubles) {
			if (v <= CardIndex_2) {
				r.push_back(std::move(CardStyle::DoubleStyle(v)));
			}
		}
		break;
	}
	case ECardStyle::Single: {
		auto perfectSingles = PerfectSingle(lastStyle.StartValue);
		auto singles = AvailableSingle(true, lastStyle.StartValue);
		for (auto v : singles) {
			if (std::any_of(perfectSingles.begin(), perfectSingles.end(), [v](uint8_t pv)->bool { return v == pv; })) {
				continue;
			}
			else {
				perfectSingles.push_back(v);
			}
		}
		for (auto & v : perfectSingles) {
			r.push_back(std::move(CardStyle::SingleStyle(v)));
		}
		break;
	}
	case ECardStyle::Single_Chain: {
		auto singleChains = AvailableSingleChain(true, lastStyle.StartValue, lastStyle.Length());
		if (hasLaiZi) {
			for (auto& v : AvailableSingleChain_LaiZi(lastStyle.StartValue, lastStyle.Length())) {
				VECTOR_INSERT_UNIQUE(singleChains, v)
			}
		}
		for (auto & v : singleChains) {
			r.push_back(std::move(CardStyle::SingleChainStyle(v, v + lastStyle.EndValue - lastStyle.StartValue)));
		}
		break;
	}
	case ECardStyle::Double_Chain: {
		auto doubleChains = AvailableDoubleChain(true, lastStyle.StartValue, lastStyle.Length());
		if (hasLaiZi) {
			for (auto& v : AvailableDoubleChain_LaiZi(lastStyle.StartValue, lastStyle.Length())) {
				VECTOR_INSERT_UNIQUE(doubleChains, v)
			}
		}
		for (auto& v : doubleChains) {
			r.push_back(std::move(CardStyle::DoubleChainStyle(v, v + lastStyle.EndValue - lastStyle.StartValue)));
		}
		break;
	}
	case ECardStyle::Triple_Chain_Zero: {
		auto tripleChains = AvailableTripleChain(true, lastStyle.StartValue, lastStyle.Length());
		if (hasLaiZi) {
			auto tripleChains_laizi = AvailableTripleChain_LaiZi(lastStyle.StartValue, lastStyle.Length());
			decltype(tripleChains) unionTriples;
			std::merge(tripleChains.begin(), tripleChains.end(), tripleChains_laizi.begin(), tripleChains_laizi.end(), std::back_inserter(unionTriples));

			for (auto&v : unionTriples) {
				r.push_back(std::move(CardStyle::TripleChainZeroStyle(v, v + lastStyle.EndValue - lastStyle.StartValue)));
			}
		}
		else {
			for (auto&v : tripleChains) {
				r.push_back(std::move(CardStyle::TripleChainZeroStyle(v, v + lastStyle.EndValue - lastStyle.StartValue)));
			}
		}
		break;
	}
	case ECardStyle::Triple_Chain_One: {
		AVAILABLE_TRIPLECHAINS
			auto chainLen = lastStyle.Length();

		for (auto&tripleIndex : tripleChains) {
			CardVector excludeIndex;
			for (int i = 0; i < chainLen; ++i) {
				excludeIndex.push_back(tripleIndex + i);
			}
			auto extraSingle = RequireSingle(chainLen, excludeIndex, hasLaiZi && !useLaiZi);
			auto extraSize = extraSingle.size();
			if (extraSize == chainLen) {
				r.push_back(std::move(CardStyle::TripleChainOneStyle(tripleIndex, tripleIndex + lastStyle.EndValue - lastStyle.StartValue, extraSingle)));
			}
			else if (extraSize > chainLen) {
				auto lst = ListTripleChainExtra(extraSingle, chainLen);
				for (auto& chainExtra : lst) {
					r.push_back(std::move(CardStyle::TripleChainOneStyle(tripleIndex, tripleIndex + lastStyle.EndValue - lastStyle.StartValue, chainExtra)));
				}
			}
			else {
				if (chainLen == 2) {
					auto extraDouble = RequireDouble(chainLen, excludeIndex, false);
					for (auto& v : extraDouble) {
						r.push_back(std::move(CardStyle::TripleChainOneStyle(tripleIndex, tripleIndex + lastStyle.EndValue - lastStyle.StartValue, { v,v })));
					}
				}
				if (chainLen == 3) {
					auto extraDouble = RequireDouble(chainLen, excludeIndex, false);
					if (extraSingle.size() > 0) {
						for (auto& v : extraDouble) {
							r.push_back(std::move(CardStyle::TripleChainOneStyle(tripleIndex, tripleIndex + lastStyle.EndValue - lastStyle.StartValue, { v,v,extraSingle[0] })));
						}
					}
					else {
						if (extraDouble.size() >= 2) {
							r.push_back(std::move(CardStyle::TripleChainOneStyle(tripleIndex, tripleIndex + lastStyle.EndValue - lastStyle.StartValue, { extraDouble[0],extraDouble[0],extraDouble[1] })));
						}
					}
				}
			}
		}
		break;
	}
	case ECardStyle::Triple_Chain_Two: {
		AVAILABLE_TRIPLECHAINS
			for (auto&tripleIndex : tripleChains) {
				CardVector excludeIndex;
				for (int i = 0; i < lastStyle.Length(); ++i) {
					excludeIndex.push_back(tripleIndex + i);
				}
				auto extra = RequireDouble(lastStyle.Length(), excludeIndex, hasLaiZi && !useLaiZi);
				auto extraSize = extra.size();
				auto styleLen = lastStyle.Length();
				if (extraSize == styleLen) {
					r.push_back(std::move(CardStyle::TripleChainTwoStyle(tripleIndex, tripleIndex + lastStyle.EndValue - lastStyle.StartValue, extra)));
				}
				if (extraSize > styleLen) {
					auto lst = ListTripleChainExtra(extra, styleLen);
					for (auto& chainExtra : lst) {
						r.push_back(std::move(CardStyle::TripleChainTwoStyle(tripleIndex, tripleIndex + lastStyle.EndValue - lastStyle.StartValue, chainExtra)));
					}
				}
			}
		break;
	}
	default:
		break;
	}
	if (lastStyle.Style != ECardStyle::Invalid && lastStyle.Style != ECardStyle::Boom) {
		CardVector booms;
		if (hasLaiZi) {
			booms = AvailableTriple();
			if (CardCount[CardIndex_SmallJoker] == 1 || CardCount[CardIndex_LargeJoker] == 1) {
				booms.push_back(CardIndex_JokerBoom);
			}
		}
		else {
			booms = AvailableBoom();
		}
		for (auto&v : booms) {
			r.push_back(std::move(CardStyle::BoomStyle(v)));
		}
	}
	return r;
}
CardVector HandCards::AvailableChain_LaiZi(int len, int count, int8_t cardStartIndex)const
{
	CardVector r;
	int cardSum = 0;
	for (int startIndex = cardStartIndex + 1; startIndex + len <= CardIndex_A; ++startIndex) {
		cardSum = 0;
		bool bFill = false;
		for (int i = startIndex; i < startIndex + len; ++i) {
			if (CardCount[i] >= count) {
				++cardSum;
			}
			else {
				if (CardCount[i] >= count - 1) {
					bFill = true;
				}
			}
		}
		if (bFill && cardSum >= len - 1) {
			r.push_back(startIndex);
		}
	}
	return r;
}

CardVector HandCards::AvailableSingleChain_LaiZi(int8_t cardStartIndex, int len)const
{
	return AvailableChain_LaiZi(len, 1, cardStartIndex);
}

CardVector HandCards::AvailableDoubleChain_LaiZi(int8_t cardStartIndex, int len)const
{
	return AvailableChain_LaiZi(len, 2, cardStartIndex);
}

CardVector HandCards::AvailableTripleChain_LaiZi(int8_t cardStartIndex, int len)const
{
	return AvailableChain_LaiZi(len, 3, cardStartIndex);
}

std::vector<CardRange> HandCards::AvailableTripleChainRange(bool hasLaiZi)const {
	std::vector<CardRange> r;
	CardVector  chain;
	if (hasLaiZi)
		chain = AvailableTripleChain_LaiZi(-1, 2);
	else
		chain = AvailableTripleChain();
	if (chain.size() > 0) {
		uint8_t head = chain[0];
		uint8_t tail = head;
		for (auto &v : chain) {
			if (v - tail > 1) {
				r.emplace_back(head, tail + 1);
				head = v;
				tail = v;
			}
			if (v - tail == 1) {
				tail = v;
			}
		}
		if (head == tail) {
			r.emplace_back(head, head + 1);
		}
		else {
			auto first = head;
			auto end = tail + 1;

			if (CardCount[head] == 2) {
				first += 1;
			}
			if (CardCount[end] == 2) {
				end -= 1;
			}
			int zeroCount = 0;
			for (int i = head; i <= end; ++i) {
				if (CardCount[i] == 2) {
					++zeroCount;
				}
				if (zeroCount == 2) {
					end = i - 1;
					break;
				}
			}
			r.emplace_back(first, end);
		}
	}
	return r;
}

std::vector<CardRange> HandCards::AvailableDoubleChainRange(bool hasLaiZi)const {
	std::vector<CardRange> r;
	CardVector  chain;
	if (hasLaiZi)
		chain = AvailableDoubleChain_LaiZi(-1, 3);
	else
		chain = AvailableDoubleChain();
	if (chain.size() > 0) {
		uint8_t head = chain[0];
		uint8_t tail = head;
		for (auto &v : chain) {
			if (v - tail > 1) {
				r.emplace_back(head, tail + 2);
				head = v;
				tail = v;
			}
			if (v - tail == 1) {
				tail = v;
			}
		}
		if (head == tail) {
			r.emplace_back(head, head + 2);
		}
		else {
			auto first = head;
			auto end = tail + 2;

			if (CardCount[head] == 1) {
				first += 1;
			}
			if (CardCount[end] == 1) {
				end -= 1;
			}
			int zeroCount = 0;
			for (int i = head; i <= end; ++i) {
				if (CardCount[i] == 1) {
					++zeroCount;
				}
				if (zeroCount == 2) {
					end = i - 1;
					break;
				}
			}
			r.emplace_back(first, end);
		}
	}
	return r;
}

std::vector<CardRange> HandCards::AvailableSingleChainRange(bool hasLaiZi) const {
	std::vector<CardRange> r;
	CardVector  chain;
	if (hasLaiZi)
		chain = AvailableSingleChain_LaiZi(-1, 5);
	else
		chain = AvailableSingleChain();
	if (chain.size() > 0) {
		uint8_t head = chain[0];
		uint8_t tail = head;
		for (auto &v : chain) {
			if (v - tail > 1) {
				r.emplace_back(head, tail + 4);
				head = v;
				tail = v;
			}
			if (v - tail == 1) {
				tail = v;
			}
		}
		if (head == tail) {
			r.emplace_back(head, head + 4);
		}
		else {
			auto first = head;
			auto end = tail + 4;

			if (CardCount[head] == 0) {
				first += 1;
			}
			if (CardCount[end] == 0) {
				end -= 1;
			}
			int zeroCount = 0;
			for (int i = head; i <= end; ++i) {
				if (CardCount[i] == 0) {
					++zeroCount;
				}
				if (zeroCount == 2) {
					end = i - 1;
					break;
				}
			}
			r.emplace_back(first, end);
		}
	}
	return r;
}
