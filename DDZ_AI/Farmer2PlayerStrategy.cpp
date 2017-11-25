#include "stdafx.h"
#include "Farmer2PlayerStrategy.h"
#include "GameTable.h"
#include"SplitStrategy.h"
#include "MinStepSplitStrategy.h"
#include "HandleLetFarmer2WinPlay.h"

Farmer2PlayerStrategy::Farmer2PlayerStrategy(const CardVector & cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
}

Farmer2PlayerStrategy::Farmer2PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
}

CardStyle Farmer2PlayerStrategy::takeLord(const CardStyle & lastStyle)
{
	CardStyle ret;
	if (m_handlerOptimiumTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//有最小步数可接牌的情况
		DEBUG_LOG("HandleOptimiumTake  " + ret.ToString());
		return ret;
	}
	if (m_handlerBoomTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//炸弹可以接的情况
		DEBUG_LOG("HandleBoomTake  " + ret.ToString());
		return ret;
	}
	if (m_handlerAvailableTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {
		DEBUG_LOG("HandleAvailableTake  " + ret.ToString());
		return ret;
	}
	DEBUG_LOG("不接");
	return CardStyle::Invalid;
}

CardStyle Farmer2PlayerStrategy::takeFarmer(const CardStyle & lastStyle)
{
	if (lastStyle.IsBigCard()) {
		return CardStyle::Invalid;
	}

	auto& optTakeStyle = m_minStepSplitStrategy->GetOptimiumStyle();
	auto& optBoomTakeStyle = m_minStepSplitStrategy->GetOptimiumBoomStyle();
	if (optTakeStyle.size() == 0 && optBoomTakeStyle.size() == 0) {
		DEBUG_LOG("没有最佳接法，不接");
		return CardStyle::Invalid;
	}
	if (optTakeStyle.size() > 0) {
		DEBUG_LOG("接农民1的牌，采用OptmiumTake");
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
	CardStyle ret;
	m_minStepSplitStrategy->Split();
	if (m_handlerLastShotPlay->Handle(this, m_minStepSplitStrategy.get(), ret)) {
		DEBUG_LOG("HandleLastShotPlay  " + ret.ToString());
		return ret;
	}
	if (m_handlerTwoStepPlay->Handle(this, m_minStepSplitStrategy.get(), ret)) {
		DEBUG_LOG("HandleTwoStepPlay  " + ret.ToString());
		return ret;
	}
	if (m_handlerAvoidOtherWinPlay->Handle(this, m_minStepSplitStrategy.get(), ret)) {
		DEBUG_LOG("HandleAvoidOtherWinPlay  " + ret.ToString());
		return ret;
	}

	m_handlerMinValuePlay->Handle(this, m_minStepSplitStrategy.get(), ret);
	DEBUG_LOG("HandleMinValuePlay  " + ret.ToString());
	return ret;
}

CardStyle Farmer2PlayerStrategy::Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_minStepSplitStrategy->Split();
	m_minStepSplitStrategy->OptimiumTake(lastStyle);
	m_minStepSplitStrategy->AvailableTake(lastStyle);

	std::vector<CardStyle> x;
	bool isWin = CheckIfWin(m_minStepSplitStrategy.get(), lastStyle, true, x);
	if (isWin) {
		DEBUG_LOG("CheckIfWin=true  " + lastStyle.ToString());
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

bool Farmer2PlayerStrategy::OtherBiggestCardValue(int compareCount) const
{
	return m_table->BiggestCardValue(EIdentity::Lord, compareCount);
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
