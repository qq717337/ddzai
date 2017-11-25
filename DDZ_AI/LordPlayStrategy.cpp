#include "stdafx.h"
#include "LordPlayStrategy.h"
#include "GameTable.h"
#include"SplitStrategy.h"
#include "MinStepSplitStrategy.h"

CardStyle LordPlayStrategy::Play()
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

CardStyle LordPlayStrategy::Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_minStepSplitStrategy->Split();
	m_minStepSplitStrategy->OptimiumTake(lastStyle);
	m_minStepSplitStrategy->AvailableTake(lastStyle);
	m_mustTake = false;
	//先判断使用哪一种拆分策略，或者一开始创建多个SplitStrategy，然后再传入不同的Handle里面进行处理
	CardStyle ret(CardStyle::Invalid);

	std::vector<CardStyle> x;
	bool isWin = CheckIfWin(m_minStepSplitStrategy.get(), lastStyle, true, x);
	if (isWin) {
		DEBUG_LOG("CheckIfWin=true  " + lastStyle.ToString());
		return x[0];
	}

	m_mustTake = m_mustTake = m_table->GetHandCard(EIdentity::Farmer1)->LastShot().Valid() ||
		m_mustTake = m_table->GetHandCard(EIdentity::Farmer2)->LastShot().Valid();

	if (m_handlerOptimiumTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//有最小步数可接牌的情况
		DEBUG_LOG("HandleOptimiumTake  " + ret.ToString());
		return ret;
	}
	if (m_handlerBoomTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {
		DEBUG_LOG("HandleBoomTake  " + ret.ToString());
		return ret;
	}
	if (m_handlerAvailableTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//以上不符合 但是可以接牌，进行后续判断
		DEBUG_LOG("HandleAvailableTake  " + ret.ToString());
		return ret;
	}
	DEBUG_LOG("不接");
	//都不符合就选择不接
	return ret;
}

bool LordPlayStrategy::OtherCanTake(const CardStyle & style) const
{
	return m_table->GetHandCard(EIdentity::Farmer1)->CanTake(style) || m_table->GetHandCard(EIdentity::Farmer2)->CanTake(style);
}

bool LordPlayStrategy::OtherBiggestCardValue(int compareCount) const
{
	return m_table->BiggestCardValue(EIdentity::Farmer1, compareCount);
}

bool LordPlayStrategy::IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level, int param1, void* param2) const
{
	const SplitType& f1_minSplit = m_table->GetPlayStrategy(EIdentity::Farmer1)->m_minStepSplitStrategy->MinStepSplit();
	const SplitType& f2_minSplit = m_table->GetPlayStrategy(EIdentity::Farmer2)->m_minStepSplitStrategy->MinStepSplit();
	switch (level)
	{
	case ESituationSafeLevel::AllPlayerMinStepGreater:
		return f1_minSplit.MinStepCount(false) > param1 && f2_minSplit.MinStepCount(false) > param1;
		break;
	case ESituationSafeLevel::AllPlayerMinStepWithOutBoomGreater:
		return f1_minSplit.MinStepCount(true) > param1 && f2_minSplit.MinStepCount(true) > param1;
		break;
	case ESituationSafeLevel::AllPlayerBoomCountLess:
		return f1_minSplit.GetBoom().size() + f2_minSplit.GetBoom().size() < param1;
		break;
	default:
		break;
	}
	return false;
}

std::vector<ECardStyle::ECardStyle_> LordPlayStrategy::AvoidPlayStyle()
{
	std::vector<ECardStyle::ECardStyle_> r;
	auto f1CardsCount = m_table->CardCount(EIdentity::Farmer1);
	auto f2CardsCount = m_table->CardCount(EIdentity::Farmer2);

	if (f1CardsCount == 1 || f2CardsCount == 1) {
		r.push_back(ECardStyle::Single);
	}
	if (f1CardsCount == 2 || f2CardsCount == 2) {
		r.push_back(ECardStyle::Double);
	}
	if (f1CardsCount == 3 || f2CardsCount == 3) {
		r.push_back(ECardStyle::Triple_Zero);
	}
	return r;
}

EIdentity::EIdentity_ LordPlayStrategy::Identity()const
{
	return EIdentity::EIdentity_::Lord;
}

LordPlayStrategy::LordPlayStrategy(const CardVector & cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
}

LordPlayStrategy::LordPlayStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
}

void LordPlayStrategy::Init()
{
	PlayStrategyBase::Init();
}


LordPlayStrategy::~LordPlayStrategy()
{

}
