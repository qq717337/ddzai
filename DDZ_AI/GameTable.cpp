#include "stdafx.h"
#include "GameTable.h"


GameTable::GameTable(const CardSet& cardSet):debug_step(0)
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

CardStyle GameTable::Play(EIdentity::EIdentity_ identity)
{
	auto playStyle = m_playerStrategy[identity]->Play();
	return playStyle;
}

CardStyle GameTable::Take(EIdentity::EIdentity_ identity, EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_lastCardStyle = lastStyle;
	//如果是王炸的话，则在此处就要不起
	if (m_lastCardStyle == CardStyle::JokerBoom) {
		return CardStyle::Invalid;
	}
	auto playStyle = m_playerStrategy[identity]->Take(lastIdentity, lastStyle);
	return playStyle;
}

size_t GameTable::CardCount(EIdentity::EIdentity_ identity)const
{
	auto handCards = m_playerStrategy[identity]->GetHandCards();
	return handCards->Size();
}

bool GameTable::IsStyleOtherCanNotTake(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)const
{
	if (lastIdentity == EIdentity::Lord) {
		return !GetHandCard(EIdentity::Farmer1)->CanTake(lastStyle) && !GetHandCard(EIdentity::Farmer2)->CanTake(lastStyle);
	}
	else {
		return !GetHandCard(EIdentity::Lord)->CanTake(lastStyle);
	}
	return false;
}

const HandCards* GameTable::GetHandCard(EIdentity::EIdentity_ identity) const
{
	return m_playerStrategy[identity]->GetHandCards();
}
HandCards* GameTable::GetHandCard(EIdentity::EIdentity_ identity)
{
	return const_cast<HandCards*>(m_playerStrategy[identity]->GetHandCards());
}
uint8_t GameTable::BiggestCardValue(EIdentity::EIdentity_ identity,int count)
{
	if (identity == EIdentity::Lord) {
		int max = 0;
		auto f1_cards = GetHandCard(EIdentity::Farmer1);
		for (int i = CardIndex_LargeJoker; i >= CardIndex_3; --i) {
			auto i_count = f1_cards->Count(i);
			if (i_count >= count) {
				max = i;
				break;
			}
		}
		auto f2_cards = GetHandCard(EIdentity::Farmer2);
		for (int i = CardIndex_LargeJoker; i >= CardIndex_3; --i) {
			auto i_count = f2_cards->Count(i);
			if (i_count >= count && i>=max) {
				return i;
			}
		}
		return max;
	}
	else {
		auto lord_cards = GetHandCard(EIdentity::Lord);
		for (int i = CardIndex_LargeJoker; i >= CardIndex_3; --i) {
			auto i_count=lord_cards->Count(i);
			if (i_count >= count) {
				return i;
			}
		}
	}
	return 0;
}
const PlayStrategyBase* GameTable::GetPlayStrategy(EIdentity::EIdentity_ identity) const
{
	return m_playerStrategy[identity].get();
}

const bool GameTable::ShouldAskLord(const SplitType & splitInfo, const HandCards & cards)
{
	if (splitInfo.GetBoom().size() >= 2) {
		return true;
	}
	if (splitInfo.MinStepCount() <= 6) {
		if (cards.Count(CardIndex_SmallJoker) * 4 + cards.Count(CardIndex_LargeJoker) * 5 + cards.Count(CardIndex_2) * 3 + cards.Count(CardIndex_A) * 2 > 10)
			return true;
	}
	return false;
}
