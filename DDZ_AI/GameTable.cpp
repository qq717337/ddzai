#include "stdafx.h"
#include "GameTable.h"


GameTable::GameTable(const CardSet& cardSet)
{
	LordPlayStrategy* lordStrategy = new LordPlayStrategy(cardSet.PlayerCardSet[0]->Data(), this);
	Farmer1PlayerStrategy*farmer1Strategy = new Farmer1PlayerStrategy(cardSet.PlayerCardSet[1]->Data(), this);
	Farmer2PlayerStrategy*farmer2Strategy = new Farmer2PlayerStrategy(cardSet.PlayerCardSet[2]->Data(), this);

	lordStrategy->Init(); farmer1Strategy->Init(); farmer2Strategy->Init();
	m_playerStrategy[0].reset(lordStrategy);
	m_playerStrategy[1].reset(farmer1Strategy);
	m_playerStrategy[2].reset(farmer2Strategy);
}


GameTable::~GameTable()
{
}

void GameTable::Play(Identity::EIdentity_ identity)
{
	auto playStyle = m_playerStrategy[identity]->Play();
	auto &handCards=const_cast<HandCards&>( m_playerStrategy[identity]->GetHandCards());
	handCards.RemoveCard(playStyle);
	handCards.UpdateByFlag();
	int size=handCards.Size(); 
}

void GameTable::Take(Identity::EIdentity_ identity, Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_playerStrategy[identity]->Take(lastIdentity, lastStyle);
}

size_t GameTable::CardCount(Identity::EIdentity_ identity)const
{
	auto& handCards = m_playerStrategy[identity]->GetHandCards();
	return handCards.Size();
}
