#pragma once
#include "stdafx.h"
#include <vector>
#include "LordPlayStrategy.h"
#include "Farmer1PlayerStrategy.h"
#include "Farmer2PlayerStrategy.h"
#include "CardSet.h"
//��������������ҵĽ���ļ�¼���⣬��������ĳЩ������Mediator 
class GameTable
{
private:
	std::unique_ptr<PlayStrategyBase> m_playerStrategy[3];
	inline PlayStrategyBase* getStrategy(Identity::EIdentity_ identity) {
		return m_playerStrategy[identity].get();
	}
public:
	GameTable(const CardSet& cardSet);
	~GameTable();
	void Play(Identity::EIdentity_ identity);
	void Take(Identity::EIdentity_ identity, Identity::EIdentity_ lastIdentity, const CardStyle& lastStyle);

	size_t CardCount(Identity::EIdentity_ identity)const;
};

