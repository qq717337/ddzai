#include "stdafx.h"
#include "PlayStrategyBase.h"

PlayStrategyBase::PlayStrategyBase(int identity, const std::vector<uint8_t>& cardsValue)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

PlayStrategyBase::PlayStrategyBase(int identity, const std::set<uint8_t, CardSetCompare>& cardsValue)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

void PlayStrategyBase::Reset(const std::vector<uint8_t>& cardsValue)
{
	m_handCards.reset(new HandCards( cardsValue));
}

void PlayStrategyBase::Reset(const std::set<uint8_t, CardSetCompare>& cardsValue)
{
	m_handCards.reset(new HandCards(cardsValue));
}

PlayStrategyBase::~PlayStrategyBase()
{
	//.if (m_handCards != nullptr)delete m_handCards;
}
