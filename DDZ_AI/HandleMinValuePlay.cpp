#include "stdafx.h"
#include "HandleMinValuePlay.h"
#include "common_algorithm.h"

HandleMinValuePlay::~HandleMinValuePlay()
{
}

bool HandleMinValuePlay::Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	const SplitType& split = splitStrategy->MinStepSplit();
	auto& _boom = split.GetBoom();
	auto& _triple = split.GetTriple();
	auto& _double = split.GetDouble();
	auto& _single = split.GetSingle();
	auto& _tripleChain = split.GetTripleChain();
	auto& _doubleChain = split.GetDoubleChain();
	auto& _singleChain = split.GetSingleChain();

	auto tripleLen = _triple.size();
	auto doubleLen = _double.size();
	auto singleLen = _single.size();
	auto tripleChainLen = _tripleChain.size();
	auto doubleChainLen = _doubleChain.size();
	auto singleChainLen = _singleChain.size();
	uint8_t minIndex = CardIndex_LargeJoker + 1;
	CardStyle r;

	bool ignoreSingle = false;
	bool ignoreDouble = false;
	if (tripleChainLen > 0) {
		tripleChainLen = _tripleChain[0].Length();
		auto tripleChainStart = _tripleChain[0].Start;
		if (tripleChainLen == singleLen) { //如果Chain的长度大于单只的数目，则不能忽略
			ignoreSingle = true;
		}
		if (tripleChainLen < singleLen) {
			auto smallerSingleCount = smallerCount(_single, tripleChainStart);
			if (smallerSingleCount <= tripleChainLen) {
				ignoreSingle = true;
			}
		}

		if (tripleChainLen == doubleLen) { //如果Chain的长度大于单只的数目，则不能忽略
			ignoreDouble = true;
		}
		if (tripleChainLen < doubleLen) {
			auto 	smallerDoubleCount = smallerCount(_double, tripleChainStart);
			if (smallerDoubleCount <= tripleChainLen) {
				ignoreDouble = true;
			}
		}
		if (_tripleChain[0].Length() == 3 && doubleLen == 1 && singleLen == 1) {
			ignoreDouble = true;
			ignoreSingle = true;
		}
	}
	if (tripleLen > 0) {
		if (tripleLen == singleLen) { //如果Chain的长度大于单只的数目，则不能忽略
			ignoreSingle = true;
		}
		if (tripleLen < singleLen) {
			auto smallerSingleCount = smallerCount(_single, _triple[0]);
			if (smallerSingleCount <= tripleLen) {
				ignoreSingle = true;
			}
		}
		if (tripleLen == doubleLen) { //如果Chain的长度大于单只的数目，则不能忽略
			ignoreDouble = true;
		}
		if (tripleLen < doubleLen) {
			auto smallerDoubleCount = smallerCount(_double, _triple[0]);
			if (smallerDoubleCount <= tripleLen) {
				ignoreDouble = true;
			}
		}

		if (tripleChainLen + tripleLen == doubleLen + singleLen) {
			ignoreDouble = true;
			ignoreSingle = true;
		}
	}

	if (!ignoreSingle && singleLen > 0 && _single[0] <= minIndex) {
		minIndex = _single[0];
		r = CardStyle(ECardStyle::Single, _single[0]);
	}
	if (!ignoreDouble && doubleLen > 0 && _double[0] <= minIndex) {
		minIndex = _double[0];
		r = CardStyle(ECardStyle::Double, _double[0]);
	}
	if (tripleLen > 0 && _triple[0] <= minIndex) {
		minIndex = _triple[0];
		r = split.GetTripleStyle();
	}
	if (singleChainLen > 0 && _singleChain[0].Start <= minIndex) {
		minIndex = _singleChain[0].Start;
		r = CardStyle(ECardStyle::Single_Chain, _singleChain[0].Start, _singleChain[0].End);
	}
	if (doubleChainLen > 0 && _doubleChain[0].Start <= minIndex) {
		minIndex = _doubleChain[0].Start;
		r = CardStyle(ECardStyle::Double_Chain, _doubleChain[0].Start, _doubleChain[0].End);
	}
	if (tripleChainLen > 0 && _tripleChain[0].Start <= minIndex) {
		minIndex = _tripleChain[0].Start;
		r = split.GetTripleChainStyle();
	}
	if (r.Style == ECardStyle::Invalid) {
		if (_boom.size() > 0) {
			if (_boom[0] > CardIndex_2) {
				r = CardStyle::JokerBoom;
			}
			else {
				r = CardStyle(ECardStyle::Boom, _boom[0], _boom[0]);
			}
		}
	}
	result = r;
	return true;
}
