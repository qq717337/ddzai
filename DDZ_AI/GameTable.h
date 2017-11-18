#pragma once
#include "stdafx.h"
#include <vector>
#include "LordPlayStrategy.h"
#include "Farmer1PlayerStrategy.h"
#include "Farmer2PlayerStrategy.h"
#include "CardSet.h"
#include "SplitType.h"

//��������������ҵĽ���ļ�¼���⣬��������ĳЩ������Mediator 
class GameTable
{
private:
	std::unique_ptr<PlayStrategyBase> m_playerStrategy[3];
	inline PlayStrategyBase* getStrategy(EIdentity::EIdentity_ identity) {
		return m_playerStrategy[identity].get();
	}
	CardStyle m_lastCardStyle;
public:
	GameTable(const CardSet& cardSet);
	~GameTable();
	const CardStyle& GetLastCardStyle()const {return m_lastCardStyle; }
	void Play(EIdentity::EIdentity_ identity);
	void Take(EIdentity::EIdentity_ identity, EIdentity::EIdentity_ lastIdentity, const CardStyle& lastStyle);
	size_t CardCount(EIdentity::EIdentity_ identity)const;
	bool IsStyleOtherCanNotTake(EIdentity::EIdentity_ lastIdentity, const CardStyle& lastStyle)const;
	bool inline IsFarmerCanNotTake(const CardStyle& lastStyle)const { return IsStyleOtherCanNotTake(EIdentity::Lord, lastStyle); }
	bool inline IsLordCanNotTake(const CardStyle& lastStyle)const { return IsStyleOtherCanNotTake(EIdentity::Farmer1, lastStyle); }

	const HandCards* GetHandCard(EIdentity::EIdentity_ identity)const;
	const PlayStrategyBase* GameTable::GetPlayStrategy(EIdentity::EIdentity_ identity) const;
	static const bool ShouldAskLord(const SplitType& splitInfo, const HandCards& cards);
};

