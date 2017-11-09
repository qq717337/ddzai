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
	virtual std::vector<uint8_t> AvailableBoom()const override;
	virtual std::vector<uint8_t> AvailableTriple()const override;
	virtual std::vector<uint8_t> AvailableDouble()const override;
	virtual std::vector<uint8_t> AvailableSingle()const override;
	virtual std::vector<uint8_t> AvailableBoom(bool bigger,uint8_t cardIndex)const;
	virtual std::vector<uint8_t> AvailableTriple(bool bigger, uint8_t cardIndex)const;
	virtual std::vector<uint8_t> AvailableDouble(bool bigger, uint8_t cardIndex)const;
	virtual std::vector<uint8_t> AvailableSingle(bool bigger, uint8_t cardIndex)const;
	virtual std::vector<uint8_t> AvailableChain(int len, int count)const override;
	virtual std::vector<uint8_t> AvailableChain(int len, int count,bool bigger,uint8_t cardIndex)const;
	std::vector<std::vector<uint8_t>> IsolateCards(bool sub=false);
	bool CanTake(const CardStyle& lastStyle)const;
	inline int Count(int cardIndex)const {
		return CardCount[cardIndex];
	}
	inline std::set<uint8_t, CardSetCompare>&Data(){
		return  CardsSet;
	}
	inline const std::set<uint8_t, CardSetCompare>&Data()const {
		return  CardsSet;
	}
	inline const std::vector<uint8_t>& Count()const {
		return CardCount;
	}
};

