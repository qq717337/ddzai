#include "stdafx.h"
#include "LordPlayStrategy.h"
#include"MinStepSplitStrategy.h"
#include "GameTable.h"

CardStyle LordPlayStrategy::Play()
{
	CardStyle r;
	m_minStepSplitStrategy->Split();
	if (m_handlerLastShotPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
		return r;
	}
	if (m_handlerTwoStepPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
		return r;
	}
	if (m_handlerAvoidOtherWinPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
		return r;
	}

	m_handlerMinStepPlay->Handle(this, m_minStepSplitStrategy.get(), r);
	return r;
}

CardStyle LordPlayStrategy::Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_minStepSplitStrategy->Split();
	m_minStepSplitStrategy->OptimiumTake(lastStyle);
	m_minStepSplitStrategy->AvailableTake(lastStyle);
	//���ж�ʹ����һ�ֲ�ֲ��ԣ�����һ��ʼ�������SplitStrategy��Ȼ���ٴ��벻ͬ��Handle������д���
	CardStyle ret(CardStyle::Invalid);

	std::vector<CardStyle> x;
	bool isWin = CheckIfWin(m_minStepSplitStrategy.get(), CardStyle::DoubleStyle(CardIndex_2), x);
	if (m_handlerOptimiumTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//����С�����ɽ��Ƶ����
		return ret;
	}
	if (m_handlerCheckTwoStepWinTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {
		return ret;
	}
	if (m_handlerOptimiumTake->Handle(this, m_keepBigSplitStrategy.get(), ret)) {//�б������ƿɽ��Ƶ����
		return ret;
	}

	if (m_handlerCanTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//���ϲ����� ���ǿ��Խ��ƣ����к����ж�

	}
	//�������Ͼ�ѡ�񲻽�
	return ret;
}

bool LordPlayStrategy::OtherCanTake(const CardStyle & style) const
{
	return m_table->GetHandCard(EIdentity::Farmer1)->CanTake(style) || m_table->GetHandCard(EIdentity::Farmer2)->CanTake(style);
}

bool LordPlayStrategy::IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const
{
	switch (level)
	{
	case ESituationSafeLevel::AllPlayerMinStepGreater2:
		break;
	case ESituationSafeLevel::AllPlayerMinStepWithOutBoomGreater2:
		break;
	case ESituationSafeLevel::AllPlayerBoomGreater2:
		break;
	default:
		break;
	}
	return false;
}

std::vector<ECardStyle::ECardStyle_> LordPlayStrategy::AvoidPlayStyle()
{
	std::vector<ECardStyle::ECardStyle_> r;
	auto f1CardsCount = m_table->CardCount(EIdentity::Farmer1);
	auto f2CardsCount = m_table->CardCount(EIdentity::Farmer2);

	if (f1CardsCount == 1 || f2CardsCount == 1) {
		r.push_back(ECardStyle::Single);
	}
	if (f1CardsCount == 2 || f2CardsCount == 2) {
		r.push_back(ECardStyle::Double);
	}
	if (f1CardsCount == 3 || f2CardsCount == 3) {
		r.push_back(ECardStyle::Triple_Zero);
	}
	return r;
}


EIdentity::EIdentity_ LordPlayStrategy::Identity()const
{
	return EIdentity::EIdentity_::Lord;
}

LordPlayStrategy::LordPlayStrategy(const CardVector & cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
	m_minStepSplitStrategy = std::make_shared<MinStepSplitStrategy>(m_handCards);
}

LordPlayStrategy::LordPlayStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
	m_minStepSplitStrategy = std::make_shared<MinStepSplitStrategy>(m_handCards);
}

void LordPlayStrategy::Init()
{
	m_handlerMinStepPlay = std::make_unique<HandleMinValuePlay>();
	m_handlerLastShotPlay = std::make_unique<HandleLastShotPlay>(); 
	m_handlerTwoStepPlay = std::make_unique<HandleTwoStepPlay>();
	m_handlerAvoidOtherWinPlay = std::make_unique<HandleAvoidOtherWinPlay>();

	m_handlerCanTake = std::make_unique<HandleCanTake>();
	m_handlerOptimiumTake = std::make_unique<HandleCanOptimiumTake>();
	m_handlerCheckTwoStepWinTake = std::make_unique<HandleTwoStepWinTake>();
	//����������װ����ͷβ������һ����һ��  
	//m_handlerCanTake->setNextStrategy(m_handlerOptimiumTake.get());
	//m_handlerOptimiumTake->setNextStrategy(m_handlerMinStepPlay.get());

}


LordPlayStrategy::~LordPlayStrategy()
{

}
