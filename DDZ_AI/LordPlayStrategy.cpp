#include "stdafx.h"
#include "LordPlayStrategy.h"
#include"MinStepSplitStrategy.h"

CardStyle LordPlayStrategy::Play()
{
	return CardStyle::Invalid;
}

CardStyle LordPlayStrategy::Take(Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	//���ж�ʹ����һ�ֲ�ֲ��ԣ�����һ��ʼ�������SplitStrategy��Ȼ���ٴ��벻ͬ��Handle������д���
	CardStyle ret;
	if (m_handlerCanTake->Handle(m_splitStrategy.get(), ret)) {

	}
	else {
		if (handler02->Handle(m_splitStrategy.get(), ret)) {

		}
		else {

		}
	}
	return ret;
}


int LordPlayStrategy::Identity()
{
	return Identity::Lord;
}

LordPlayStrategy::LordPlayStrategy(const std::vector<uint8_t>& cardsValue) : PlayStrategyBase(Identity(), cardsValue)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
	m_splitStrategy = std::make_shared<MinStepSplitStrategy>(m_handCards);
}

LordPlayStrategy::LordPlayStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue) : PlayStrategyBase(Identity(), cardsValue)
{
	m_handCards = std::make_shared<HandCards>(cardsValue);
	m_splitStrategy = std::make_shared<MinStepSplitStrategy>(m_handCards);
}

void LordPlayStrategy::InitResponsibility()
{
	m_handlerCanTake = std::make_unique<HandleCanTakeCard>();
	handler02 = std::make_unique<HandleCanTakeCard>();
	handler03 = std::make_unique<HandleCanTakeCard>();

	// ����������װ����ͷβ������һ����һ��  
	m_handlerCanTake->setNextStrategy(handler02.get());
	handler02->setNextStrategy(handler03.get());

}


LordPlayStrategy::~LordPlayStrategy()
{

}
