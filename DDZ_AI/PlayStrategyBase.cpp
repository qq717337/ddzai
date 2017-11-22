#include "stdafx.h"
#include "PlayStrategyBase.h"
#include "MinStepSplitStrategy.h"
#include "HandleOptimiumTake.h"
#include "HandleAvailableTake.h"
#include "HandleMinValuePlay.h"
#include "HandleLastShotPlay.h"
#include "HandleAvoidOtherWinPlay.h"
#include "HandleTwoStepPlay.h"
#include "HandleBoomTake.h"
#include "GameTable.h"
const CardStyle & PlayStrategyBase::GetLastCardStyle() const
{
	return m_table->GetLastCardStyle();
}
bool PlayStrategyBase::CheckIfWin(const SplitStrategy * split, const CardStyle & style, bool isTake, std::vector<CardStyle>& styleList) const
{
	CardStyle r;
	if (split->MinStepSplit().GetLastShotCardStyle(&r)) {
		if (r.Compare(style) > 0) {
			styleList.push_back(std::move(r));
			return true;
		}
	}

	const std::vector<HandleResult>& optStyles = split->GetOptimiumStyle();
	const std::vector<HandleResult>& optBoomStyles = split->GetOptimiumBoomStyle();
	std::vector<HandleResult> mergedStyles;
	std::copy(optStyles.begin(), optStyles.end(), std::back_inserter(mergedStyles));
	std::copy(optBoomStyles.begin(), optBoomStyles.end(), std::back_inserter(mergedStyles));
	//std::merge(optStyles.begin(), optStyles.end(), optBoomStyles.begin(), optBoomStyles.end(),mergedStyles.begin());
	CardStyle choosedStyle;
	for (auto& v : mergedStyles) {//������е����Ž��жԷ������Խ��ƣ����ʾ����Ѿ��޷�ֱ��ȡ��ʤ��
		auto& style = v.Style();
		if (m_table->IsStyleOtherCanNotTake(this->Identity(), style)) {
			choosedStyle = style;
			break;
		}
	}
	if (!choosedStyle.Valid()) {
		return false;
	}
	auto splitType = split->MinStepSplit();
	auto allStyle = splitType.GetAllSplitStyle();
	auto finded_elem = std::find_if(allStyle.begin(), allStyle.end(), [choosedStyle](CardStyle v) {
		return v == choosedStyle;
	});

	if (finded_elem == allStyle.end()) {//���Chain�޷���������ס�����������ƣ���Ҫ���½���Split
		Recorder<HandCards>::Push(new HandCardsMemnto(*m_handCards.get()));
		m_handCards->RemoveCard(choosedStyle);
		MinStepSplitStrategy new_split(m_handCards);
		new_split.Split();
		allStyle = new_split.MinStepSplit().GetAllSplitStyle();
		Recorder<HandCards>::Pop(m_handCards.get());
	}
	else {
		if (!isTake) {
			allStyle.erase(finded_elem);
		}
	}
	int canTakeCount = 0;

	for (auto& v : allStyle) {
		if (m_table->IsStyleOtherCanNotTake(this->Identity(), v)) {
			styleList.emplace_back(std::move(v));
		}
		else {
			canTakeCount++;
		}
	}

	if (canTakeCount <= 1) {//����ֻ��һ�����Ա�������ѹ�ƣ���ô���Խ���һ������ʵ��ʤ��
		styleList.insert(styleList.begin(), choosedStyle);
		return true;
	}
	return false;
}
PlayStrategyBase::PlayStrategyBase(int identity, const CardVector & cardsValue, GameTable *table)
{
	m_table = table;
	m_handCards = std::make_shared<HandCards>(cardsValue);
	m_minStepSplitStrategy = std::make_shared<MinStepSplitStrategy>(m_handCards);
	//�������ڸ���Ĺ��캯���е���������鷽��
	//�����ڴ˴�����Init�����ᱨ���Ӵ���
}

PlayStrategyBase::PlayStrategyBase(int identity, const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable *table)
{
	m_table = table;
	m_handCards = std::make_shared<HandCards>(cardsValue);
	m_minStepSplitStrategy = std::make_shared<MinStepSplitStrategy>(m_handCards);
}

void PlayStrategyBase::Reset(const CardVector & cardsValue)
{
	m_handCards.reset(new HandCards(cardsValue));
}

void PlayStrategyBase::Reset(const std::set<uint8_t, CardSetCompare>& cardsValue)
{
	m_handCards.reset(new HandCards(cardsValue));
}

const PlayStrategyBase * PlayStrategyBase::GetStrategyPtr(EIdentity::EIdentity_ identity)
{
	return m_table->GetPlayStrategy(identity);
}

PlayStrategyBase::~PlayStrategyBase()
{
	//.if (m_handCards != nullptr)delete m_handCards;
}

void PlayStrategyBase::Init()
{
	m_handlerMinValuePlay = std::make_unique<HandleMinValuePlay>();
	m_handlerLastShotPlay = std::make_unique<HandleLastShotPlay>();
	m_handlerTwoStepPlay = std::make_unique<HandleTwoStepPlay>();
	m_handlerAvoidOtherWinPlay = std::make_unique<HandleAvoidOtherWinPlay>();

	m_handlerAvailableTake = std::make_unique<HandleAvailableTake>();
	m_handlerOptimiumTake = std::make_unique<HandleOptimiumTake>();
	m_handlerBoomTake = std::make_unique<HandleBoomTake>();
}
