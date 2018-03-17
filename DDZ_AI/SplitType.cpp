#include "stdafx.h"
#include "SplitType.h"
#include "common_algorithm.h"
#include <algorithm>
#include <unordered_set>
#include "Recorder.h"

SplitType::SplitType() :m_isSplitBoom(false)
{
}

SplitType::~SplitType()
{
}
SplitType::SplitType(const SplitType & rhs) :m_isSplitBoom(false)
{
	VECTOR_COPY(rhs.Boom, Boom);
	VECTOR_COPY(rhs.Single, Single);
	VECTOR_COPY(rhs.Double, Double);
	VECTOR_COPY(rhs.Triple, Triple);
	VECTOR_COPY(rhs.SingleChain, SingleChain);
	VECTOR_COPY(rhs.DoubleChain, DoubleChain);
	VECTOR_COPY(rhs.TripleChain, TripleChain);
}
SplitType & SplitType::operator=(const SplitType & rhs)
{
	if (this != &rhs)
	{
		m_isSplitBoom = rhs.m_isSplitBoom;
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

void SplitType::Backup()const
{
	Recorder<SplitType>::Push(new SplitMemnto(*this));
}

void SplitType::Restore()const
{
	Recorder<SplitType>::Pop(const_cast<SplitType*>(this));
}

size_t SplitType::MinStepCount(bool exceptBoom)const
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
	if (exceptBoom) {
		totalStep -= Boom.size();
	}
	return totalStep;
}
bool SplitType::GetLastShotCardStyle(CardStyle* ref) const {
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
					*ref = CardStyle(ECardStyle::Triple_Chain_One, TripleChain[0].Start, TripleChain[0].End, CardVector{ Double[0], Double[0] });
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
CardStyle SplitType::MinValueCardStyle()const {
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
		r = GetTripleStyles()[0];
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


CardStyle SplitType::GetSingleChainStyle()const {
	if (SingleChain.size() > 0) {
		return CardStyle(ECardStyle::Single_Chain, SingleChain[0].Start, SingleChain[0].End);
	}
	return CardStyle::Invalid;
}
CardStyle SplitType::GetDoubleChainStyle() const {
	if (DoubleChain.size() > 0) {
		return CardStyle(ECardStyle::Double_Chain, DoubleChain[0].Start, DoubleChain[0].End);
	}
	return CardStyle::Invalid;
}


CardStyle SplitType::GetTripleChainStyle()const {
	CardStyle r;
	if (TripleChain.size() == 0) {
		return CardStyle::Invalid;
	}
	size_t chainLen = TripleChain[0].Length();

	size_t singleLen = Single.size();
	size_t doubleLen = Double.size();

	if (singleLen >= chainLen) {
		if (doubleLen < chainLen) {
			r = CardStyle(ECardStyle::Triple_Chain_One, TripleChain[0].Start, TripleChain[0].End, CardVector(Single.begin(), Single.begin() + chainLen));
			Single.erase(Single.begin(), Single.begin() + chainLen);
		}
		else {
			if (Single[0] + Single[1] < Double[0] + Double[1]) {
				r = CardStyle(ECardStyle::Triple_Chain_One, TripleChain[0].Start, TripleChain[0].End, CardVector(Single.begin(), Single.begin() + chainLen));
				Single.erase(Single.begin(), Single.begin() + chainLen);
			}
			else {
				r = CardStyle(ECardStyle::Triple_Chain_Two, TripleChain[0].Start, TripleChain[0].End, CardVector(Double.begin(), Double.begin() + chainLen));
				Double.erase(Double.begin(), Double.begin() + chainLen);
			}
		}
	}
	else {
		if (doubleLen >= chainLen) {
			r = CardStyle(ECardStyle::Triple_Chain_Two, TripleChain[0].Start, TripleChain[0].End, CardVector(Double.begin(), Double.begin() + chainLen));
			Double.erase(Double.begin(), Double.begin() + chainLen);
		}
		else {
			r = CardStyle(ECardStyle::Triple_Chain_Zero, TripleChain[0].Start, TripleChain[0].End);
		}
	}
	return r;
}

std::vector<CardStyle> SplitType::GetTripleStyles(bool erase)const {
	std::vector<CardStyle> r;
	if (Triple.size() == 0) {
		return std::vector<CardStyle>();
	}
	for (auto & v : Triple) {
		size_t singleLen = Single.size();
		size_t  doubleLen = Double.size();
		if (singleLen > 0) {
			if (doubleLen == 0) {
				r.emplace_back(ECardStyle::Triple_One, v, v, std::vector<uint8_t>({ Single[0] }));
				if (erase)
					Single.erase(Single.begin());
			}
			else {
				if (Single[0] < Double[0]) {
					r.emplace_back(ECardStyle::Triple_One, v, v, std::vector<uint8_t>({ Single[0] }));
					if (erase)
						Single.erase(Single.begin());
				}
				else {
					r.emplace_back(ECardStyle::Triple_Two, v, v, std::vector<uint8_t>({ Double[0] }));
					if (erase)
						Double.erase(Double.begin());
				}
			}
		}
		else {
			if (doubleLen > 0) {
				r.emplace_back(ECardStyle::Triple_Two, v, v, std::vector<uint8_t>({ Double[0] }));
				if (erase)
					Double.erase(Double.begin());
			}
			else {
				r.emplace_back(ECardStyle::Triple_Zero, v, v);
			}
		}

	}
	return r;
}
std::vector<CardStyle> SplitType::GetAllSplitStyle() const
{
	Backup();
	std::vector<CardStyle> availableStyle;
	auto singleChainStyle = GetSingleChainStyle();
	if (singleChainStyle.Valid()) {
		availableStyle.push_back(std::move(singleChainStyle));
	}
	auto doubleChainStyle = GetDoubleChainStyle();
	if (doubleChainStyle.Valid()) {
		availableStyle.push_back(std::move(doubleChainStyle));
	}

	auto tripleChainStyle = GetTripleChainStyle();
	if (tripleChainStyle.Valid()) {
		availableStyle.push_back(std::move(tripleChainStyle));
	}

	auto tripleStyle = GetTripleStyles();
	if (!tripleStyle.empty()) {
		availableStyle.insert(availableStyle.end(), tripleStyle.begin(), tripleStyle.end());
	}

	for (auto v : Double) {
		availableStyle.emplace_back(ECardStyle::Double, v);
	}
	for (auto v : Single) {
		availableStyle.emplace_back(ECardStyle::Single, v);
	}

	for (auto v : Boom) {
		availableStyle.emplace_back(ECardStyle::Boom, v);
	}
	Restore();
	return availableStyle;
}
size_t SplitType::CardCount() const {
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
size_t SplitType::CauseExtraSingleDoubleStep(CardRange& range) const
{
	size_t sum = 0;
	for (uint8_t i = range.Start; i <= range.End; i++) {
		if (InSingle(i) || InDouble(i)) {
			++sum;
		}
	}
	return sum;
}
bool SplitType::RequireSingle(size_t requireCount, CardVector & out, bool splitDouble) const
{
	for (auto& v : Single) {
		out.push_back(v);
		if (out.size() == requireCount) {
			return true;
		}
	}
	bool hasSplitDouble = false;
	if (requireCount - out.size() >= 2) {
		for (auto&v : Double) {
			out.push_back(v);
			out.push_back(v);
			hasSplitDouble = true;
			if (requireCount - out.size() < 2) {
				break;
			}
		}
	}
	bool isSucc = RequireSingleFromChain(requireCount - out.size(), out);
	return isSucc;
}
bool SplitType::RequireDouble(size_t requireCount, CardVector & out, bool splitTriple) const
{
	for (auto& v : Double) {
		out.push_back(v);
		if (out.size() == requireCount) {
			return true;
		}
	}
	if (requireCount == 1) {
		bool isSucc = RequireDoubleFromChain(requireCount - out.size(), out, false);
		if (isSucc)
			return true;
		else {
			if (Triple.empty() == false) {
				out.push_back(Triple[0]);
			}
		}
	}
	else {
		bool isSucc = RequireDoubleFromChain(requireCount - out.size(), out, true);
		if (isSucc)
			return true;
	}

	return false;
}
bool SplitType::RequireSingleFromChain(size_t requireCount, CardVector & out, bool forceSplit)const
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
		CardVector 	exclude;
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
bool SplitType::RequireDoubleFromChain(size_t requireCount, CardVector & out, bool forceSplit)const
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
		CardVector 	exclude;
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
bool SplitType::RequireFromAll(size_t requireCount, CardVector & outSingleIndex, CardVector & outDoubleIndex, CardVector & outTripleIndex,
	std::function<void(CardVector &, CardVector &, CardVector &)> redefineFunc)
{
	size_t lenOfSingle = Single.size();
	size_t lenOfDouble = Double.size();
	size_t lenOfTriple = Triple.size();
	size_t lenOfTripleChain = 0;

	std::unordered_set<uint8_t> singleIsolateCards;
	std::unordered_set<uint8_t> doubleIsolateCards;
	std::unordered_set<uint8_t> tripleIsolateCards;
	if (redefineFunc) {
		redefineFunc(Single, Double, Triple);
	}
	if (lenOfSingle > 0) {
		for (auto v : Single) {
			singleIsolateCards.insert(v);
		}
	}
	if (lenOfDouble > 0) {
		for (auto v : Double) {
			doubleIsolateCards.insert(v);
		}
	}
	if (lenOfTriple > 0) {
		for (auto v : Triple) {
			tripleIsolateCards.insert(v);
		}
	}
	if (SingleChain.size() > 0) {
		for (auto v : SingleChain) {
			if (v.Length() == 6) {
				singleIsolateCards.insert(v.Start);
			}
			if (v.Length() > 6) {
				singleIsolateCards.insert(v.Start);
				singleIsolateCards.insert(v.End);
			}
		}
	}
	if (DoubleChain.size() > 0) {
		for (auto v : DoubleChain) {
			if (v.Length() > 3) {
				doubleIsolateCards.insert(v.Start);
				doubleIsolateCards.insert(v.End);
			}
		}
	}
	if (TripleChain.size() > 0) {
		for (auto v : TripleChain) {
			if (v.Length() > 2) {
				tripleIsolateCards.insert(v.Start);
				tripleIsolateCards.insert(v.End);
			}
		}
	}
	int index;
	if (singleIsolateCards.size() + doubleIsolateCards.size() * 2 + tripleIsolateCards.size() * 3 < requireCount) {
		if (DoubleChain.size() > 0) {
			for (index = DoubleChain[0].Start; index <= DoubleChain[0].End; ++index) {
				doubleIsolateCards.insert(index);
			}
		}
		//if (SingleChain.size() > 0) {
		//	for (auto v : SingleChain) {
		//		if (v.Length() <= 6) {
		//			for (index = SingleChain[0].Start; index <= SingleChain[0].End; ++index) {
		//				singleIsolateCards.insert(index);
		//			}
		//		}
		//	}
		//}
	}
	// CommonRandom.NextInt(0, swapOtherIndex.size())
	size_t ls = singleIsolateCards.size();
	size_t ld = doubleIsolateCards.size();
	size_t lt = tripleIsolateCards.size();
	CardVector  singleSlice(singleIsolateCards.begin(), singleIsolateCards.end());
	CardVector  doubleSlice(doubleIsolateCards.begin(), doubleIsolateCards.end());
	CardVector  tripleSlice(tripleIsolateCards.begin(), tripleIsolateCards.end());

	std::vector< uint8_t> randSingleIndex;
	perm<uint8_t>(ls, randSingleIndex);
	std::vector< uint8_t> randDoubleIndex;
	perm<uint8_t>(ld, randDoubleIndex);
	std::vector< uint8_t> randTripleIndex;
	perm<uint8_t>(lt, randTripleIndex);

	if (requireCount <= ls) {
		for (index = 0; index < requireCount; index++) {
			outSingleIndex.push_back(singleSlice[randSingleIndex[index]]);
		}
		return true;
	}
	else {
		if (requireCount <= ld * 2 + ls) {

			int requireSingleCount = 0;
			int	requireDoubleCount = 0;
			int	subSingleCount = requireCount - ls;
			if (subSingleCount % 2 == 0) {
				requireSingleCount = ls;
				requireDoubleCount = subSingleCount / 2;
			}
			else {
				requireSingleCount = ls - 1;
				requireDoubleCount = subSingleCount / 2 + 1;
			}

			for (index = 0; index < requireSingleCount; ++index) {
				int cardIndex = singleSlice[randSingleIndex[index]];
				outSingleIndex.push_back(cardIndex);
			}

			for (index = 0; index < requireDoubleCount; ++index) {
				int cardIndex = doubleSlice[randDoubleIndex[index]];
				outDoubleIndex.push_back(cardIndex);
			}
		}
		else {
			if (ls + ld * 2 + 3 < requireCount) {
				if (lt < 2) {
					throw std::runtime_error("无法找到两个Triple");
				}
				else {
					outTripleIndex.push_back(tripleSlice[randTripleIndex[0]]);
					outDoubleIndex.push_back(tripleSlice[randTripleIndex[1]]);
				}
				return true;
			}

			int requireSingleCount = 0;
			int	requireDoubleCount = 0;
			int	subSingleDoubleCount = requireCount - 3;
			if (subSingleDoubleCount % 3 == 1) {
				requireSingleCount = 1;
			}
			if (subSingleDoubleCount % 3 == 2) {
				requireDoubleCount = 1;
			}

			if (requireSingleCount == 1) {
				if (ls == 0) {
					if (ld == 0) {
						throw std::runtime_error("既没有单也没有双，找不到了");
					}
					else {
						for (index = 0; index < requireSingleCount; index++) {
							outSingleIndex.push_back(doubleSlice[randDoubleIndex[index]]);
						}
					}
				}
				else {
					outSingleIndex.push_back(singleSlice[randSingleIndex[0]]);
				}
			}
			if (requireDoubleCount == 1) {
				if (ld == 0) {
					if (ls < 2) {
						throw std::runtime_error("既没有单也没有双，找不到了");
					}
					else {
						for (index = 0; index < 2; index++) {
							outSingleIndex.push_back(singleSlice[randSingleIndex[index]]);
						}
					}
				}
				else {
					outDoubleIndex.push_back(doubleSlice[randDoubleIndex[0]]);
				}
			}
			if (randTripleIndex.size() > 0) {
				outTripleIndex.push_back(tripleSlice[randTripleIndex[0]]);
			}
			else {
				throw std::runtime_error("没有三张可以替换");
			}
		}
	}
	return true;
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
void SplitType::Sort()
{
	std::sort(Boom.begin(), Boom.end());
	std::sort(Triple.begin(), Triple.end());
	std::sort(Double.begin(), Double.end());
	std::sort(Single.begin(), Single.end());
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