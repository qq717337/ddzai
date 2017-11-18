#pragma once
#include "HandCards.h"
#include<memory>
/*
��������������Ӳ�ͬ��SplitStrategy
*/
class GameTable;//ǰ��������GameTable.hͷ�ļ�������cpp��
class SplitStrategy;
class PlayStrategyHandle;

class PlayStrategyBase
{
protected:
	GameTable *m_table;
	std::shared_ptr<HandCards> m_handCards;

	std::shared_ptr<SplitStrategy>m_minStepSplitStrategy;
	std::unique_ptr<PlayStrategyHandle>m_handlerMinStepPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerLastShotPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerTwoStepPlay;
	std::unique_ptr<PlayStrategyHandle>m_handlerAvoidOtherWinPlay;

	std::unique_ptr<PlayStrategyHandle>m_handlerCanTake;
	std::unique_ptr<PlayStrategyHandle>m_handlerOptimiumTake;
	//����������������������Ƿ����ѹ����ȡʤ��
	std::unique_ptr<PlayStrategyHandle>m_handlerCheckTwoStepWinTake;
public:
	virtual void Init();
	virtual CardStyle Play() = 0;
	virtual CardStyle Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle) = 0;
	virtual bool OtherCanTake(const CardStyle& style)const = 0;

	virtual bool IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const = 0;
	virtual std::vector<ECardStyle::ECardStyle_> AvoidPlayStyle() = 0;
	virtual EIdentity::EIdentity_ Identity()const = 0;
	const CardStyle& GetLastCardStyle()const;

	//��ȡ��ǰ���Խӵ����ƣ�Ȼ�����ʣ����ƣ����Ƿ�ֻ��һ���Ǳ���Ҫ������ƣ�����������ʤ���ˣ��������еĳ���˳�����һ��vector�С�
	virtual bool  CheckIfWin(const SplitStrategy* split, const CardStyle& style, std::vector<CardStyle>& styleList)const;
	//virtual bool IsLetOtherWin()const = 0;
	PlayStrategyBase(int identity, const  CardVector & cardsValue, GameTable *table);
	PlayStrategyBase(int identity, const  std::set<uint8_t, CardSetCompare>& cardsValue, GameTable *table);
	virtual ~PlayStrategyBase();
	virtual void Reset(const  CardVector & cardsValue);
	virtual void Reset(const  std::set<uint8_t, CardSetCompare>& cardsValue);

	inline const GameTable *Table_Ptr() { return m_table; }
	const PlayStrategyBase *Strategy_Ptr(EIdentity::EIdentity_ identity);
	inline const HandCards& GetHandCards()const {
		_ASSERT(m_handCards);
		return *m_handCards.get();
	}
};