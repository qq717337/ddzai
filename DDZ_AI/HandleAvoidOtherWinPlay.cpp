#include "stdafx.h"
#include "HandleAvoidOtherWinPlay.h"


HandleAvoidOtherWinPlay::~HandleAvoidOtherWinPlay()
{
}

bool HandleAvoidOtherWinPlay::Handle(PlayStrategyBase * playStrategy, SplitStrategy * splitStrategy, CardStyle & result)
{
	const SplitType& p = splitStrategy->MinStepSplit();
	auto avoidStyle = playStrategy->AvoidPlayStyle();

	if (avoidStyle.size() > 0) {
		bool avoidTriple = false, avoidDouble = false, avoidSingle = false;
		for (auto EStyle : avoidStyle) {
			if (EStyle == ECardStyle::Single) {
				avoidSingle = true;
				continue;
			}
			if (EStyle == ECardStyle::Double) {
				avoidDouble = true;
				continue;
			}
			if (EStyle == ECardStyle::Triple_Zero) {
				avoidTriple = true;
				continue;
			}
		}
		std::vector<CardStyle> availableStyle;
		std::vector<CardStyle> optionalStyle;
		CardStyle singleChainStyle = p.GetSingleChainStyle();

		if (singleChainStyle.Valid()) {
			availableStyle.emplace_back(singleChainStyle);
		}
		CardStyle doubleChainStyle = p.GetDoubleChainStyle();
		if (doubleChainStyle.Valid()) {
			availableStyle.emplace_back(doubleChainStyle);
		}

		CardStyle tripleChainStyle = p.GetTripleChainStyle();
		if (tripleChainStyle.Valid()) {
			availableStyle.emplace_back(tripleChainStyle);
		}
		CardStyle	tripleStyle = p.GetTripleStyle();
		if (tripleStyle.Valid()) {
			if (tripleStyle.Style != ECardStyle::Triple_Chain_Zero) {
				availableStyle.emplace_back(tripleStyle);
			}
			else {
				if (avoidTriple) {
					optionalStyle.emplace_back(tripleStyle);
				}
				else {
					availableStyle.emplace_back(tripleStyle);
				}
			}
		}
		auto Double = p.GetDouble();
		if (Double.size() > 0) {
			if (avoidDouble) {
				optionalStyle.emplace_back(CardStyle(ECardStyle::Double, Double[0]));
			}
			else {
				availableStyle.emplace_back(CardStyle(ECardStyle::Double, Double[0]));
			}
		}

		auto Single = p.GetSingle();
		if (Single.size() > 0) {
			if (avoidSingle) {
				optionalStyle.emplace_back(CardStyle(ECardStyle::Single, Single[0]));
			}
			else {
				availableStyle.emplace_back(CardStyle(ECardStyle::Single, Single[0]));
			}
		}
		auto Boom = p.GetBoom();
		if (availableStyle.size() == 0 && Boom.size() > 0) {
			availableStyle.emplace_back(CardStyle(ECardStyle::Boom, Boom[0]));
		}

		if (availableStyle.size() > 0) {
			result = *std::max_element(availableStyle.begin(), availableStyle.end(), [](const CardStyle x, const CardStyle y) ->bool {
				return x.EndValue < y.EndValue;
			});
			return true;
		}
		else {
			result = *std::max_element(optionalStyle.begin(), optionalStyle.end(), [](const CardStyle x, const CardStyle y) ->bool {
				return x.EndValue < y.EndValue;
			});
			return true;
		}
	}
	return false;
}
