#pragma once
#include "SplitStrategy.h"
#include<memory>
/*
可以在这里面添加不同的SplitStrategy
*/
class PlayStrategyBase
{
protected:
	std::shared_ptr<HandCards> m_handCards;
	std::shared_ptr<SplitStrategy>m_splitStrategy;
public:
	virtual CardStyle Play() = 0;
	virtual CardStyle Take(Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle) = 0;
	virtual int Identity() = 0;
	PlayStrategyBase(int identity, const  std::vector<uint8_t>& cardsValue);
	PlayStrategyBase(int identity, const  std::set<uint8_t, CardSetCompare>& cardsValue);
	virtual ~PlayStrategyBase();
	virtual void Reset(const  std::vector<uint8_t>& cardsValue);
	virtual void Reset(const  std::set<uint8_t, CardSetCompare>& cardsValue);

};