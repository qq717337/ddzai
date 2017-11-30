#pragma once
#include"SplitType.h"
#include "HandCards.h"
#include"Recorder.h"
#include<memory>

class SplitStrategyBase
{
#define UPDATE_TRIPLE m_triples=(m_cards->AvailableTriple());
#define UPDATE_BOOM m_booms=(m_cards->AvailableBoom());
#define UPDATE_TRIPLECHAIN m_tripleChains=(m_cards->AvailableTripleChainRange());
#define UPDATE_DOUBLECHAIN m_doubleChains=(m_cards->AvailableDoubleChainRange());
#define UPDATE_SINGLECHAIN m_singleChains=(m_cards->AvailableSingleChainRange());


public:
	SplitStrategyBase(std::shared_ptr<HandCards> cards);
	virtual ~SplitStrategyBase();
private:
	bool m_truncBoom;
protected:
	CardRange findNextRepeatCardRange(CardRange range);
	CardRange getIntersectSingleChainPart(CardRange singleChainRange, bool trimHead, bool trimTail);
	void  recursivelyFindTwoChain(CardRange findRange, int findStart);

	std::shared_ptr<HandCards> m_cards;
	std::shared_ptr<SplitType> m_splitType;

	CardVector  m_booms;
	CardVector  m_triples;
	std::vector<CardRange> m_tripleChains;
	std::vector<CardRange> m_doubleChains;
	std::vector<CardRange> m_singleChains;
public:
	void SplitIsolate();
	void Config(bool truncBoom);

	void SplitTriple(bool once = true);
	void SplitBoom(bool once = true);
	void SplitTripleChain(bool once = true);
	void SplitDoubleChain(bool once = true);
	void SplitSingleChain(bool once = true);
	void SplitSingleChainTruncBoom(bool once = true);
	void SplitIntersectChain();

	CardVector  FindInChain(const std::vector<CardRange>&chain, size_t count);
	virtual bool CouldTruncBoom(uint8_t * boomIndex = NULL);
	int GetLongestSingleChain();
	virtual void Backup();
	virtual void Restore();

#pragma region INLINE_FUNCTION
	inline SplitType* _Split_Ptr() { return m_splitType.get(); }
	inline CardVector   FindBoomInSingleChain() {
		UPDATE_BOOM
			UPDATE_SINGLECHAIN
			return FindInChain(m_singleChains, 4);
	}
	inline CardVector   FindTripleInSingleChain() {
		UPDATE_TRIPLE
			UPDATE_SINGLECHAIN
			return FindInChain(m_singleChains, 3);
	}
	inline CardVector   FindBoomInDoubleChain() {
		UPDATE_BOOM
			UPDATE_DOUBLECHAIN
			return FindInChain(m_doubleChains, 4);
	}
	inline CardVector   FindTripleInDoubleChain() {
		UPDATE_TRIPLE
			UPDATE_DOUBLECHAIN
			return FindInChain(m_doubleChains, 3);
	}
#pragma endregion

};

