#include "stdafx.h"
#include "Farmer1PlayerStrategy.h"
#include "GameTable.h"
#include"SplitStrategy.h"
#include "MinStepSplitStrategy.h"
#include "HandleLetFarmer2WinPlay.h"

CardStyle Farmer1PlayerStrategy::takeLord(const CardStyle & lastStyle)
{
	CardStyle ret;
	if (m_handlerOptimiumTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {//����С�����ɽ��Ƶ����
		return ret;
	}
	if (m_handlerCanTake->Handle(this, m_minStepSplitStrategy.get(), ret)) {
		return ret;
	}
	return CardStyle::Invalid;
}

CardStyle Farmer1PlayerStrategy::takeFarmer(const CardStyle & lastStyle)
{
	//��ΪFarmer2�Ѿ������˵���Ҫ�������������Բ���Ҫ�ٶ�����з�ֹ����Ҫ��ֹ�ľ��Ǹ�����³����ƻᵼ�µ�����ʤ
	//��ѹ�����
	//1.Ҫ����
	//2.Farmer2 ���ƻ��õ���ѹ��
	//3.Farmer2 �޷�ȡ��ʤ��������ȫ��5�ţ���ֻ+���ӣ�
	auto& optTakeStyle = m_minStepSplitStrategy->GetOptimiumStyle();
	//auto& optBoomTakeStyle = m_minStepSplitStrategy->GetOptimiumBoomStyle();
	if (optTakeStyle.size() == 0) {
		return CardStyle::Invalid;
	}
	const SplitType& minStepSplit = GetStrategyPtr(EIdentity::Farmer2)->m_minStepSplitStrategy->MinStepSplit();
	auto allStyles = minStepSplit.GetAllSplitStyle();

	if (allStyles.size() == 1) {
		return CardStyle::Invalid;
	}
	auto& f2_handcards = GetStrategyPtr(EIdentity::Farmer2)->GetHandCards();
	auto& lord_handcards = GetStrategyPtr(EIdentity::Lord)->GetHandCards();

	int beatedCount = 0;//�ɱ�ѹ�Ƶ���f2�޷�ѹ�Ƶ�Style�������������1������ô�Ҿ�ѡ����ƣ����򲻽�
	for (auto &v : allStyles) {
		CardStyle ret;
		bool lordCanTake = lord_handcards.CanTake(v, ret);//����������Խ��ƣ�����f2�޷���ס����ѹ���ƣ���ô���һ����ѹ�Ƶĸ���
		if (lordCanTake && !f2_handcards.CanTake(ret)) {
			++beatedCount;
		}
	}
	if (allStyles.size() - beatedCount > 1) {
		return	optTakeStyle[0];
	}

	return CardStyle::Invalid;
}

Farmer1PlayerStrategy::~Farmer1PlayerStrategy()
{
}

Farmer1PlayerStrategy::Farmer1PlayerStrategy(const CardVector & cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
}

Farmer1PlayerStrategy::Farmer1PlayerStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable* table) : PlayStrategyBase(Identity(), cardsValue, table)
{
}

CardStyle Farmer1PlayerStrategy::Play()
{
	CardStyle r;
	m_minStepSplitStrategy->Split();
	if (m_handlerLastShotPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
		return r;
	}
	if (m_handlerLetFarmer2WinPlay->Handle(this, m_minStepSplitStrategy.get(), r)) {
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

CardStyle Farmer1PlayerStrategy::Take(EIdentity::EIdentity_ lastIdentity, const CardStyle & lastStyle)
{
	m_minStepSplitStrategy->Split();
	m_minStepSplitStrategy->OptimiumTake(lastStyle);
	m_minStepSplitStrategy->AvailableTake(lastStyle);

	std::vector<CardStyle> x;
	bool isWin = CheckIfWin(m_minStepSplitStrategy.get(), lastStyle,true, x);
	if (isWin) {
		return x[0];
	}
	if (lastIdentity == EIdentity::Lord) {
		return takeLord(lastStyle);
	}
	else {
		return takeFarmer(lastStyle);
	}
}

bool Farmer1PlayerStrategy::OtherCanTake(const CardStyle & style) const
{
	return m_table->GetHandCard(EIdentity::Lord)->CanTake(style);
}

bool Farmer1PlayerStrategy::IsSafeSituation(ESituationSafeLevel::ESituationSafeLevel_ level) const
{
	return false;
}

std::vector<ECardStyle::ECardStyle_> Farmer1PlayerStrategy::AvoidPlayStyle()
{
	std::vector<ECardStyle::ECardStyle_> r;
	auto lordCardsCount = m_table->CardCount(EIdentity::Lord);

	if (lordCardsCount == 1) {
		r.push_back(ECardStyle::Single);
	}
	if (lordCardsCount == 2) {
		r.push_back(ECardStyle::Double);
	}
	if (lordCardsCount == 3) {
		r.push_back(ECardStyle::Triple_Zero);
	}
	return r;
}

EIdentity::EIdentity_ Farmer1PlayerStrategy::Identity()const
{
	return EIdentity::EIdentity_::Farmer1;
}

void Farmer1PlayerStrategy::Init()
{
	PlayStrategyBase::Init();
	m_handlerLetFarmer2WinPlay = std::make_unique<HandleLetFarmer2WinPlay>();
}
