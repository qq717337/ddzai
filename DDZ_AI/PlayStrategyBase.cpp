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
	for (auto& v : mergedStyles) {//如果所有的最优解中对方都可以接牌，则表示这边已经无法直接取得胜利
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

	if (finded_elem == allStyle.end()) {//如果Chain无法被完美接住，产生其他牌，需要重新进行Split
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

	if (canTakeCount <= 1) {//至多只有一个可以被其他人压制，那么可以将这一个最后出实现胜利
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
	//不可以在父类的构造函数中调用子类的虚方法
	//尝试在此处调用Init函数会报链接错误
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
