#include "stdafx.h"
#include "PlayStrategyBase.h"

#include "GameTable.h"
PlayStrategyBase::PlayStrategyBase(int identity, const std::vector<uint8_t>& cardsValue, GameTable *table)
{
	m_table = table;
	m_handCards = std::make_shared<HandCards>(cardsValue);
	//不可以在父类的构造函数中调用子类的虚方法
	//尝试在此处调用Init函数会报链接错误
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
