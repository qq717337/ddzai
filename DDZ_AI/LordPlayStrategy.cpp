#include "stdafx.h"
#include "LordPlayStrategy.h"
#include"MinStepSplitStrategy.h"
#include "GameTable.h"

CardStyle LordPlayStrategy::Play()
{
	CardStyle r;
	m_minStepSplitStrategy->Split();

	if (m_handlerLastShot->Handle(this,m_minStepSplitStrategy.get(),r)) {
		return r;
	}
	 
	m_handlerMinStepPlay->Handle(this,m_minStepSplitStrategy.get(), r);
	return r;
}

CardStyle LordPlayStrategy::Take(Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_minStepSplitStrategy->Split();
	m_minStepSplitStrategy->OptimiumTake(lastStyle);
	m_minStepSplitStrategy->AvailableTake(lastStyle);
	//先判断使用哪一种拆分策略，或者一开始创建多个SplitStrategy，然后再传入不同的Handle里面进行处理
	CardStyle ret(CardStyle::Invalid);

	if (m_handlerOptimiumTake->Handle(this,m_minStepSplitStrategy.get(), ret)) {//有最小步数可接牌的情况
		return ret;
	}

	if (m_handlerOptimiumTake->Handle(this,m_keepBigSplitStrategy.get(), ret)) {//有保留大牌可接牌的情况
		return ret;
	}

	if (m_handlerCanTake->Handle(this,m_minStepSplitStrategy.get(), ret)) {//以上不符合 但是可以接牌，进行后续判断

	}
	//都不符合就选择不接
	return ret;
}

bool LordPlayStrategy::OtherCanTake(const CardStyle & style) const
{
	
	return false;
}

bool LordPlayStrategy::IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const
{

	return false;
}


int LordPlayStrategy::Identity()
{
	return Identity::Lord;
}

LordPlayStrategy::LordPlayStrategy(const std::vector<uint8_t>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
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
	m_handlerCanTake = std::make_unique<HandleCanTake>();
	m_handlerOptimiumTake = std::make_unique<HandleCanOptimiumTake>();
	m_handlerMinStepPlay = std::make_unique<HandleMinValuePlay>();
	m_handlerLastShot = std::make_unique<HandleLastShot>();
	//进行链的组装，即头尾相连，一层套一层  
	//m_handlerCanTake->setNextStrategy(m_handlerOptimiumTake.get());
	//m_handlerOptimiumTake->setNextStrategy(m_handlerMinStepPlay.get());

}


LordPlayStrategy::~LordPlayStrategy()
{

}
