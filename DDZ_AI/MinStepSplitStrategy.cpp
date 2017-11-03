#include "stdafx.h"
#include "MinStepSplitStrategy.h"
#include <algorithm>
//typedef void(MinStepSplitStrategy::*MYFUN)();

void MinStepSplitStrategy::Split()
{
	m_splitTypeVector.clear();
	m_splitMinStepVector.clear();
	//void(MinStepSplitStrategy::*ptrnonstatic)(std::function<void(void)>) = &MinStepSplitStrategy::IterSplitHandleFunc;
	std::for_each(m_split_function.begin(), m_split_function.end(),
		[this](std::function<void(void)> func) {
		Backup();
		m_splitType->Reset();
		SplitIsolate();
		func();
		SplitIsolate();
		auto itFinder = std::find(m_splitTypeVector.begin(), m_splitTypeVector.end(), *m_splitType);
		if (m_splitTypeVector.size() == 0 || itFinder == m_splitTypeVector.end()) {
			m_splitMinStepVector.push_back(m_splitType->MinStepCount());
			m_splitTypeVector.push_back(std::move(*m_splitType));
		}
		Restore();
	});
}

std::vector< CardStyle>  MinStepSplitStrategy::OptimizeTake(const CardStyle & style)
{
	std::vector<CardStyle> optStyle;
	auto& splitTypeRef = MinStepSplit();
	switch (style.Style)
	{
	case ECardStyle::Single: {
		auto& optSingles = splitTypeRef.GetSingle();
		for (uint8_t v : optSingles) {
			if (v > style.StartValue) {
				optStyle.emplace_back(style.Style, v);
			}
		}
		break;
	}
	case ECardStyle::Double: {
		auto& optDoubles = splitTypeRef.GetDouble();
		for (uint8_t v : optDoubles) {
			if (v > style.StartValue) {
				optStyle.emplace_back(style.Style, v);
			}
		}
		break;
	}
	case ECardStyle::Triple_Zero:
	case ECardStyle::Triple_One:
	case ECardStyle::Triple_Two: {
		std::vector<uint8_t> okTriple;
		auto& optTriples = splitTypeRef.GetTriple();
		for (uint8_t v : optTriples) {
			if (v > style.StartValue) {
				okTriple.push_back(v);
			}
		}
		std::vector<uint8_t> extra;
		auto & _double = splitTypeRef.GetDouble();
		auto & _single = splitTypeRef.GetSingle();
		size_t singleLen = _single.size();
		size_t doubleLen = _double.size();
		if (style.Style == ECardStyle::Triple_One) {
			if (singleLen > 0) {
				extra.push_back(_single[0]);
			}
			else if (doubleLen > 0) {
				extra.push_back(_double[0]);
			}
			else {
				splitTypeRef.RequireSingleFromChain(1, extra);
			}
		}
		else if (style.Style == ECardStyle::Triple_Two) {
			if (doubleLen > 0) {
				extra.push_back(_double[0]);
			}
			else {
				splitTypeRef.RequireDoubleFromChain(1, extra);
			}
		}
		if (style.Style == ECardStyle::Triple_Zero || style.Style != ECardStyle::Triple_Zero && extra.size() > 0) {
			for (auto v : okTriple) {
				optStyle.emplace_back(style.Style, v, extra);
			}
		}
		break;
	}
	case ECardStyle::Boom: {
		auto &	biggerBoom = splitTypeRef.GetBoom();
		for (auto v : biggerBoom) {
			if (v > style.StartValue) {
				if (v < CardIndex_SmallJoker) {
					optStyle.emplace_back(style.Style, v);
				}
				else {
					optStyle.push_back(CardStyle::JokerBoom);
				}
			}
		}
		break;
	}
	case ECardStyle::Single_Chain: {
		auto& chain = splitTypeRef.GetSingleChain();
		for (auto & v : chain) {
			if (v.Start > style.StartValue && v.Length() >= style.Length() && v.Length() - style.Length() < 3) {
				optStyle.emplace_back(style.Style, v.Start, v.Start + (style.EndValue - style.StartValue));
			}
		}
		break;
	}
	case ECardStyle::Double_Chain: {
		auto& chain = splitTypeRef.GetDoubleChain();
		for (auto & v : chain) {
			if (v.Start > style.StartValue && v.Length() >= style.Length() && v.Length() - style.Length() < 2) {
				optStyle.emplace_back(style.Style, v.Start, v.Start + (style.EndValue - style.StartValue));
			}
		}
		break;
	}
	case ECardStyle::Triple_Chain_Zero:
	case ECardStyle::Triple_Chain_One:
	case ECardStyle::Triple_Chain_Two: {std::vector<uint8_t> extra;
		auto & _double = splitTypeRef.GetDouble();
		auto & _single = splitTypeRef.GetSingle();
		size_t singleLen = _single.size();
		size_t doubleLen = _double.size();
		int chainCount = style.Length();
		if (style.Style == ECardStyle::Triple_Chain_One) {
			if (singleLen >= chainCount) {
				for (int i = 0; i < chainCount; ++i)extra.push_back(_single[i]);
			}
			else { //如果拆牌的不包含额外的单只，那么需要
				splitTypeRef.RequireSingleFromChain(chainCount, extra);
			}
			if (doubleLen > 0) {
				if (chainCount == 2) {
					extra.push_back(_double[0]);
					extra.push_back(_double[0]);
				}
				if (chainCount == 3) {
					if (singleLen == 0 && doubleLen >= 2) {
						extra.push_back(_double[0]);
						extra.push_back(_double[0]);
						extra.push_back(_double[1]);
					}
					if (doubleLen == 1 && singleLen == 1) {
						extra.push_back(_single[0]);
						extra.push_back(_double[0]);
						extra.push_back(_double[0]);
					}
				}
			}
		}
		else if (style.Style == ECardStyle::Triple_Chain_Two) {
			if (doubleLen >= chainCount) {
				for (int i = 0; i < chainCount; ++i)extra.push_back(_double[i]);
			}
			else {
				splitTypeRef.RequireDoubleFromChain(chainCount, extra);
			}
		}

		int tripleChainStartValue = -1;
		int tripleChainEndValue = -1;
		auto &	chain = splitTypeRef.GetTripleChain();
		for (auto v : chain) {
			if (v.Start > style.StartValue && v.Length() == style.Length()) {
				tripleChainStartValue = v.Start;
				tripleChainEndValue = v.Start + (style.EndValue - style.StartValue);
			}
		}
		if (tripleChainStartValue > -1) {
			if (style.Style == ECardStyle::Triple_Chain_Zero) {
				optStyle.emplace_back(style.Style, tripleChainStartValue, tripleChainEndValue);
			}
			else {
				if (extra.size() >= chainCount) {
					optStyle.emplace_back(style.Style, tripleChainStartValue, tripleChainEndValue, extra);
				}
			}
		}
		break;
	}
	default:
		break;
	}
	return optStyle;
}

MinStepSplitStrategy::MinStepSplitStrategy(std::shared_ptr<HandCards> cards) :SplitStrategy(cards), m_split_function{
	std::bind(&MinStepSplitStrategy::singleTripleChainFirst,this),
	std::bind(&MinStepSplitStrategy::singleDoubleChainFirst,this),
	std::bind(&MinStepSplitStrategy::singleTripleChainSecond,this),
	std::bind(&MinStepSplitStrategy::singleDoubleChainSecond,this),
	std::bind(&MinStepSplitStrategy::doubleSingleChainFirst,this),
	std::bind(&MinStepSplitStrategy::doubleTripleChainFirst,this),
	std::bind(&MinStepSplitStrategy::tripleDoubleChainFirst,this),
	std::bind(&MinStepSplitStrategy::tripleSingleChainFirst,this),
}
{
}


MinStepSplitStrategy::~MinStepSplitStrategy()
{
}

void MinStepSplitStrategy::singleTripleChainFirst()
{
	SplitSingleChainTruncBoom();
	SplitIntersectChain();
	SplitSingleChain();
	SplitTripleChain();
	SplitTriple(false);
	SplitDoubleChain();
	SplitBoom();
}

void MinStepSplitStrategy::singleDoubleChainFirst()
{
	SplitBoom(false);
	SplitSingleChain(false);
	SplitDoubleChain(false);
	SplitTripleChain();
	SplitTriple(false);
}

void MinStepSplitStrategy::singleTripleChainSecond()
{
	SplitSingleChainTruncBoom();
	SplitSingleChain(false);
	SplitTripleChain();
	SplitTriple(false);
	SplitDoubleChain();
	SplitBoom(false);
}

void MinStepSplitStrategy::singleDoubleChainSecond()
{
	SplitSingleChainTruncBoom();
	SplitSingleChain(false);
	SplitDoubleChain();
	SplitTripleChain();
	SplitTriple(false);
	SplitBoom();
}

void MinStepSplitStrategy::doubleSingleChainFirst()
{
	SplitBoom(false);
	SplitDoubleChain(false);
	SplitIntersectChain();
	SplitSingleChain();
	SplitTripleChain();
	SplitTriple(false);
}

void MinStepSplitStrategy::doubleTripleChainFirst()
{
	SplitBoom(false);
	SplitDoubleChain(false);
	SplitTripleChain();
	SplitIntersectChain();
	SplitSingleChain();
	SplitTripleChain();
	SplitTriple(false);
}

void MinStepSplitStrategy::tripleDoubleChainFirst()
{
	SplitBoom(false);
	SplitTripleChain();
	SplitTriple(false);
	SplitDoubleChain();
	SplitIntersectChain();
	SplitSingleChain();
	SplitTripleChain();
	SplitTriple(false);
}

void MinStepSplitStrategy::tripleSingleChainFirst()
{
	SplitBoom();
	SplitTripleChain();
	SplitTriple(false);
	SplitIntersectChain();
	SplitDoubleChain(false);
	SplitSingleChain(false);
}
