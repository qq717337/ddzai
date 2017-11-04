#include "stdafx.h"
#include "Farmer2PlayerStrategy.h"


Farmer2PlayerStrategy::Farmer2PlayerStrategy(const std::vector<uint8_t>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue,table)
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

CardStyle Farmer2PlayerStrategy::Take(Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	return CardStyle();
}

int Farmer2PlayerStrategy::Identity()
{
	return 2;
}

void Farmer2PlayerStrategy::Init()
{
}
