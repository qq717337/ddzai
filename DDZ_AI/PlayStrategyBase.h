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
	//�Ƿ��Ǳ���Ҫ�ӣ�ָʾ�Է�ֻ��һ���ƾͿ���ȡ��ʤ��������У��ڽ��Ƶ�ʱ��Ҫ���⿼�ǲ���ʤ�������
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
	//��ȡ��ǰ���Խӵ����ƣ�Ȼ�����ʣ����ƣ����Ƿ�ֻ��һ���Ǳ���Ҫ������ƣ�����������ʤ���ˣ��������еĳ���˳�����һ��vector�С�
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