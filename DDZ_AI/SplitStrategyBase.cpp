#include "stdafx.h"
#include "SplitStrategyBase.h"
#include <algorithm>

CardRange SplitStrategyBase::findNextRepeatCardRange(CardRange range)
{
	int startIndex = range.Start;
	int endIndex = range.End;
	int lastNotSingleIndex = -1;
	int firstNotSingleIndex = -1;
	for (int i = startIndex; i <= endIndex; i++) {
		if (lastNotSingleIndex > -1) {
			if (m_cards->Count(i) == 1) {
				break;
			}
			if (m_cards->Count(i) > 1) {
				lastNotSingleIndex = i;
			}
		}
		else {
			if (m_cards->Count(i) > 1) {
				firstNotSingleIndex = i;
				lastNotSingleIndex = i;
			}
		}
	}
	return CardRange(firstNotSingleIndex, lastNotSingleIndex);
}

CardRange SplitStrategyBase::getIntersectSingleChainPart(CardRange singleChainRange, bool trimHead, bool trimTail)
{
	int singleChainCount = singleChainRange.End - singleChainRange.Start + 1;
	if (singleChainCount <= 5) {
		return singleChainRange;
	}
	int lastSingleIndex = -1;
	if (trimTail) {
		for (int j = singleChainRange.End; j >= singleChainRange.Start + 5; --j) {
			if (m_cards->Count(j) == 1) {
				lastSingleIndex = j;
				break;
			}
		}
	}
	else {
		lastSingleIndex = singleChainRange.End;
	}

	if (lastSingleIndex < 0) {
		lastSingleIndex = singleChainRange.Start + 4;
	}

	int  firstSingleIndex = -1;
	if (trimHead) {
		for (int i = singleChainRange.Start; i <= lastSingleIndex - 4; ++i) { //先判断前面的几张牌是不是不为1，然后在判断后面的几张牌数是不是1
			if (m_cards->Count(i) == 1) {
				firstSingleIndex = i;
				break;
			}
		}
	}
	else {
		firstSingleIndex = singleChainRange.Start;
	}

	if (firstSingleIndex < 0) {
		firstSingleIndex = lastSingleIndex - 4;
	}

	return CardRange(firstSingleIndex, lastSingleIndex);
}

void SplitStrategyBase::recursivelyFindTwoChain(CardRange findRange, int findStart)
{
	CardRange repeatRange = findNextRepeatCardRange(CardRange(findStart, findRange.End));
	int	x = repeatRange.Start;
	int	y = repeatRange.End;
	if (repeatRange.Start > CardIndex_LargeJoker) {
		return;
	}

	Recorder<HandCards>::Push(new HandCardsMemnto(*m_cards));

	if (y - findRange.Start >= 4) {
		CardRange r0 = getIntersectSingleChainPart(CardRange(findRange.Start, y), true, false);

		m_cards->RemoveChain(r0.Start, r0.End, 1);
		m_cards->UpdateByFlag();
		UPDATE_SINGLECHAIN
			if (m_singleChains.size() > 0) {
				int len = GetLongestSingleChain();
				CardRange r1;
				if (len >= 0) {
					r1 = m_singleChains[len];
				}
				r1 = getIntersectSingleChainPart(r1, true, true);
				m_cards->RemoveChain(r1.Start, r1.End, 1);
				m_cards->UpdateByFlag();

				m_splitType->AddSingleChain(r0.Start, r0.End);
				m_splitType->AddSingleChain(r1.Start, r1.End);
				Recorder<HandCards>::Pop(NULL);
				return;
			}
			else {
				Recorder<HandCards>::Pop(m_cards.get());
				if (y + 1 < findRange.End) {
					recursivelyFindTwoChain(findRange, y + 1);
				}
			}
	}
	else {
		Recorder<HandCards>::Pop(NULL);
		recursivelyFindTwoChain(findRange, y + 1);
	}
}

SplitStrategyBase::SplitStrategyBase(std::shared_ptr<HandCards> cards) :m_cards(cards), m_splitType(new SplitType())
//m_triples(m_cards->AvailableTriple()),
//m_booms(m_cards->AvailableBoom()),
//m_tripleChains(m_cards->AvailableTripleChainRange()),
//m_doubleChains(m_cards->AvailableDoubleChainRange()),
//m_singleChains(m_cards->AvailableSingleChainRange())
{
}

SplitStrategyBase::~SplitStrategyBase()
{
	//delete m_splitType;
}

//提取出孤立的牌，且将大小王炸也提取出来
void SplitStrategyBase::SplitIsolate() {
	std::vector<CardVector > isolateCards = m_cards->IsolateCards(true);
	if (isolateCards[CardIndex_SmallJoker][0] == 1 && isolateCards[CardIndex_LargeJoker][0] == 1) {
		m_splitType->AddBoom(CardIndex_JokerBoom);
	}
	else {
		if (isolateCards[CardIndex_SmallJoker][0] > 0) {
			m_splitType->AddSingle(CardIndex_SmallJoker);
		}
		if (isolateCards[CardIndex_LargeJoker][0] > 0) {
			m_splitType->AddSingle(CardIndex_LargeJoker);
		}
	}
	for (int i = 0; i <= CardIndex_2; i++) {
		int sum = isolateCards[i][0] + isolateCards[i][1] + isolateCards[i][2] + isolateCards[i][3];
		switch (sum) {
		case 1:
			m_splitType->AddSingle(i);
			break;
		case 2:
			m_splitType->AddDouble(i);
			break;
		case 3:
			m_splitType->AddTriple(i);
			break;
		case 4:
			m_splitType->AddBoom(i);
			break;
		}
	}
	m_cards->UpdateByFlag();
}

void SplitStrategyBase::SplitBoom(bool once) {
	UPDATE_BOOM
		for (auto iter = m_booms.begin(); iter != m_booms.end(); ) {
			uint8_t v = *iter;
			if (v >= CardIndex_SmallJoker) {
				m_splitType->AddBoom(CardIndex_SmallJoker);
				m_cards->RemoveJokerBoom();
			}
			else {
				m_splitType->AddBoom(v);
				m_cards->RemoveIndex(v, 4);
			}
			iter = m_booms.erase(iter);
			if (once)break;
		}
	m_cards->UpdateByFlag();
}

void SplitStrategyBase::SplitTriple(bool once) {
	UPDATE_TRIPLE
		for (auto iter = m_triples.begin(); iter != m_triples.end(); ) {
			uint8_t v = *iter;
			m_splitType->AddTriple(v);
			m_cards->RemoveIndex(v, 3);
			iter = m_triples.erase(iter);
			if (once)break;
		}
	m_cards->UpdateByFlag();
}

void SplitStrategyBase::SplitTripleChain(bool once) {
	UPDATE_TRIPLECHAIN
		for (auto iter = m_tripleChains.begin(); iter != m_tripleChains.end(); ) {
			CardRange v = *iter;
			m_splitType->AddTripleChain(v.Start, v.End);
			m_cards->RemoveChain(v.Start, v.End, 3);
			iter = m_tripleChains.erase(iter);
			if (once)break;
		}
	m_cards->UpdateByFlag();
}

void SplitStrategyBase::SplitDoubleChain(bool once) {
	UPDATE_DOUBLECHAIN
		for (auto iter = m_doubleChains.begin(); iter != m_doubleChains.end(); ) {
			CardRange v = *iter;
			m_splitType->AddDoubleChain(v.Start, v.End);
			m_cards->RemoveChain(v.Start, v.End, 2);
			iter = m_doubleChains.erase(iter);
			if (once)break;
		}
	m_cards->UpdateByFlag();
}

void SplitStrategyBase::SplitSingleChain(bool once)
{
	UPDATE_SINGLECHAIN
		for (auto iter = m_singleChains.begin(); iter != m_singleChains.end();) {
			CardRange v = *iter;
			m_splitType->AddSingleChain(v.Start, v.End);
			m_cards->RemoveChain(v.Start, v.End, 1);
			iter = m_singleChains.erase(iter);
			if (once)break;
		}
	m_cards->UpdateByFlag();
}

void SplitStrategyBase::SplitSingleChainTruncBoom(bool once)
{
	uint8_t boomIndex = 0xff;
	bool couldTrunc = CouldTruncBoom(&boomIndex);
	if (boomIndex == 0xff)return;
	if (couldTrunc) {
		m_splitType->AddTriple(boomIndex);
		m_cards->RemoveIndex(boomIndex, 3);
	}
	else {
		m_splitType->AddBoom(boomIndex);
		m_cards->RemoveIndex(boomIndex, 4);
	}

	auto iter = std::find(m_booms.begin(), m_booms.end(), boomIndex);
	_ASSERTE(iter != m_booms.end());
	m_booms.erase(iter);
	m_cards->UpdateByFlag();
}

CardVector  SplitStrategyBase::FindInChain(const std::vector<CardRange>& chain, size_t count)
{
	CardVector  r;
	for (auto& c : chain) {
		for (int v = c.Start; v <= c.End; ++v) {
			int index = v;
			if (m_cards->Count(index) == count) {
				r.push_back(index);
			}
		}
	}
	return r;
}

bool SplitStrategyBase::CouldTruncBoom(uint8_t * boomIndex)
{
	UPDATE_BOOM
		UPDATE_SINGLECHAIN
		CardVector  booms = FindBoomInSingleChain();
	for (auto& v : booms) {
		int i;
		int aheadSingleCount = 0;
		int	tailSingleCount = 0;
		for (i = v - 1; i >= 0; --i) {
			if (m_cards->Count(i) == 1) {
				++aheadSingleCount;
			}
			else {
				break;
			}
		}
		for (i = v + 1; i < CardIndex_2; ++i) {
			if (m_cards->Count(i) == 1) {
				++tailSingleCount;
			}
			else {
				break;
			}
		}

		if (boomIndex != NULL) { *boomIndex = v; }
		if (aheadSingleCount + tailSingleCount >= 4 && aheadSingleCount < 5 && tailSingleCount < 5) {
			return true;
		}
	}
	return false;
}

int SplitStrategyBase::GetLongestSingleChain()
{
	UPDATE_SINGLECHAIN
		int max_index = -1;
	int max = 0;
	for (int i = 0; i < m_singleChains.size(); ++i) {
		CardRange v = m_singleChains[i];
		if (v.Length() > max) {
			max = v.Length();
			max_index = i;
		}
	}
	return max_index;
}
//备份手牌HandCards
void SplitStrategyBase::Backup()
{
	Recorder<HandCards>::Push(new HandCardsMemnto(*m_cards));
}
//恢复上一次备份的手牌HandCards
void SplitStrategyBase::Restore()
{
	Recorder<HandCards>::Pop(m_cards.get());
}

void SplitStrategyBase::SplitIntersectChain()
{
	//UPDATE_SINGLECHAIN
	int index = GetLongestSingleChain();
	if (index < 0)return;

	CardRange singleChainRange = m_singleChains[index];
	int rangeCount = 0;
	for (int i = singleChainRange.Start; i <= singleChainRange.End; ++i) {
		rangeCount += m_cards->Count(i);
	}
	if (singleChainRange.Length() < 6 || rangeCount < 5 * 2) { //<5 或者所有的联排数量小于10张 则判定无法分成两个顺子
		return;
	}

	recursivelyFindTwoChain(singleChainRange, singleChainRange.Start);
}