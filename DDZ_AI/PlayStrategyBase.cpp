#include "stdafx.h"
#include "PlayStrategyBase.h"

#include "GameTable.h"
inline const CardStyle & PlayStrategyBase::GetLastCardStyle() const
{
	return m_table->GetLastCardStyle();
}
bool PlayStrategyBase::CheckIfWin(const SplitStrategy * split, const CardStyle & style, std::vector<CardStyle>& styleList) const
{
	CardStyle r;
	if (split->MinStepSplit().GetLastShotCardStyle(&r)) {
		if (r.Compare(style) > 0) {
			return true;
		}
	}

	const std::vector<CardStyle>& optStyles = split->GetOptimiumStyle();
	const std::vector<CardStyle>& optBoomStyles = split->GetOptimiumBoomStyle();
	std::vector<CardStyle> mergedStyles;
	std::copy(optStyles.begin(), optStyles.end(), std::back_inserter(mergedStyles));
	std::copy(optBoomStyles.begin(), optBoomStyles.end(), std::back_inserter(mergedStyles));
	//std::merge(optStyles.begin(), optStyles.end(), optBoomStyles.begin(), optBoomStyles.end(),mergedStyles.begin());
	CardStyle choosedStyle;
	for (auto& v : mergedStyles) {//如果所有的最优解中对方都可以接牌，则表示这边已经无法直接取得胜利
		if (m_table->IsStyleOtherCanNotTake(this->Identity(), v)) {
			choosedStyle = v;
			break;
		}
	}
	if (!choosedStyle.Valid()) {
		return false;
	}
	auto splitType = split->MinStepSplit();
	auto allStyle = splitType.GetAllSplitStyle();
	auto finded_elem= std::find_if(allStyle.begin(), allStyle.end(), [choosedStyle](CardStyle v) {
		return v == choosedStyle;
	});
	allStyle.erase(finded_elem);

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
		return true;
	}
	return false;
}
PlayStrategyBase::PlayStrategyBase(int identity, const std::vector<uint8_t>& cardsValue, GameTable *table)
{
	m_table = table;
	m_handCards = std::make_shared<HandCards>(cardsValue);
	//不可以在父类的构造函数中调用子类的虚方法
	//尝试在此处调用Init函数会报链接错误
}

PlayStrategyBase::PlayStrategyBase(int identity, const std::set<uint8_t, CardSetCompare>& cardsValue, GameTable *table)
{
	m_table = table;
	m_handCards = std::make_shared<HandCards>(cardsValue);
}

void PlayStrategyBase::Reset(const std::vector<uint8_t>& cardsValue)
{
	m_handCards.reset(new HandCards(cardsValue));
}

void PlayStrategyBase::Reset(const std::set<uint8_t, CardSetCompare>& cardsValue)
{
	m_handCards.reset(new HandCards(cardsValue));
}

PlayStrategyBase::~PlayStrategyBase()
{
	//.if (m_handCards != nullptr)delete m_handCards;
}
