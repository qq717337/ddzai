#include "stdafx.h"
#include "CardStyle.h"
#include "HandCardsFlag.h"
#include"common_algorithm.h"
const CardStyle CardStyle::Invalid = CardStyle(ECardStyle::Invalid, 0, 0);
const CardStyle CardStyle::JokerBoom = CardStyle(ECardStyle::Boom, CardIndex_JokerBoom);

CardStyle::CardStyle()
{
	Style = ECardStyle::Invalid;
	StartValue = EndValue = 0;
	Extra = CardVector();
}


CardStyle::~CardStyle()
{
}
CardStyle::CardStyle(const CardStyle & inStyle)
{
	Style = inStyle.Style;
	StartValue = inStyle.StartValue;
	EndValue = inStyle.EndValue;
	Extra = inStyle.Extra;
}
CardStyle::CardStyle(int style, uint8_t startValue)
{
	Style = style;
	StartValue = startValue;
	EndValue = startValue;
}
CardStyle::CardStyle(int style, uint8_t startValue, uint8_t endValue)
{
	Style = style;
	StartValue = startValue;
	EndValue = endValue;
}
CardStyle::CardStyle(int style, uint8_t startValue, uint8_t endValue, uint8_t extra)
{
	Style = style;
	StartValue = startValue;
	EndValue = endValue;
	Extra = { extra };
}
CardStyle::CardStyle(int style, uint8_t startValue, uint8_t endValue, const CardVector & extra)
{
	Style = style;
	StartValue = startValue;
	EndValue = endValue;
	Extra = extra;
}
CardStyle::CardStyle(int style, uint8_t startValue, const CardVector & extra)
{
	Style = style;
	StartValue = startValue;
	EndValue = startValue;
	Extra = extra;
}
CardVector CardStyle::Index() {
	CardVector  r;
	switch (Style) {
	case ECardStyle::Single:
		r.push_back(StartValue);
		break;
	case ECardStyle::Double:
		r.push_back(StartValue);
		r.push_back(StartValue);
		break;
	case ECardStyle::Boom:
		r.push_back(StartValue);
		r.push_back(StartValue);
		r.push_back(StartValue);
		r.push_back(StartValue);
		break;
	case ECardStyle::Triple_Zero:
	case ECardStyle::Triple_One:
	case ECardStyle::Triple_Two:
		r.push_back(StartValue);
		r.push_back(StartValue);
		r.push_back(StartValue);

		r.insert(r.end(), Extra.begin(), Extra.end());
		if (Style == ECardStyle::Triple_Two) {
			r.insert(r.end(), Extra.begin(), Extra.end());
		}
		break;
	case ECardStyle::Double_Chain:
		for (int i = StartValue; i <= EndValue; i++) {
			r.push_back(i);
			r.push_back(i);
		}
		break;
	case ECardStyle::Triple_Chain_Zero:
	case ECardStyle::Triple_Chain_One:
	case ECardStyle::Triple_Chain_Two:
		for (int i = StartValue; i <= EndValue; i++) {
			r.push_back(i);
			r.push_back(i);
			r.push_back(i);
		}
		r.insert(r.end(), Extra.begin(), Extra.end());
		if (Style == ECardStyle::Triple_Chain_Two) {
			r.insert(r.end(), Extra.begin(), Extra.end());
		}
		break;
	case ECardStyle::Single_Chain:
		for (int i = StartValue; i <= EndValue; i++) {
			r.push_back(i);
		}
		break;
	}
	return r;
}


int  CardStyle::GetCardsCount()const {
	switch (Style) {
	case ECardStyle::Single:
		return 1;
	case ECardStyle::Double:
		return 2;
	case ECardStyle::Boom:{
		if (StartValue == CardIndex_JokerBoom)
			return 2;
		return 4;
	}
	case ECardStyle::Quad_Single:
		return 6;
	case ECardStyle::Quad_Double:
		return 8;
	case ECardStyle::Triple_Zero:
		return 3;
	case ECardStyle::Triple_One:
		return 4;
	case ECardStyle::Triple_Two:
		return 5;
	case ECardStyle::Double_Chain:
		return (EndValue - StartValue + 1) * 2;
	case ECardStyle::Triple_Chain_Zero:
		return (EndValue - StartValue + 1) * 3;
	case ECardStyle::Triple_Chain_One:
		return (EndValue - StartValue + 1) * 4;
	case ECardStyle::Triple_Chain_Two:
		return (EndValue - StartValue + 1) * 5;
	case ECardStyle::Single_Chain:
		return (EndValue - StartValue + 1) * 1;
	}
	return 0;
}

CardVector CardStyle::GetCardCount(CardVector cards)
{
	CardVector CardCount{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	for (auto v : cards) {
		if (v == 0x01) {
			CardCount[ECardIndex::CardIndex_SmallJoker]++;
		}
		else if (v == 0x02) {
			CardCount[ECardIndex::CardIndex_LargeJoker]++;
		}
		else {
			CardCount[HandCardsFlag::CardValueToIndex(v)]++;
		}
	}
	return CardCount;
}

CardStyle CardStyle::FromCardsValue(const CardVector &cards) {
	size_t length = cards.size();
	CardVector  indexCards(length);
	auto be = indexCards.begin();
	for (size_t i = 0; i < length; ++i) {
		indexCards[i] = HandCardsFlag::CardValueToIndex(cards[i]);
	}
	std::sort(indexCards.begin(), indexCards.end());
	if (cards.size() >= 5) {
		bool isChain = true;
		for (int i = 1; i < indexCards.size(); i++) {
			if (indexCards[i] - indexCards[i - 1] != 1) {
				isChain = false;
			}
		}
		auto tailValue = *(indexCards.end() - 1);
		if (isChain && tailValue < CardIndex_2)
			return CardStyle::SingleChainStyle(*indexCards.begin(), tailValue);

	}
	switch (length) {
	case 1: {
		uint8_t v = indexCards[0];
		return CardStyle(ECardStyle::Single, v, v);
	}
	case 2: {
		if (indexCards[0] == indexCards[1]) {
			uint8_t v = indexCards[0];
			return CardStyle(ECardStyle::Double, v, v);
		}
		else {
			if (indexCards[0] == CardIndex_SmallJoker && indexCards[1] == CardIndex_LargeJoker) {
				return CardStyle::JokerBoom;
			}
			else {
				return CardStyle::Invalid;
			}
		}
		break;
	}

	case 3: {
		uint8_t v = indexCards[0];
		if (indexCards[1] == v && indexCards[2] == v) {
			return CardStyle(ECardStyle::Triple_Zero, v, v);
		}
		break;
	}
	case 4: {
		auto m = cardCountMap(indexCards);
		int8_t boom = -1;
		int8_t triple = -1;
		int8_t extra = -1;

		for (auto &v : m) {
			if (v.second == 4) {
				boom = v.first;
			}
			if (v.second == 3) {
				triple = v.first;
			}
			if (v.second == 1) {
				extra = v.first;
			}
		}
		if (boom > -1) {
			return CardStyle::BoomStyle(boom);
		}
		if (triple > -1 && extra > -1) {
			return CardStyle::TripleOneStyle(triple, { uint8_t(extra) });
		}

		break;
	}

	case 5: {
		auto m = cardCountMap(indexCards);
		int8_t triple = -1;
		int8_t extra = -1;
		for (auto &v : m) {
			if (v.second == 3) {
				triple = v.first;
			}
			if (v.second == 2) {
				extra = v.first;
			}
		}
		if (triple > -1 && extra > -1) {
			return CardStyle(ECardStyle::Triple_Two, uint8_t(triple), uint8_t(triple), { uint8_t(extra) });
		}

		break;
	}
	case 6: {
		auto m = cardCountMap(indexCards);
		CardVector tripleChain;
		int8_t boom = -1;
		CardVector extra;
		CardVector doubleChain;
		for (auto &kv : m) {
			if (kv.second == 4) {
				boom = kv.first;
			}
			if (kv.second == 3) {
				tripleChain.push_back(kv.first);
			}
			if (kv.second == 1) {
				extra.push_back(kv.first);
			}
			if (kv.second == 2) {
				extra.push_back(kv.first);
				extra.push_back(kv.first);
				doubleChain.push_back(kv.first);
			}
		}
		if (doubleChain.size() == 3) {
			return CardStyle::DoubleChainStyle(*doubleChain.begin(), *(--doubleChain.end()));
		}
		if (tripleChain.size() == 2) {
			auto min = *tripleChain.begin();
			auto max = *(--tripleChain.end());
			if (max - min == 1 && max < CardIndex_2) {
				return CardStyle::TripleChainZeroStyle(min, max);
			}
		}
		else {
			if (boom > -1 && extra.size() == 2)
				return CardStyle::QuadSingleStyle(boom, extra);
		}
		break;

	}
	case 8: {
		auto m = cardCountMap(indexCards);
		CardVector tripleChain;
		CardVector extra;
		CardVector boom;
		CardVector doubleChain;
		for (auto &kv : m) {
			if (kv.second == 3) {
				tripleChain.push_back(kv.first);
			}
			if (kv.second == 4) {
				boom.push_back(kv.first);
				tripleChain.push_back(kv.first);
				extra.push_back(kv.first);
			}
			if (kv.second == 1) {
				extra.push_back(kv.first);
			}
			if (kv.second == 2) {
				extra.push_back(kv.first);
				extra.push_back(kv.first);
				doubleChain.push_back(kv.first);
			}
		}
		if (doubleChain.size() == 4) {
			auto min = *doubleChain.begin();
			auto max = *(--doubleChain.end());
			if (max - min == 3 && max < CardIndex_2) {
				return CardStyle::DoubleChainStyle(min, max);
			}
		}
		if (tripleChain.size() == 2) {
			auto min = *tripleChain.begin();
			auto max = *(--tripleChain.end());
			if (max - min == 1 && max < CardIndex_2) {
				return CardStyle::TripleChainOneStyle(min, max, extra);
			}
		}
		if (boom.size() == 1 && doubleChain.size() == 2) {
			return CardStyle::QuadDoubleStyle(boom[0], doubleChain);
		}
		if (boom.size() == 2) {
			return CardStyle::QuadDoubleStyle(boom[1], { boom[0],boom[0] });
		}
		break;
	}

	case 9:
	{
		auto m = cardCountMap(indexCards);
		CardVector tripleChain;
		for (auto &kv : m) {
			if (kv.second != 3) {
				return CardStyle::Invalid;
			}
			else {
				tripleChain.push_back(kv.first);
			}
		}
		if (tripleChain.size() == 3) {
			auto min = *tripleChain.begin();
			auto max = *(--tripleChain.end());
			if (max - min == 2 && max < CardIndex_2) {
				return CardStyle::TripleChainZeroStyle(min, max);
			}
		}
		break;
	}

	case 10: {
		auto m = cardCountMap(indexCards);

		CardVector tripleChain;
		CardVector extra;
		CardVector doubleChain;
		for (auto &kv : m) {
			if (kv.second == 3) {
				tripleChain.push_back(kv.first);
			}
			if (kv.second == 2) {
				extra.push_back(kv.first);
				doubleChain.push_back(kv.first);
			}
		}
		if (doubleChain.size() == 5) {
			auto min = *doubleChain.begin();
			auto max = *(--doubleChain.end());
			if (max - min == 4 && max < CardIndex_2) {
				return CardStyle(ECardStyle::Double_Chain, min, max);
			}
		}
		if (tripleChain.size() == 2 && extra.size() == 2) {
			auto min = *tripleChain.begin();
			auto max = *(--tripleChain.end());
			if (max - min == 1 && max < CardIndex_2) {
				return CardStyle::TripleChainTwoStyle(min, max, extra);
			}
		}
		break;

	}
	case 12: {
		auto m = cardCountMap(indexCards);

		CardVector doubleChain;
		CardVector tripleChain;
		CardVector extra;
		for (auto &kv : m) {
			if (kv.second == 3) {
				tripleChain.push_back(kv.first);
			}
			if (kv.second == 4) {
				tripleChain.push_back(kv.first);
				extra.push_back(kv.first);
			}
			if (kv.second == 1) {
				extra.push_back(kv.first);
			}
			if (kv.second == 2) {
				doubleChain.push_back(kv.first);
				extra.push_back(kv.first);
				extra.push_back(kv.first);
			}
		}
		if (doubleChain.size() == 6) {
			auto min = *doubleChain.begin();
			auto max = *(--doubleChain.end());
			if (max - min == 5) {
				return CardStyle::DoubleChainStyle(min, max);
			}
        }
		if (tripleChain.size() == 4) {
            auto min = *tripleChain.begin();
            auto max = *(--tripleChain.end());
			if (max - min == 3 && max < CardIndex_2) {
				return CardStyle::TripleChainZeroStyle(min, max);
			}
		}
		if (tripleChain.size() == 3 && extra.size() == 3) {
            auto min = *tripleChain.begin();
            auto max = *(--tripleChain.end());
			if (max - min == 2 && max < CardIndex_2) {
				return CardStyle::TripleChainOneStyle(min, max, extra);
			}
		}
		break;
	}
	case 14: {
		auto m = cardCountMap(indexCards);

		CardVector doubleChain;
		for (auto &kv : m) {
			if (kv.second == 2) {
				doubleChain.push_back(kv.first);
			}
		}
		if (doubleChain.size() == 7) {
			auto min = *doubleChain.begin();
			auto max = *(--doubleChain.end());
			if (max - min == 6 && max < CardIndex_2) {
				return CardStyle::DoubleChainStyle(min, max);
			}
		}
		break;
	}
	case 15:
	{
		auto m = cardCountMap(indexCards);
		CardVector tripleChain;
		CardVector extra;
		for (auto &kv : m) {
			if (kv.second == 3) {
				tripleChain.push_back(kv.first);
			}
			if (kv.second == 2) {
				extra.push_back(kv.first);
			}
        }
		if (tripleChain.size() == 5) {
            auto min = *tripleChain.begin();
            auto max = *(--tripleChain.end());
			if (max - min == 4 && max < CardIndex_2) {
				return CardStyle::TripleChainZeroStyle(min, max);
			}
		}
		if (tripleChain.size() == 3 && extra.size() == 3) {
            auto min = *tripleChain.begin();
            auto max = *(--tripleChain.end());
			if (max - min == 2 && max < CardIndex_2) {
				return CardStyle::TripleChainTwoStyle(min, max, extra);
			}
		}
		break;
	}
	case 16:
	{
		auto m = cardCountMap(indexCards);
		CardVector tripleChain;
		CardVector extra;
		CardVector doubleChain;

		for (auto& kv : m)
		{
			if (kv.second == 4) {
				tripleChain.push_back(kv.first);
				extra.push_back(kv.first);
			}
			if (kv.second == 3) {
				tripleChain.push_back(kv.first);
			}
			if (kv.second == 2) {
				doubleChain.push_back(kv.first);
			}
			if (kv.second == 1) {
				extra.push_back(kv.first);
			}
		}
		if (tripleChain.size() == 4 && extra.size() == 4)
		{
			auto min = *tripleChain.begin();
			auto max = *(--tripleChain.end());
			if (max - min == 3 && max < CardIndex_2)
			{
				return  CardStyle::TripleChainOneStyle(min, max, extra);
			}
		}
		if (doubleChain.size() == 8)
		{
			auto min = *doubleChain.begin();
			auto max = *(--doubleChain.end());
			if (max - min == 7 && max < CardIndex_2)
			{
				return CardStyle::DoubleChainStyle(min, max);
			}
		}
		break;
	}
	case 18:
	{
		auto m = cardCountMap(indexCards);
		CardVector doubleChain;

		for (auto& kv : m)
		{
			if (kv.second == 2)
			{
				doubleChain.push_back(kv.first);
			}
        }

        if (doubleChain.size() == 9)
		{
            auto min = *doubleChain.begin();
            auto max = *(--doubleChain.end());
            if(max - min == 8 && max < CardIndex_2){
                return CardStyle::DoubleChainStyle(min, max);
            }
		}
		break;
	}
	case 20:
	{
		auto m = cardCountMap(indexCards);
		CardVector tripleChain;
		CardVector doubleChain;

		for (auto& kv : m)
		{
			if (kv.second == 3)
			{
				tripleChain.push_back(kv.first);
			}
			if (kv.second == 2)
			{
				doubleChain.push_back(kv.first);
			}
		}
		if (tripleChain.size() == 4 && doubleChain.size() == 4)
		{
			auto min = *tripleChain.begin();
			auto max = *(--tripleChain.end());
			if (max - min == 3 && max < CardIndex_2)
			{
				return CardStyle::TripleChainTwoStyle(min, max, doubleChain);
			}
		}
		if (doubleChain.size() == 10)
		{
			auto min = *doubleChain.begin();
			auto max = *(--doubleChain.end());
			if (max - min == 9 && max < CardIndex_2)
			{
				return  CardStyle::DoubleChainStyle(min, max);
			}
		}
		break;
	}
	}
	return CardStyle::Invalid;
}

CardStyle CardStyle::FromCardsValueWithLaizi(CardVector cards, uint8_t & laiZiIndex, ECardStyle::ECardStyle_ style) {
	CardStyle r = Invalid;
	int ProposeCount = cards.size();
	cards.erase(std::find_if(cards.begin(), cards.end(), [](uint8_t x) { return x >= 0x50; }));

	auto CardCount = GetCardCount(cards);
	if (ProposeCount >= 5) {
		r = CheckIfLaiziSingleChain(CardCount, cards, laiZiIndex);
		if (r.Valid()) {
			goto ret;
		}
	}
	if (ProposeCount > 5) {
		if (ProposeCount % 2 == 0) {
			r = CheckIfLaiziDoubleChain(CardCount, cards, laiZiIndex);
			if (r.Valid()) {
				goto ret;
			}
		}
		if (style != ECardStyle::Triple_Zero && ProposeCount == 6) {
			auto quadIndexIter = std::find(CardCount.begin(), CardCount.end(), 4);
			if (quadIndexIter != CardCount.end()) {
				auto laiZiIndexIter = std::find(CardCount.begin(), CardCount.end(), 1);
				if (laiZiIndexIter != CardCount.end()) {
					laiZiIndex = laiZiIndexIter - CardCount.begin();
					uint8_t quadIndex = quadIndexIter - CardCount.begin();
					r = QuadSingleStyle(quadIndex, { laiZiIndex, laiZiIndex });
					goto ret;
				}
			}
			quadIndexIter = std::find(CardCount.begin(), CardCount.end(), 3);
			if (quadIndexIter != CardCount.end()) {
				uint8_t quadIndex = quadIndexIter - CardCount.begin();
				laiZiIndex = quadIndex;
				auto doubleExtraIter = quadIndexIter = std::find(CardCount.begin(), CardCount.end(), 2);
				if (doubleExtraIter != CardCount.end()) {
					uint8_t doubleExtra = doubleExtraIter - CardCount.begin();
					r = QuadSingleStyle(quadIndex, { doubleExtra, doubleExtra });
				}
				else {
					auto firstExtraIter = std::find(CardCount.begin(), CardCount.end(), 1);
					if (firstExtraIter != CardCount.end()) {
						auto secondExtraIter = std::find(firstExtraIter + 1, CardCount.end(), 1);
						if (secondExtraIter != CardCount.end())
						{
							uint8_t first = firstExtraIter - CardCount.begin();
							uint8_t second = secondExtraIter - CardCount.begin();
							r = QuadSingleStyle(quadIndex, { first ,second });
						}
					}
				}
			}
			if (r.Valid())
				goto ret;
		}
		else if (style != ECardStyle::Triple_Chain_One && ProposeCount == 8) {
			auto quadIndexIter = std::find(CardCount.begin(), CardCount.end(), 4);
			if (quadIndexIter != CardCount.end()) {
				uint8_t quadIndex = quadIndexIter - CardCount.begin();
				auto laiZiIndexIter = std::find(CardCount.begin(), CardCount.end(), 1);
				auto secondExtraIter = std::find(CardCount.begin(), CardCount.end(), 2);
				if (laiZiIndexIter != CardCount.end())
				{
					laiZiIndex = laiZiIndexIter - CardCount.begin();
					uint8_t secondExtra = secondExtraIter - CardCount.begin();
					r = QuadDoubleStyle(quadIndex, { secondExtra, laiZiIndex });
				}
			}

			quadIndexIter = std::find(CardCount.begin(), CardCount.end(), 3);
			if (quadIndexIter != CardCount.end()) {
				uint8_t quadIndex = quadIndexIter - CardCount.begin();
				laiZiIndex = quadIndex;
				auto firstExtraIter = std::find(CardCount.begin(), CardCount.end(), 2);
				if (firstExtraIter != CardCount.end()) {
					auto secondExtraIter = std::find(firstExtraIter + 1, CardCount.end(), 2);
					if (secondExtraIter != CardCount.end()) {
						uint8_t	firstExtra = firstExtraIter - CardCount.begin();
						uint8_t	secondExtra = secondExtraIter - CardCount.begin();
						r = QuadDoubleStyle(quadIndex, { firstExtra, secondExtra });
					}
				}
			}
			if (r.Valid())
				goto ret;
		}
		else {
			if (ProposeCount % 3 == 0) {
				r = CheckIfLaiziTripleChainZero(CardCount, cards, laiZiIndex);
				if (r.Valid()) {
					goto ret;
				}
			}
		}
		if (ProposeCount % 4 == 0) {
			r = CheckIfLaiziTripleChainOne(CardCount, cards, laiZiIndex);
			if (r.Valid()) {
				goto ret;
			}
		}
		if (ProposeCount % 5 == 0) {
			r = CheckIfLaiziTripleChainTwo(CardCount, cards, laiZiIndex);
			if (r.Valid()) {
				goto ret;
			}
		}
	}

	switch (ProposeCount)
	{
	case 1:// 单张
		break;
	case 2:// 对子 - 王炸弹
	{
		if (CardCount[ECardIndex::CardIndex_SmallJoker] == 1) {
			laiZiIndex = 102;
			r = JokerBoom;
		}
		else if (CardCount[ECardIndex::CardIndex_LargeJoker] == 1) {// 大王
			laiZiIndex = 101;
			r = JokerBoom;
		}
		else {
			auto laiZiIndexIter = std::find(CardCount.begin(), CardCount.end(), 1);
			if (laiZiIndexIter != CardCount.end()) {
				laiZiIndex = laiZiIndexIter - CardCount.begin();
				r = DoubleStyle(laiZiIndex);
			}
		}
		break;
	}
	case 3:
	{
		auto laiZiIndexIter = std::find(CardCount.begin(), CardCount.end(), 2);
		if (laiZiIndexIter != CardCount.end()) {
			laiZiIndex = laiZiIndexIter - CardCount.begin();
			r = TripleZeroStyle(laiZiIndex);
		}
		break;
	}
	case 4:
	{
		auto laiZiIndexIter = std::find(CardCount.begin(), CardCount.end(), 3);
		if (laiZiIndexIter != CardCount.end()) {
			laiZiIndex = laiZiIndexIter - CardCount.begin();
			r = BoomStyle(laiZiIndex);
		}else{
			auto laiZiIndexIter = std::find(CardCount.begin(), CardCount.end(), 2);
			if (laiZiIndexIter != CardCount.end()) {
				laiZiIndex = laiZiIndexIter - CardCount.begin();
				uint8_t extra = std::find(CardCount.begin(), CardCount.end(), 1) - CardCount.begin();
				r = TripleOneStyle(laiZiIndex, { extra });
			}
		}
		break;
	}
	case 5:
	{
		auto tripleIndexIter = std::find(CardCount.begin(), CardCount.end(), 3);
		if (tripleIndexIter != CardCount.end()) {
			auto tripleIndex = tripleIndexIter - CardCount.begin();
			auto laiZiIndexIter = std::find(CardCount.begin(), CardCount.end(), 1);
			if (laiZiIndex >= 0)
			{
				laiZiIndex = laiZiIndexIter - CardCount.begin();
				r = TripleTwoStyle(tripleIndex, { laiZiIndex });
			}
		}
		else {
			auto extraIndexIter = std::find(CardCount.begin(), CardCount.end(), 2);
			auto tripleIndexIter = std::find(extraIndexIter + 1, CardCount.end(), 2);
			if (extraIndexIter != CardCount.end() && tripleIndexIter != CardCount.end() && tripleIndexIter > extraIndexIter)
			{
				laiZiIndex = tripleIndexIter - CardCount.begin();
				uint8_t extraIndex = extraIndexIter - CardCount.begin();
				r = TripleTwoStyle(laiZiIndex, { extraIndex });
			}
		}
		break;
	}
	}
	ret:
	if (laiZiIndex > 100)
	{
		laiZiIndex = laiZiIndex - 20;
	}
	laiZiIndex = HandCardsFlag::CardColorIndexToValue(4, laiZiIndex);
	return r;
}

CardStyle CardStyle::CheckIfLaiziSingleChain(CardVector & cardCount, CardVector & cards, uint8_t & laiZi)
{
	auto StartIndexIter = std::find_if(cardCount.begin(), cardCount.end(), [](uint8_t x) -> bool { return x == 1; });
	auto StartIndex = StartIndexIter - cardCount.begin();
	int chainLen = (cards.size() + 1) / 1;
	int EndIndex = StartIndex + chainLen - 1;
	if (StartIndex == ECardIndex::CardIndex_J)
	{
		StartIndex = ECardIndex::CardIndex_10;
	}
	if (StartIndex >= 0 && StartIndex <= ECardIndex::CardIndex_A - 4 && EndIndex <= ECardIndex::CardIndex_A)
	{
		int ChainCount = 0;
		for (int i = StartIndex; i < StartIndex + chainLen; ++i)
		{
			if (cardCount[i] == 1)
			{
				++ChainCount;
			}
			if (cardCount[i] == 0)
			{
				laiZi = i;
			}
		}
		if (ChainCount == chainLen - 1)
		{
			return SingleChainStyle(StartIndex, StartIndex + chainLen - 1);
		}
	}
	return Invalid;
}

CardStyle CardStyle::CheckIfLaiziDoubleChain(CardVector & cardCount, CardVector & cards, uint8_t & laiZi)
{
	auto StartIndexIter = std::find_if(cardCount.begin(), cardCount.end(), [](uint8_t x) -> bool { return x == 1 || x == 2; });
	auto StartIndex = StartIndexIter - cardCount.begin();
	int chainLen = (cards.size() + 1) / 2;
	int EndIndex = StartIndex + chainLen - 1;
	if (StartIndex >= 0 && StartIndex <= ECardIndex::CardIndex_A - 2 && EndIndex <= ECardIndex::CardIndex_A)
	{
		int ChainCount = 0;
		for (int i = StartIndex; i < StartIndex + chainLen; ++i)
		{
			if (cardCount[i] == 2)
			{
				++ChainCount;
			}
			if (cardCount[i] == 1)
			{
				laiZi = i;
			}
		}
		if (ChainCount == chainLen - 1)
		{
			return DoubleChainStyle(StartIndex, StartIndex + chainLen - 1);
		}
	}
	return Invalid;
}

CardStyle CardStyle::CheckIfLaiziTripleChainZero(CardVector & cardCount, CardVector & cards, uint8_t & laiZi)
{
	auto StartIndexIter = std::find_if(cardCount.begin(), cardCount.end(), [](uint8_t x) -> bool { return x == 2 || x == 3; });
	auto StartIndex = StartIndexIter - cardCount.begin();
	int chainLen = (cards.size() + 1) / 3;
	int EndIndex = StartIndex + chainLen - 1;
	if (StartIndex >= 0 && StartIndex <= ECardIndex::CardIndex_A - 1 && EndIndex <= ECardIndex::CardIndex_A)
	{
		int ChainCount = 0;
		for (int i = StartIndex; i < StartIndex + chainLen; ++i)
		{
			if (cardCount[i] == 3)
			{
				++ChainCount;
			}
			if (cardCount[i] == 2)
			{
				laiZi = i;
				++ChainCount;
			}
		}
		if (ChainCount == chainLen)
		{
			return TripleChainZeroStyle(StartIndex, StartIndex + chainLen - 1);
		}
	}
	return Invalid;
}

CardStyle CardStyle::CheckIfLaiziTripleChainOne(CardVector & cardCount, CardVector & cards, uint8_t & laiZi)
{
	auto newCards = cards;
	int chainLen = (newCards.size() + 1) / 4;
	auto StartIndexIter = cardCount.begin();
	int StartIndex = -1;
	while (StartIndex <= ECardIndex::CardIndex_A - 1)
	{
		if (StartIndexIter != cardCount.begin())
			++StartIndexIter;
		StartIndexIter = std::find_if(StartIndexIter, cardCount.end(), [](uint8_t x) -> bool { return x == 2 || x == 3 || x == 4; });
		StartIndex = StartIndexIter - cardCount.begin();
		int EndIndex = StartIndex + chainLen - 1;
		if (StartIndexIter != cardCount.end() && StartIndex <= ECardIndex::CardIndex_A - 1 && EndIndex <= ECardIndex::CardIndex_A)
		{
			int ChainCount = 0;
			CardVector DoubleCards;
			CardVector SingleCards;

			for (int i = StartIndex; i < StartIndex + chainLen; ++i) {
				if (cardCount[i] == 3) {
					++ChainCount;
				}
				if (cardCount[i] == 2) {
					DoubleCards.push_back(i);
				}
				if (cardCount[i] == 4) {
					++ChainCount;
					SingleCards.push_back(i);//额外的单张，因为下面RemoveAll删掉了这些牌
				}
			}

			if (ChainCount == chainLen)
			{
				newCards.erase(std::remove_if(newCards.begin(), newCards.end(), [StartIndex, chainLen](uint8_t x)->bool
				{
					auto index = HandCardsFlag::CardValueToIndex(x);
					return index >= StartIndex && index < StartIndex + chainLen;
				}), newCards.end());
				laiZi = ECardIndex::CardIndex_2;

				CardVector extra;
				for (auto v : newCards) {
					extra.push_back(HandCardsFlag::CardValueToIndex(v));
				}
				return TripleChainOneStyle(StartIndex, StartIndex + chainLen - 1, extra);
			}
			if (ChainCount == chainLen - 1 && DoubleCards.size() == 1) {
				newCards.erase(std::remove_if(newCards.begin(), newCards.end(), [StartIndex, chainLen](uint8_t x)->bool
				{
					auto index = HandCardsFlag::CardValueToIndex(x);
					return index >= StartIndex && index < StartIndex + chainLen;
				}), newCards.end());
				CardVector extra;
				for (auto v : newCards) {
					extra.push_back(HandCardsFlag::CardValueToIndex(v));
				}
				laiZi = DoubleCards[0];
				return TripleChainOneStyle(StartIndex, StartIndex + chainLen - 1, extra);
			}
		}
		else {
			return Invalid;
		}
	}
	return Invalid;
}

CardStyle CardStyle::CheckIfLaiziTripleChainTwo(CardVector & cardCount, CardVector & cards, uint8_t & laiZi)
{
	int chainLen = (cards.size() + 1) / 5;
	auto StartIndexIter = cardCount.begin();
	int StartIndex = -1;
	while (StartIndex <= ECardIndex::CardIndex_A - 1)
	{
		if (StartIndexIter != cardCount.begin())
			++StartIndexIter;
		StartIndexIter = std::find_if(StartIndexIter, cardCount.end(), [](uint8_t x) -> bool { return x == 2 || x == 3; });
		StartIndex = StartIndexIter - cardCount.begin();
		int EndIndex = StartIndex + chainLen - 1;
		if (StartIndexIter != cardCount.end() && StartIndex <= ECardIndex::CardIndex_A - 1 && EndIndex <= ECardIndex::CardIndex_A) {
			int ChainCount = 0;
			CardVector DoubleCards;
			for (int i = StartIndex; i < StartIndex + chainLen; ++i) {
				if (cardCount[i] == 3) {
					++ChainCount;
				}
				if (cardCount[i] == 2) {
					DoubleCards.push_back(i);
				}
			}
			if (ChainCount == chainLen && DoubleCards.size() == 0)//癞子牌做带的
			{
				CardVector ExtraSingleCards;
				CardVector ExtraDoubleCards;
				auto leftCardCount = GetCardCount(cards);
				for (auto index = 0; index < leftCardCount.size(); ++index)
				{
					if (index >= StartIndex && index < StartIndex + chainLen)
						continue;
					if (leftCardCount[index] == 1) {
						ExtraSingleCards.push_back(index);
					}
					if (leftCardCount[index] == 2) {
						ExtraDoubleCards.push_back(index);
					}
					if (leftCardCount[index] == 3) {
						ExtraSingleCards.push_back(index);
						ExtraDoubleCards.push_back(index);
					}
					if (leftCardCount[index] == 4) {
						ExtraDoubleCards.push_back(index);
						ExtraDoubleCards.push_back(index);
					}
				}
				if (ExtraSingleCards.size() == 1 && ExtraDoubleCards.size() == chainLen - 1) {
					laiZi = ExtraSingleCards[0];
					CardVector unionSet;
					std::set_union(ExtraSingleCards.begin(), ExtraSingleCards.end(), ExtraDoubleCards.begin(), ExtraDoubleCards.end(), std::back_inserter(unionSet));
					return TripleChainTwoStyle(StartIndex, StartIndex + chainLen - 1, unionSet);
				}
			}
			if (ChainCount == chainLen - 1 && DoubleCards.size() == 1)//癞子牌做主体
			{
				CardVector ExtraDoubleCards;
				auto leftCardCount = GetCardCount(cards);
				for (int index = 0; index < leftCardCount.size(); ++index) {
					if (index >= StartIndex && index < StartIndex + chainLen)
						continue;
					if (leftCardCount[index] == 2) {
						ExtraDoubleCards.push_back(index);
					}
					if (leftCardCount[index] == 4) {
						ExtraDoubleCards.push_back(index);
						ExtraDoubleCards.push_back(index);
					}
				}
				if (ExtraDoubleCards.size() == chainLen) {
					laiZi = DoubleCards[0];
					return TripleChainTwoStyle(StartIndex, StartIndex + chainLen - 1, ExtraDoubleCards);
				}
			}
		}
		else {
			return Invalid;
		}
	}
	return Invalid;
}

CardStyle CardStyle::SingleStyle(uint8_t value)
{
	return CardStyle(ECardStyle::Single, value);
}

CardStyle CardStyle::DoubleStyle(uint8_t value)
{
	return CardStyle(ECardStyle::Double, value);
}

CardStyle CardStyle::TripleZeroStyle(uint8_t value)
{
	return CardStyle(ECardStyle::Triple_Zero, value);
}

CardStyle CardStyle::TripleOneStyle(uint8_t value, const CardVector &extra)
{
	return CardStyle(ECardStyle::Triple_One, value, value, extra);
}

CardStyle CardStyle::TripleTwoStyle(uint8_t value, const CardVector & extra)
{
	return CardStyle(ECardStyle::Triple_Two, value, value, extra);
}

CardStyle CardStyle::BoomStyle(uint8_t value)
{
	return CardStyle(ECardStyle::Boom, value);
}

CardStyle CardStyle::QuadSingleStyle(uint8_t value, const CardVector & extra)
{
	return CardStyle(ECardStyle::Quad_Single, value, value, extra);
}

CardStyle CardStyle::QuadDoubleStyle(uint8_t value, const CardVector & extra)
{
	return CardStyle(ECardStyle::Quad_Double, value, value, extra);
}

CardStyle CardStyle::SingleChainStyle(uint8_t startValue, uint8_t endValue)
{
	return CardStyle(ECardStyle::Single_Chain, startValue, endValue);
}

CardStyle CardStyle::DoubleChainStyle(uint8_t startValue, uint8_t endValue)
{
	return CardStyle(ECardStyle::Double_Chain, startValue, endValue);
}

CardStyle CardStyle::TripleChainZeroStyle(uint8_t startValue, uint8_t endValue)
{
	return CardStyle(ECardStyle::Triple_Chain_Zero, startValue, endValue);
}

CardStyle CardStyle::TripleChainOneStyle(uint8_t startValue, uint8_t endValue, const CardVector & extra)
{
	return CardStyle(ECardStyle::Triple_Chain_One, startValue, endValue, extra);
}

CardStyle CardStyle::TripleChainTwoStyle(uint8_t startValue, uint8_t endValue, const CardVector & extra)
{
	return CardStyle(ECardStyle::Triple_Chain_Two, startValue, endValue);
}


int CardStyle::Compare(const CardStyle & style)const
{
	bool isPBoom = Style == ECardStyle::Boom;
	bool isABoom = style.Style == ECardStyle::Boom;

	if (Style == style.Style &&  StartValue == EndValue && EndValue == style.EndValue) {
		return 0;
	}

	if (isPBoom == isABoom) {
		if (Style == style.Style) {
			switch (Style) {
			case ECardStyle::Single:
			case ECardStyle::Double:
			case ECardStyle::Triple_Zero:
			case ECardStyle::Triple_Two:
			case ECardStyle::Triple_One:
			case ECardStyle::Boom:
			case ECardStyle::Quad_Double:
			case ECardStyle::Quad_Single:
				if (StartValue > style.StartValue) {
					return 1;
				}
				else {
					return -1;
				}
			case ECardStyle::Single_Chain:
			case ECardStyle::Double_Chain:
			case ECardStyle::Triple_Chain_Zero:
			case ECardStyle::Triple_Chain_One:
			case ECardStyle::Triple_Chain_Two:
				if ((EndValue - StartValue) == (style.EndValue - style.StartValue)) {
					if (StartValue > style.StartValue) {
						return 1;
					}
					else {
						return -1;
					}
				}
				else {
					return 0;
				}
			}
		}
		else {
			return 0;
		}
	}
	else {
		if (isPBoom) {
			return 1;
		}
		if (isABoom) {
			return -1;
		}
	}
	return 0;
}
std::string CardStyle::StyleString()const {
	switch (Style)
	{
	case ECardStyle::Invalid:
		return ("Invalid");
	case ECardStyle::Boom:
		return("Boom");
	case ECardStyle::Triple_Zero:
		return("Triple_Zero");
	case ECardStyle::Triple_Two:
		return("Triple_Two");
	case ECardStyle::Triple_One:
		return("Triple_One");
	case ECardStyle::Double:
		return("Double");
	case ECardStyle::Double_Chain:
		return("Double_Chain");
	case ECardStyle::Single:
		return("Single");
	case ECardStyle::Single_Chain:
		return("Single_Chain");
	case ECardStyle::Triple_Chain_Zero:
		return("Triple_Chain_Zero");
	case ECardStyle::Triple_Chain_One:
		return("Triple_Chain_One");
	case ECardStyle::Triple_Chain_Two:
		return("Triple_Chain_Two");
	case ECardStyle::Quad_Single:
		return("Quad_Single");
	case ECardStyle::Quad_Double:
		return("Quad_Double");
	default:
		break;
	}
	return "";
}
std::string CardStyle::ToString()const {
	if (Style == ECardStyle::Boom && StartValue > CardIndex_2) {
		return "Joker Boom";
	}
	std::string r(StyleString());

	r.push_back(' ');
	if (EndValue == StartValue) {
		r.append(CardNameTable[StartValue]);
	}
	else {
		r.append(CardNameTable[StartValue]);
		r.append(" - ");
		r.append(CardNameTable[EndValue]);
	}
	if (Extra.size() == 0)
		return r;
	r.append("  [");
	for (auto &v : Extra) {
		r.append(CardNameTable[v]);
	}
	r.push_back(']');
	return r;
}

bool CardStyle::IsBigCard()const
{
	switch (Style)
	{
	case ECardStyle::Boom:
	case ECardStyle::Quad_Single:
		return true;
	case ECardStyle::Triple_Zero:
	case ECardStyle::Triple_One:
	case ECardStyle::Triple_Two:
		return StartValue >= CardIndex_J;
	case ECardStyle::Double:
		return StartValue >= CardIndex_K;
	case ECardStyle::Single:
		return StartValue >= CardIndex_2;
	default:
		return EndValue >= CardIndex_J;
		break;
	}
	return false;
}


CardStyle& CardStyle::operator=(const CardStyle& in) {
	if (&in != this) {
		Style = in.Style;
		StartValue = in.StartValue;
		EndValue = in.EndValue;
		Extra = in.Extra;
	}
	return *this;
}

bool CardStyle::operator==(const CardStyle & in)const
{
	return Style == in.Style && StartValue == in.StartValue && EndValue == in.EndValue && Extra == in.Extra;
}

bool CardStyle::operator!=(const CardStyle & in)const
{
	return Style != in.Style || StartValue != in.StartValue || EndValue != in.EndValue || Extra != in.Extra;
}
