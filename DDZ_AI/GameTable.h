#ifndef _GAMETABLE_H_
#define _GAMETABLE_H_
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
	EIdentity::EIdentity_ m_curIdentity;
	EIdentity::EIdentity_ m_lastIdentity;

public:
	GameTable(const CardSet& cardSet);
	~GameTable();
	const CardStyle& GetLastCardStyle()const { return m_lastCardStyle; }
	CardStyle Play(EIdentity::EIdentity_ identity);
	CardStyle Take(EIdentity::EIdentity_ identity, EIdentity::EIdentity_ lastIdentity, const CardStyle& lastStyle);
	size_t CardCount(EIdentity::EIdentity_ identity)const;
	bool IsStyleOtherCanNotTake(EIdentity::EIdentity_ lastIdentity, const CardStyle& lastStyle)const;
	bool inline IsFarmerCanNotTake(const CardStyle& lastStyle)const { return IsStyleOtherCanNotTake(EIdentity::Lord, lastStyle); }
	bool inline IsLordCanNotTake(const CardStyle& lastStyle)const { return IsStyleOtherCanNotTake(EIdentity::Farmer1, lastStyle); }

	const HandCards* GetHandCard(EIdentity::EIdentity_ identity)const;
	HandCards* GetHandCard(EIdentity::EIdentity_ identity);
	uint8_t BiggestCardValue(EIdentity::EIdentity_ identity, int count);
    const PlayStrategyBase* GetPlayStrategy(EIdentity::EIdentity_ identity) const;
	static const bool ShouldAskLord(const SplitType& splitInfo, const HandCards& cards);

	int debug_step;
	//������Ҫ���е���Ϸ��������֪����Ϸ�����������Ҫ�������������ǰ�����ƻ��н�ɫ ������һ���ƾ�
	static EIdentity::EIdentity_ NextPlayerIdentity(EIdentity::EIdentity_ identity) {
		switch (identity) {
		case EIdentity::Lord:
			return EIdentity::Farmer1;
		case EIdentity::Farmer1:
			return EIdentity::Farmer2;
		case EIdentity::Farmer2:
			return EIdentity::Lord;
		}
		return EIdentity::Lord;;
	}
	void SaveTable();
	void LoadTable(const std::string &f);
};

#endif
