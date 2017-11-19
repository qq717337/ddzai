#include "stdafx.h"
#include "Farmer2PlayerStrategy.h"
#include "GameTable.h"
#include"SplitStrategy.h"
#include "MinStepSplitStrategy.h"
#include "HandleLetFarmer2WinPlay.h"

Farmer2PlayerStrategy::Farmer2PlayerStrategy(const CardVector & cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

Farmer2PlayerStrategy::Farmer2PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

CardStyle Farmer2PlayerStrategy::takeLord(const CardStyle & lastStyle)
{
	CardStyle ret;
	if (m_handlerOptimiumTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//有最小步数可接牌的情况
		return ret;
	}
	if (m_handlerBoomTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//炸弹可以接的情况
		return ret;
	}
	if (m_handlerAvailableTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {
		return ret;
	}
	return CardStyle::Invalid;
}

CardStyle Farmer2PlayerStrategy::takeFarmer(const CardStyle & lastStyle)
{
	auto& optTakeStyle = m_minStepSplitStrategy->GetOptimiumStyle();
	auto& optBoomTakeStyle = m_minStepSplitStrategy->GetOptimiumBoomStyle();
	if (optTakeStyle.size() == 0 && optBoomTakeStyle.size() == 0) {
		return CardStyle::Invalid;
	}
	if (optTakeStyle.size() > 0) {
		return optTakeStyle[0];
	}
	if (optBoomTakeStyle.size() > 0) {
		//判断地主能不能接住以及还有几回合走完确定要不要使用炸弹

	}
	return CardStyle::Invalid;
}

Farmer2PlayerStrategy::~Farmer2PlayerStrategy()
{
}

CardStyle Farmer2PlayerStrategy::Play()
{
	CardStyle r;
	m_minStepSplitStrategy->Split();
	if (m_handlerLastShotPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
		return r;
	}
	if (m_handlerTwoStepPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
		return r;
	}
	if (m_handlerAvoidOtherWinPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
		return r;
	}

	m_handlerMinStepPlay->Handle(this, m_minStepSplitStrategy.get(), r);
	return r;
}

CardStyle Farmer2PlayerStrategy::Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_minStepSplitStrategy->Split();
	m_minStepSplitStrategy->OptimiumTake(lastStyle);
	m_minStepSplitStrategy->AvailableTake(lastStyle);

	std::vector<CardStyle> x;
	bool isWin = CheckIfWin(m_minStepSplitStrategy.get(), lastStyle, true, x);
	if (isWin) {
		return x[0];
	}
	if (lastIdentity == EIdentity::Lord) {
		return takeLord(lastStyle);
	}
	else {
		return takeFarmer(lastStyle);
	}
}

bool Farmer2PlayerStrategy::OtherCanTake(const CardStyle & style) const
{
	return m_table->GetHandCard(EIdentity::Lord)->CanTake(style);
}

bool Farmer2PlayerStrategy::IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level, int param1, void* param2) const
{
	return false;
}

std::vector<ECardStyle::ECardStyle_> Farmer2PlayerStrategy::AvoidPlayStyle()
{
	std::vector<ECardStyle::ECardStyle_> r;
	auto lordCardsCount = m_table->CardCount(EIdentity::Lord);

	if (lordCardsCount == 1) {
		r.push_back(ECardStyle::Single);
	}
	if (lordCardsCount == 2) {
		r.push_back(ECardStyle::Double);
	}
	if (lordCardsCount == 3) {
		r.push_back(ECardStyle::Triple_Zero);
	}
	return r;
}

EIdentity::EIdentity_ Farmer2PlayerStrategy::Identity()const
{
	return EIdentity::EIdentity_::Farmer2;
}

void Farmer2PlayerStrategy::Init()
{
	PlayStrategyBase::Init();
}
