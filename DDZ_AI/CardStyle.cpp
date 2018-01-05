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
	case ECardStyle::Boom:
		return 4;
	case ECardStyle::Quad_Two:
		return 6;
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
	if (isChain)
		return CardStyle::SingleChainStyle(*indexCards.begin(), *(indexCards.end() - 1));

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
			if (cards[0] + cards[1] == 3) {
				return CardStyle::JokerBoom;
			}
			else {
				//logger.Error("无效的2张牌")
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
		for(auto &v:m){
			if (v.second == 3) {
				triple = v.first;
			}
			if (v.second == 2) {
				extra = v.first;
			}
		}
		if (triple > -1 && extra > -1) {
			return CardStyle(ECardStyle::Triple_Two, uint8_t(triple), uint8_t(triple),{ uint8_t(extra) });
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
			auto min_max = std::minmax_element(doubleChain.begin(), doubleChain.end());
			return CardStyle(ECardStyle::Double_Chain, *min_max.first, *min_max.second);
		}
		if (tripleChain.size() == 2) {
			auto min_max = std::minmax_element(tripleChain.begin(), tripleChain.end());
			if (*min_max.second - *min_max.first == 1) {
				return CardStyle(ECardStyle::Triple_Chain_Zero, *min_max.first, *min_max.second);
			}
		}
		else {
			if (boom > -1 && extra.size() == 2)
				return CardStyle(ECardStyle::Quad_Two, boom, boom, extra);
		}
		break;

	}
	case 8: {
		auto m = cardCountMap(indexCards);
		CardVector tripleChain;
		CardVector extra;
		CardVector doubleChain;
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
				extra.push_back(kv.first);
				extra.push_back(kv.first);
				doubleChain.push_back(kv.first);
			}
		}
		if (doubleChain.size() == 4) {
			auto min_max = std::minmax_element(doubleChain.begin(), doubleChain.end());
			if (*min_max.second - *min_max.first == 3) {
				return CardStyle(ECardStyle::Double_Chain, *min_max.first, *min_max.second);
			}
		}
		if (tripleChain.size() == 2) {
			auto min_max = std::minmax_element(tripleChain.begin(), tripleChain.end());
			if (*min_max.second - *min_max.first == 1) {
				return CardStyle(ECardStyle::Triple_Chain_One, *min_max.first, *min_max.second, extra);
			}
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
			auto min_max = std::minmax_element(tripleChain.begin(), tripleChain.end());
			if (*min_max.second - *min_max.first == 2) {
				return CardStyle(ECardStyle::Triple_Chain_Zero, *min_max.first, *min_max.second);
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
			auto min_max = std::minmax_element(doubleChain.begin(), doubleChain.end());
			if (*min_max.second - *min_max.first == 4) {
				return CardStyle(ECardStyle::Double_Chain, *min_max.first, *min_max.second);
			}
		}
		if (tripleChain.size() == 2 && extra.size() == 2) {
			auto min_max = std::minmax_element(tripleChain.begin(), tripleChain.end());
			if (*min_max.second - *min_max.first == 1) {
				return CardStyle(ECardStyle::Triple_Chain_Two, *min_max.first, *min_max.second, extra);
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
			auto min_max = std::minmax_element(doubleChain.begin(), doubleChain.end());
			if (*min_max.second - *min_max.first == 5) {
				return CardStyle(ECardStyle::Double_Chain, *min_max.first, *min_max.second);
			}
		}
		auto min_max = std::minmax_element(tripleChain.begin(), tripleChain.end());
		if (tripleChain.size() == 4) {
			if (*min_max.second - *min_max.first == 3) {
				return CardStyle(ECardStyle::Triple_Chain_Zero, *min_max.first, *min_max.second);
			}
		}
		if (tripleChain.size() == 3 && extra.size() == 3) {
			if (*min_max.second - *min_max.first == 2) {
				return CardStyle(ECardStyle::Triple_Chain_One, *min_max.first, *min_max.second, extra);
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
			auto min_max = std::minmax_element(doubleChain.begin(), doubleChain.end());

			if (*min_max.second - *min_max.first == 6) {
				return CardStyle(ECardStyle::Double_Chain, *min_max.first, *min_max.second);
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
		auto min_max = std::minmax_element(tripleChain.begin(), tripleChain.end());
		if (tripleChain.size() == 5) {
			if (*min_max.second - *min_max.first == 4) {
				return CardStyle(ECardStyle::Triple_Chain_Zero, *min_max.first, *min_max.second);
			}
		}
		if (tripleChain.size() == 3 && extra.size() == 3) {
			if (*min_max.second - *min_max.first == 2) {
				return CardStyle(ECardStyle::Triple_Chain_Two, *min_max.first, *min_max.second, extra);
			}
		}
		break;
	}
	}
			 return CardStyle::Invalid;
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

	CardStyle CardStyle::QuadTwoStyle(uint8_t value, const CardVector & extra)
	{
		return CardStyle(ECardStyle::Quad_Two, value, value, extra);
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

		if (Style == style.Style &&  StartValue == EndValue &&  EndValue == style.EndValue) {
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
		case ECardStyle::Quad_Two:
			return("Quad_Two");
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
		case ECardStyle::Quad_Two:
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
