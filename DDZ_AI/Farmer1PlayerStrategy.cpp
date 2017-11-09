#include "stdafx.h"
#include "Farmer1PlayerStrategy.h"
#include "GameTable.h"

Farmer1PlayerStrategy::~Farmer1PlayerStrategy()
{
}

Farmer1PlayerStrategy::Farmer1PlayerStrategy(const std::vector<uint8_t>& cardsValue, GameTable* table) : PlayStrategyBase(EIdentity(), cardsValue,table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
	//m_minStepSplitStrategy = std::make_shared<MinStepSplitStrategy>(m_handCards);
}

Farmer1PlayerStrategy::Farmer1PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table) : PlayStrategyBase(EIdentity(), cardsValue,table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

CardStyle Farmer1PlayerStrategy::Play()
{
	return CardStyle();
}

CardStyle Farmer1PlayerStrategy::Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	return CardStyle();
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
	return std::vector<ECardStyle::ECardStyle_>();
}

int Farmer1PlayerStrategy::EIdentity()
{
	return 1;
}

void Farmer1PlayerStrategy::Init()
{
}
