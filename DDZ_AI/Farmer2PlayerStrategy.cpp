#include "stdafx.h"
#include "Farmer2PlayerStrategy.h"
#include "GameTable.h"

Farmer2PlayerStrategy::Farmer2PlayerStrategy(const CardVector & cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue,table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

Farmer2PlayerStrategy::Farmer2PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue,table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

Farmer2PlayerStrategy::~Farmer2PlayerStrategy()
{
}

CardStyle Farmer2PlayerStrategy::Play()
{
	return CardStyle();
}

CardStyle Farmer2PlayerStrategy::Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	return CardStyle();
}

bool Farmer2PlayerStrategy::OtherCanTake(const CardStyle & style) const
{
	return m_table->GetHandCard(EIdentity::Lord)->CanTake(style);
}

bool Farmer2PlayerStrategy::IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const
{
	return false;
}

std::vector<ECardStyle::ECardStyle_> Farmer2PlayerStrategy::AvoidPlayStyle()
{
	return std::vector<ECardStyle::ECardStyle_>();
}

EIdentity::EIdentity_ Farmer2PlayerStrategy::Identity()const
{
	return EIdentity::EIdentity_::Farmer2;
}

void Farmer2PlayerStrategy::Init()
{
}
