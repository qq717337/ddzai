#pragma once
#include "stdafx.h"
#include <vector>
#include "LordPlayStrategy.h"
class GameTable
{
private:
	std::shared_ptr<PlayStrategyBase> m_playerHandCards[3];
	inline PlayStrategyBase* getStrategy(Identity::EIdentity_ identity) {
		return m_playerHandCards[identity].get();
	}
public:
	GameTable();
	~GameTable();
	void Play(Identity::EIdentity_ identity);
	void Take(Identity::EIdentity_ identity, Identity::EIdentity_ lastIdentity, const CardStyle& lastStyle);
};

