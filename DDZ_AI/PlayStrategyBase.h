#pragma once
#include "HandCards.h"
#include<memory>
/*
可以在这里面添加不同的SplitStrategy
*/
class GameTable;//前置声明后将GameTable.h头文件包含在cpp中
class SplitStrategy;
class PlayStrategyHandle;

class PlayStrategyBase
{
protected:
	friend class LordPlayStrategy;
	friend class Farmer1PlayerStrategy;
	friend class Farmer2PlayerStrategy;

	GameTable *m_table;
	std::shared_ptr<HandCards> m_handCards;

	std::shared_ptr<SplitStrategy>m_minStepSplitStrategy;
	std::unique_ptr<PlayStrategyHandle>m_handlerMinValuePlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerLastShotPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerTwoStepPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerAvoidOtherWinPlay;

	std::unique_ptr<PlayStrategyHandle>m_handlerAvailableTake;
	std::unique_ptr<PlayStrategyHandle>m_handlerBoomTake;
	std::unique_ptr<PlayStrategyHandle>m_handlerOptimiumTake;
	std::vector<CardStyle> m_realAvoidStyle;
	
	bool m_mustTake;
public:
	//是否是必须要接，指示对方只有一手牌就可以取得胜利，如果有，在接牌的时候还要额外考虑不被胜利的情况
	bool MustTake();
	virtual void Init();
	virtual CardStyle Play() = 0;
	virtual CardStyle Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle) = 0;
	virtual bool OtherCanTake(const CardStyle& style)const = 0;

	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level, int param1, void* param2 = nullptr) const = 0;

	virtual std::vector<ECardStyle::ECardStyle_> AvoidPlayStyle()=0;
	void RealAvoidStyle();
	virtual EIdentity::EIdentity_ Identity()const = 0;
	const CardStyle& GetLastCardStyle()const;

	virtual bool OtherBiggestCardValue(int compareCount)const=0;
	//获取当前可以接到的牌，然后遍历剩余的牌，看是否只有一个是别人要不起的牌，如果是则代表胜利了，并将所有的出牌顺序存入一个vector中。
	virtual bool CheckIfWin(const SplitStrategy* split, const CardStyle& style, bool isTake, std::vector<CardStyle>& styleList)const;
	//virtual bool IsLetOtherWin()const = 0;
	PlayStrategyBase(int identity, const  CardVector & cardsValue, GameTable *table);
	PlayStrategyBase(int identity, const  std::set<uint8_t, CardSetCompare>& cardsValue, GameTable *table);
	virtual ~PlayStrategyBase();
	virtual void Reset(const  CardVector & cardsValue);
	virtual void Reset(const  std::set<uint8_t, CardSetCompare>& cardsValue);

	const PlayStrategyBase *GetStrategyPtr(EIdentity::EIdentity_ identity);
	inline const HandCards* GetHandCards()const {
		_ASSERT(m_handCards);
		return m_handCards.get();
	}
	inline std::vector<CardStyle>& GetRealAvoidStyle() {
		return m_realAvoidStyle;
	}
};