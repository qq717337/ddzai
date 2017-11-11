#pragma once
#include "SplitStrategy.h"
#include<memory>
/*
��������������Ӳ�ͬ��SplitStrategy
*/
class GameTable;//ǰ��������GameTable.hͷ�ļ�������cpp��

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

	//��ȡ��ǰ���Խӵ����ƣ�Ȼ�����ʣ����ƣ����Ƿ�ֻ��һ���Ǳ���Ҫ������ƣ�����������ʤ���ˣ��������еĳ���˳�����һ��vector�С�
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