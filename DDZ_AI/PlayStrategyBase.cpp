#include "stdafx.h"
#include "PlayStrategyBase.h"

#include "GameTable.h"
PlayStrategyBase::PlayStrategyBase(int identity, const std::vector<uint8_t>& cardsValue, GameTable *table)
{
	m_table = table;
	m_handCards = std::make_shared<HandCards>(cardsValue);
	//�������ڸ���Ĺ��캯���е���������鷽��
	//�����ڴ˴�����Init�����ᱨ���Ӵ���
}

PlayStrategyBase::PlayStrategyBase(int identity, const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable *table)
{
	m_table = table;
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
