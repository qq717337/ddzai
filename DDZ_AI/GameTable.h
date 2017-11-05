#pragma once
#include "stdafx.h"
#include <vector>
#include "LordPlayStrategy.h"
#include "Farmer1PlayerStrategy.h"
#include "Farmer2PlayerStrategy.h"
#include "CardSet.h"
//将各个玩家所查找的结果的记录在这，用于区分某些条件。Mediator 
class GameTable
{
private:
	std::unique_ptr<PlayStrategyBase> m_playerStrategy[3];
	inline PlayStrategyBase* getStrategy(EIdentity::EIdentity_ identity) {
		return m_playerStrategy[identity].get();
	}
public:
	GameTable(const CardSet& cardSet);
	~GameTable();
	void Play(EIdentity::EIdentity_ identity);
	void Take(EIdentity::EIdentity_ identity, EIdentity::EIdentity_ lastIdentity, const CardStyle& lastStyle);

	size_t CardCount(EIdentity::EIdentity_ identity)const;
	const HandCards* GetHandCard(EIdentity::EIdentity_ identity)const;
	const PlayStrategyBase* GameTable::GetPlayStrategy(EIdentity::EIdentity_ identity) const;
};

