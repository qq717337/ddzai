#include "stdafx.h"
#include "SplitType.h"
#include "common_algorithm.h"
#include <algorithm>

SplitType::SplitType()
{
}

SplitType::~SplitType()
{
}
SplitType & SplitType::operator=(const SplitType & rhs)
{
	if (this != &rhs)
	{
		VECTOR_COPY(rhs.Boom, Boom);
		VECTOR_COPY(rhs.Single, Single);
		VECTOR_COPY(rhs.Double, Double);
		VECTOR_COPY(rhs.Triple, Triple);
		VECTOR_COPY(rhs.SingleChain, SingleChain);
		VECTOR_COPY(rhs.DoubleChain, DoubleChain);
		VECTOR_COPY(rhs.TripleChain, TripleChain);
	}
	return *this;
}

size_t SplitType::MinStepCount()
{
	size_t singleCount = Single.size();
	size_t	doubleCount = Double.size();

	for (auto &v : TripleChain) {
		size_t tripleChainCount = (v.End - v.Start) + 1;
		if (tripleChainCount <= singleCount) {
			singleCount -= tripleChainCount;
		}
		else {
			if (tripleChainCount <= doubleCount) {
				doubleCount -= tripleChainCount;
			}
			else {
				if (tripleChainCount == 2) {
					doubleCount -= 1;
				}
			}
		}
	}
	int extra = singleCount + doubleCount;
	int tripleCount = Triple.size();
	int diff = tripleCount - extra; //三个头的个数减去单只和双只的个数，如果大于0，则追加步数，否则，算到单只里面就不需要额外的步数了
	if (diff < 0) {
		diff = 0;
	}
	size_t totalStep = TripleChain.size() + SingleChain.size() + DoubleChain.size() + Boom.size() + extra + diff;
	return totalStep;
}
bool SplitType::GetLastShotCardStyles(CardStyle* ref) {
	if (MinStepCount() == 1) {
		if (TripleChain.size() > 0) {
			if (Single.size() == TripleChain[0].Length()) {
				*ref = CardStyle(ECardStyle::Triple_Chain_One, TripleChain[0].Start, TripleChain[0].End, Single);
				return true;
			}
			if (Double.size() == TripleChain[0].Length()) {
				*ref = CardStyle(ECardStyle::Triple_Chain_Two, TripleChain[0].Start, TripleChain[0].End, Double);
				return true;
			}
			else {
				if (Double.size() * 2 == TripleChain[0].Length()) {
					*ref = CardStyle(ECardStyle::Triple_Chain_Two, TripleChain[0].Start, TripleChain[0].End, std::vector<uint8_t>{ Double[0], Double[0] });
					return true;
				}
				else {
					*ref = CardStyle(ECardStyle::Triple_Chain_Zero, TripleChain[0].Start, TripleChain[0].End);
					return true;
				}
			}
		}
		if (DoubleChain.size() > 0) {
			*ref = CardStyle(ECardStyle::Double_Chain, DoubleChain[0].Start, DoubleChain[0].End);
			return true;
		}
		if (SingleChain.size() > 0) {
			*ref = CardStyle(ECardStyle::Single_Chain, SingleChain[0].Start, SingleChain[0].End);
			return true;
		}
		if (Boom.size() > 0) {
			if (Boom[0] == CardIndex_JokerBoom) {
				*ref = CardStyle::JokerBoom;
			}
			else {
				*ref = CardStyle(ECardStyle::Boom, Boom[0]);
			}
			return true;
		}
		if (Triple.size() > 0) {
			if (Single.size() > 0) {
				*ref = CardStyle(ECardStyle::Triple_One, Triple[0], Single);
				return true;
			}
			else if (Double.size() > 0) {
				*ref = CardStyle(ECardStyle::Triple_Two, Triple[0], Double);
				return true;
			}
			else {
				*ref = CardStyle(ECardStyle::Triple_Zero, Triple[0]);
				return true;
			}
		}
		if (Double.size() > 0) {
			*ref = CardStyle(ECardStyle::Double, Double[0]);
			return true;
		}
		if (Single.size() > 0) {
			*ref = CardStyle(ECardStyle::Single, Single[0]);
			return true;
		}
	}
	return false;
}

void SplitType::AddBoom(uint8_t cardIndex)
{
	INSERT_SPLITTYPE(Boom);
}

void SplitType::AddSingle(uint8_t cardIndex)
{
	INSERT_SPLITTYPE(Single);
}

void SplitType::AddDouble(uint8_t cardIndex)
{
	INSERT_SPLITTYPE(Double);
}

void SplitType::AddTriple(uint8_t cardIndex)
{
	INSERT_SPLITTYPE(Triple);
}

void SplitType::AddSingleChain(uint8_t startIndex, uint8_t endIndex)
{
	INSERT_CHAIN_SPLITTYPE(SingleChain);
}

void SplitType::AddDoubleChain(uint8_t startIndex, uint8_t endIndex)
{
	INSERT_CHAIN_SPLITTYPE(DoubleChain);
}

void SplitType::AddTripleChain(uint8_t startIndex, uint8_t endIndex)
{
	INSERT_CHAIN_SPLITTYPE(TripleChain);
}

//最小牌值出法,34567,这种先出 556677，后出，排除炸弹
//如果有三个带的，则优先考虑使用三个带出牌，而不是用单只或者对子
CardStyle SplitType::MinValueCardStyle() {
	uint8_t minIndex = CardIndex_LargeJoker + 1;
	CardStyle r;

	bool ignoreSingle = false;
	bool ignoreDouble = false;

	size_t	singleLen = Single.size();
	size_t doubleLen = Double.size();
	size_t tripleLen = Triple.size();
	size_t	tripleChainLen = 0;
	if (TripleChain.size() > 0) {
		tripleChainLen = TripleChain[0].Length();
		uint8_t tripleChainStart = TripleChain[0].Start;
		if (tripleChainLen == singleLen) { //如果Chain的长度大于单只的数目，则不能忽略
			ignoreSingle = true;
		}
		if (tripleChainLen < singleLen) {
			size_t 	smallerSingleCount = smallerCount<uint8_t>(Single, tripleChainStart);
			if (smallerSingleCount <= tripleChainLen) {
				ignoreSingle = true;
			}
		}

		if (tripleChainLen == doubleLen) { //如果Chain的长度大于单只的数目，则不能忽略
			ignoreDouble = true;
		}
		if (tripleChainLen < doubleLen) {
			size_t	smallerDoubleCount = smallerCount(Double, tripleChainStart);
			if (smallerDoubleCount <= tripleChainLen) {
				ignoreDouble = true;
			}
		}
		if (TripleChain[0].Length() == 3 && doubleLen == 1 && singleLen == 1) {
			ignoreDouble = true;
			ignoreSingle = true;
		}
	}

	if (tripleLen > 0) {
		if (tripleLen == singleLen) { //如果Chain的长度大于单只的数目，则不能忽略
			ignoreSingle = true;
		}
		if (tripleLen < singleLen) {
			size_t smallerSingleCount = smallerCount(Single, Triple[0]);
			if (smallerSingleCount <= tripleLen) {
				ignoreSingle = true;
			}
		}
		if (tripleLen == doubleLen) { //如果Chain的长度大于单只的数目，则不能忽略
			ignoreDouble = true;
		}
		if (tripleLen < doubleLen) {
			size_t smallerDoubleCount = smallerCount(Double, Triple[0]);
			if (smallerDoubleCount <= tripleLen) {
				ignoreDouble = true;
			}
		}

		if (tripleChainLen + tripleLen == doubleLen + singleLen) {
			ignoreDouble = true;
			ignoreSingle = true;
		}
	}
	if (!ignoreSingle && singleLen > 0 && Single[0] <= minIndex) {
		minIndex = Single[0];
		r = CardStyle{ ECardStyle::Single,Single[0] };
	}
	if (!ignoreDouble && doubleLen > 0 && Double[0] <= minIndex) {
		minIndex = Double[0];
		r = CardStyle{ ECardStyle::Double,Double[0] };
	}
	if (tripleLen > 0 && Triple[0] <= minIndex) {
		minIndex = Triple[0];
		r = GetTripleStyle();
	}
	if (SingleChain.size() > 0 && SingleChain[0].Start <= minIndex) {
		minIndex = SingleChain[0].Start;
		r = CardStyle(ECardStyle::Single_Chain, SingleChain[0].Start, SingleChain[0].End);
	}
	if (DoubleChain.size() > 0 && DoubleChain[0].Start <= minIndex) {
		minIndex = DoubleChain[0].Start;
		r = CardStyle(ECardStyle::Double_Chain, DoubleChain[0].Start, DoubleChain[0].End);
	}
	if (TripleChain.size() > 0 && TripleChain[0].Start <= minIndex) {
		minIndex = TripleChain[0].Start;
		r = GetTripleChainStyle();
	}

	if (r.Style == ECardStyle::Invalid) {
		if (Boom.size() > 0) {
			if (Boom[0] > CardIndex_2) {
				r = CardStyle::JokerBoom;
			}
			else {
				r = CardStyle(ECardStyle::Boom, Boom[0]);
			}
		}
	}
	return r;
}


CardStyle SplitType::GetSingleChainStyle() {
	if (SingleChain.size() > 0) {
		return CardStyle(ECardStyle::Single_Chain, SingleChain[0].Start, SingleChain[0].End);
	}
	return CardStyle::Invalid;
}
CardStyle SplitType::GetDoubleChainStyle() {
	if (DoubleChain.size() > 0) {
		return CardStyle(ECardStyle::Double_Chain, DoubleChain[0].Start, DoubleChain[0].End);
	}
	return CardStyle::Invalid;
}


CardStyle SplitType::GetTripleChainStyle() {
	CardStyle r;
	if (TripleChain.size() == 0) {
		return CardStyle::Invalid;
	}
	size_t chainLen = TripleChain[0].Length();

	size_t singleLen = Single.size();
	size_t doubleLen = Double.size();

	if (singleLen >= chainLen) {
		if (doubleLen < chainLen) {
			r = CardStyle(ECardStyle::Triple_Chain_One, TripleChain[0].Start, TripleChain[0].End, std::vector<uint8_t>(Single.begin(), Single.begin() + chainLen));
		}
		else {
			if (Single[0] + Single[1] < Double[0] + Double[1]) {
				r = CardStyle(ECardStyle::Triple_Chain_One, TripleChain[0].Start, TripleChain[0].End, std::vector<uint8_t>(Single.begin(), Single.begin() + chainLen));
			}
			else {
				r = CardStyle(ECardStyle::Triple_Chain_Two, TripleChain[0].Start, TripleChain[0].End, std::vector<uint8_t>(Double.begin(), Double.begin() + chainLen));
			}
		}
	}
	else {
		if (doubleLen >= chainLen) {
			r = CardStyle(ECardStyle::Triple_Chain_Two, TripleChain[0].Start, TripleChain[0].End, std::vector<uint8_t>(Double.begin(), Double.begin() + chainLen));
		}
		else {
			r = CardStyle(ECardStyle::Triple_Chain_Zero, TripleChain[0].Start, TripleChain[0].End);
		}
	}
	return r;
}

CardStyle SplitType::GetTripleStyle() {
	CardStyle r;
	if (Triple.size() == 0) {
		return CardStyle::Invalid;
	}
	size_t singleLen = Single.size();
	size_t  doubleLen = Double.size();
	if (singleLen > 0) {
		if (doubleLen == 0) {
			r = CardStyle(ECardStyle::Triple_One, Triple[0], Triple[0], { Single[0] });
		}
		else {
			if (Single[0] < Double[0]) {
				r = CardStyle(ECardStyle::Triple_One, Triple[0], Triple[0], { Single[0] });
			}
			else {
				r = CardStyle(ECardStyle::Triple_Two, Triple[0], Triple[0], { Double[0] });
			}
		}
	}
	else {
		if (doubleLen > 0) {
			r = CardStyle(ECardStyle::Triple_Two, Triple[0], Triple[0], { Double[0] });
		}
		else {
			CardStyle(ECardStyle::Triple_Zero, Triple[0], Triple[0]);
		}
	}
	return r;
}
size_t SplitType::CardCount() {
	size_t sum = Boom.size() * 4 + Triple.size() * 3 + Double.size() * 2 + Single.size();
	for (auto& v : SingleChain) {
		sum += v.Length();
	}
	for (auto& v : DoubleChain) {
		sum += v.Length() * 2;
	}
	for (auto& v : TripleChain) {
		sum += v.Length() * 3;
	}
	return sum;
}
bool SplitType::RequireSingleFromChain(size_t requireCount, std::vector<uint8_t>& out, bool forceSplit)const
{
	if (SingleChain.size() == 0) {
		return false;
	}
	out.resize(requireCount);
	for (auto & v : SingleChain) { //一般可以忽略双飞的同事还会出现两个对子的情况，但是为了兼顾三张，所以遍历下，第一种情况 可以从
		if (v.Length() - requireCount >= 5) {
			for (int i = 0; i < requireCount; ++i) {
				out[i] = SingleChain[0].Start + i;
			}
			return true;
		}
	}
	//如果无法实现完美的拆分，则破坏顺子，破坏顺子的前提是找张数一致的进行优先破坏，这里判断是不是单张
	if (forceSplit) {
		int	appendCount = 0;
		std::vector<uint8_t>	exclude;
		for (uint8_t i = SingleChain[0].Start; i < SingleChain[0].End; ++i) {
			if (InSingle(i) || InDouble(i)) {
				exclude.push_back(i);
				continue;
			}
			out[appendCount] = i;
			appendCount++;
			if (appendCount == requireCount) {
				break;
			}
		}
		for (int j = appendCount; j < requireCount; j++) {
			out[appendCount] = exclude[requireCount - j - 1];
		}
		return true;
	}

	return false;
}
 bool SplitType::RequireDoubleFromChain(size_t requireCount, std::vector<uint8_t>& out, bool forceSplit)const
{
	if (DoubleChain.size() == 0) {
		return false;
	}
	out.resize(requireCount);
	for (auto & v : DoubleChain) { //一般可以忽略双飞的同事还会出现两个对子的情况，但是为了兼顾三张，所以遍历下，第一种情况 可以从
		if (v.Length() - requireCount >= 5) {
			for (int i = 0; i < requireCount; ++i) {
				out[i] = DoubleChain[0].Start + i;
			}
			return true;
		}
	}
	//如果无法实现完美的拆分，则破坏顺子，破坏顺子的前提是找张数一致的进行优先破坏，这里判断是不是单张
	if (forceSplit) {
		int	appendCount = 0;
		std::vector<uint8_t>	exclude;
		for (uint8_t i = DoubleChain[0].Start; i < DoubleChain[0].End; ++i) {
			if (InSingle(i) || InDouble(i)) {
				exclude.push_back(i);
				continue;
			}
			out[appendCount] = i;
			appendCount++;
			if (appendCount == requireCount) {
				break;
			}
		}
		for (int j = appendCount; j < requireCount; j++) {
			out[appendCount] = exclude[requireCount - j - 1];
		}
		return true;
	}

	return false;
}
void SplitType::Reset()
{
	Boom.clear();
	Single.clear();
	Double.clear();
	Triple.clear();
	SingleChain.clear();  //至少5张
	DoubleChain.clear(); //至少3张
	TripleChain.clear();//至少2张
}
#define OUTPUT_SPLITTYPE_VALUE(variable_name,x) if(x.size()>0){\
ostr << variable_name<<":{";\
for (auto & v : x) {\
	ostr << CardNameTable[v] << ',';\
}\
long pos = ostr.tellp();\
ostr.seekp(pos - 1);\
ostr << "}\n";\
}

#define OUTPUT_SPLITTYPE_RANGE(variable_name,x) if (x.size() > 0) {\
ostr << variable_name << ":{";\
for (auto & v : x) {\
	ostr << '[' << CardNameTable[v.Start] << '-' << CardNameTable[v.End] << "],";\
}\
long pos = ostr.tellp();\
ostr.seekp(pos - 1);\
ostr << "}\n";\
}


std::string SplitType::ToString() {
	std::ostringstream ostr;

	OUTPUT_SPLITTYPE_VALUE("Single", Single)
		OUTPUT_SPLITTYPE_VALUE("Double", Double)
		OUTPUT_SPLITTYPE_VALUE("Triple", Triple)
		OUTPUT_SPLITTYPE_VALUE("Boom", Boom)

		OUTPUT_SPLITTYPE_RANGE("SingleChain", SingleChain)
		OUTPUT_SPLITTYPE_RANGE("DoubleChain", DoubleChain)
		OUTPUT_SPLITTYPE_RANGE("TripleChain", TripleChain)
		return ostr.str();
}