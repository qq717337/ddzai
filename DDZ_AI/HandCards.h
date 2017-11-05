#pragma once
#include "HandCardsFlag.h"
#include <set>
#include <string>
#include<unordered_set>

class HandCardsMemnto;

class HandCards :
	public HandCardsFlag
{
	friend class HandCardsMemnto;
private:
	std::vector<uint8_t>CardCount;
	std::set<uint8_t, CardSetCompare> CardsSet;
public:
	HandCards();
	HandCards(const HandCards&) = delete;
	HandCards(const std::vector<uint8_t>&  cardValues, bool updateSet = true); 
	HandCards(const std::set<uint8_t, CardSetCompare>& cardValues, bool updateSet = true);
	~HandCards();
	bool HasCard(uint8_t cardValue);
	void RemoveJokerBoom();
	virtual void UpdateByFlag();

	virtual std::string ToString();
	virtual void Reset(bool createNewCard)override;
	virtual size_t Size() const override;

	//可以通过判断上一次是否对卡牌造成改变来决定是否需要重新Update
	virtual std::vector<uint8_t> AvailableBoom()override;
	virtual std::vector<uint8_t> AvailableTriple()override;
	virtual std::vector<uint8_t> AvailableDouble()override;
	virtual std::vector<uint8_t> AvailableSingle()override;
	virtual std::vector<uint8_t> AvailableBoom(bool bigger,uint8_t cardIndex);
	virtual std::vector<uint8_t> AvailableTriple(bool bigger, uint8_t cardIndex);
	virtual std::vector<uint8_t> AvailableDouble(bool bigger, uint8_t cardIndex);
	virtual std::vector<uint8_t> AvailableSingle(bool bigger, uint8_t cardIndex);
	virtual std::vector<uint8_t> AvailableChain(int len, int count)override;
	virtual std::vector<uint8_t> AvailableChain(int len, int count,bool bigger,uint8_t cardIndex);
	std::vector<std::vector<uint8_t>> IsolateCards(bool sub=false);

	inline int Count(int cardIndex) {
		return CardCount[cardIndex];
	}
	inline std::set<uint8_t, CardSetCompare>&Data() {
		return  CardsSet;
	}
	inline const std::vector<uint8_t>& Count() {
		return CardCount;
	}
};

