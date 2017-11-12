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
	CardVector CardCount;
	std::set<uint8_t, CardSetCompare> CardsSet;
public:
	HandCards();
	HandCards(const HandCards&) = delete;
	HandCards(const CardVector &  cardValues, bool updateSet = true); 
	HandCards(const std::set<uint8_t, CardSetCompare>& cardValues, bool updateSet = true);
	~HandCards();
	bool HasCard(uint8_t cardValue);
	void RemoveJokerBoom();
	virtual void UpdateByFlag();

	virtual std::string ToString();
	virtual void Reset(bool createNewCard)override;
	virtual size_t Size() const override;

	//可以通过判断上一次是否对卡牌造成改变来决定是否需要重新Update
	virtual CardVector  AvailableBoom()const override;
	virtual CardVector  AvailableTriple()const override;
	virtual CardVector  AvailableDouble()const override;
	virtual CardVector  AvailableSingle()const override;
	virtual CardVector  AvailableBoom(bool bigger,uint8_t cardIndex)const;
	virtual CardVector  AvailableTriple(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableDouble(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableSingle(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableChain(int len, int count)const override;
	virtual CardVector  AvailableChain(int len, int count,bool bigger,uint8_t cardIndex)const;
	std::vector<CardVector > IsolateCards(bool sub=false);
	bool CanTake(const CardStyle& lastStyle)const;
	CardVector  GetCardsValue(uint8_t cardIndex, int count);
	inline int Count(int cardIndex)const {
		return CardCount[cardIndex];
	}
	inline std::set<uint8_t, CardSetCompare>&Data(){
		return  CardsSet;
	}
	inline const std::set<uint8_t, CardSetCompare>&Data()const {
		return  CardsSet;
	}
	inline const CardVector & Count()const {
		return CardCount;
	}
};

