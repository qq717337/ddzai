#pragma once
#include "SplitStrategy.h"
#include<memory>
/*
可以在这里面添加不同的SplitStrategy
*/
class GameTable;//前置声明后将GameTable.h头文件包含在cpp中

class PlayStrategyBase
{
protected:
	GameTable *m_table;
	std::shared_ptr<HandCards> m_handCards;
public:
	virtual void Init() = 0;
	virtual CardStyle Play() = 0;
	virtual CardStyle Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle) = 0;
	virtual bool OtherCanTake(const CardStyle& style)const = 0;

	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const = 0;
	virtual std::vector<ECardStyle::ECardStyle_> AvoidPlayStyle() = 0;
	virtual EIdentity::EIdentity_ Identity()const = 0;
	inline const CardStyle& GetLastCardStyle()const;

	//获取当前可以接到的牌，然后遍历剩余的牌，看是否只有一个是别人要不起的牌，如果是则代表胜利了，并将所有的出牌顺序存入一个vector中。
	virtual bool  CheckIfWin(const SplitStrategy* split, const CardStyle& style,std::vector<CardStyle>& styleList)const;
	//virtual bool IsLetOtherWin()const = 0;
	PlayStrategyBase(int identity, const  std::vector<uint8_t>& cardsValue, GameTable *table);
	PlayStrategyBase(int identity, const  std::set<uint8_t, CardSetCompare>& cardsValue, GameTable *table);
	virtual ~PlayStrategyBase();
	virtual void Reset(const  std::vector<uint8_t>& cardsValue);
	virtual void Reset(const  std::set<uint8_t, CardSetCompare>& cardsValue);

	inline const HandCards& GetHandCards()const {
		_ASSERT(m_handCards);
		return *m_handCards.get();
	}
};