#include "stdafx.h"
#include "Farmer1PlayerStrategy.h"
#include "GameTable.h"
#include"SplitStrategy.h"
#include "MinStepSplitStrategy.h"
#include "HandleLetFarmer2WinPlay.h"

CardStyle Farmer1PlayerStrategy::takeLord(const CardStyle & lastStyle)
{
	return CardStyle::Invalid;
}

CardStyle Farmer1PlayerStrategy::takeFarmer(const CardStyle & lastStyle)
{
	//不压的情况
	//1.要不起
	//2.Farmer2 出牌会让地主压死
	//3.Farmer2 无法取得胜利，例如全是5张（单只+对子）

	//因为Farmer2已经经过了地主要不起的情况，所以不需要再对其进行防止。需要防止的就是该玩家新出的牌会导致地主获胜
	return CardStyle::Invalid;
}

Farmer1PlayerStrategy::~Farmer1PlayerStrategy()
{
}

Farmer1PlayerStrategy::Farmer1PlayerStrategy(const CardVector & cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
	//m_minStepSplitStrategy = std::make_shared<MinStepSplitStrategy>(m_handCards);
}

Farmer1PlayerStrategy::Farmer1PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

CardStyle Farmer1PlayerStrategy::Play()
{
	CardStyle r;
	m_minStepSplitStrategy->Split();
	if (m_handlerLastShotPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
		return r;
	}
	if (m_handlerLetFarmer2WinPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
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

CardStyle Farmer1PlayerStrategy::Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	std::vector<CardStyle> x;
	bool isWin = CheckIfWin(m_minStepSplitStrategy.get(), lastStyle, x);
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

bool Farmer1PlayerStrategy::OtherCanTake(const CardStyle & style) const
{
	return m_table->GetHandCard(EIdentity::Lord)->CanTake(style);
}

bool Farmer1PlayerStrategy::IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const
{
	return false;
}

std::vector<ECardStyle::ECardStyle_> Farmer1PlayerStrategy::AvoidPlayStyle()
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

EIdentity::EIdentity_ Farmer1PlayerStrategy::Identity()const
{
	return EIdentity::EIdentity_::Farmer1;
}

void Farmer1PlayerStrategy::Init()
{
	PlayStrategyBase::Init();
	m_handlerLetFarmer2WinPlay = std::make_unique<HandleLetFarmer2WinPlay>();
}
