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
	HandCards(HandCardsFlag& handCardsFlag);
	HandCards(const CardVector& cardValues, bool updateSet = true); 
	HandCards(const std::set<uint8_t, CardSetCompare>& cardValues, bool updateSet = true);

	~HandCards();
	bool HasCard(uint8_t cardValue);
	void RemoveJokerBoom();
	virtual void UpdateByFlag();

	virtual std::string ToString()const;
	virtual void Reset(bool createNewCard)override;
	virtual void Reset(const CardVector& cardValues);
	virtual size_t Size() const override;

	//可以通过判断上一次是否对卡牌造成改变来决定是否需要重新Update
	virtual CardVector  AvailableBoom()const override;
	virtual CardVector  AvailableTriple()const override;
	virtual CardVector  AvailableDouble()const override;
	virtual CardVector  AvailableSingle()const override;
	virtual CardVector  PerfectTriple(int startIndex)const;
	virtual CardVector  PerfectDouble(int startIndex)const;
	virtual CardVector  PerfectSingle(int startIndex)const;
	virtual CardVector  AvailableBoom(bool bigger,uint8_t cardIndex)const;
	virtual CardVector  AvailableTriple(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableDouble(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableSingle(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableChain(int len, int count)const override;
	virtual CardVector  AvailableChain(int len, int count,bool bigger,uint8_t cardIndex)const;
	CardVector AvailableChain_LaiZi(int len, int count, uint8_t cardStartIndex);
	CardVector AvailableSingleChain_LaiZi(uint8_t cardStartIndex, int len);
	CardVector AvailableDoubleChain_LaiZi(uint8_t cardStartIndex, int len);
	CardVector AvailableTripleChain_LaiZi(uint8_t cardStartIndex, int len);
	bool FindAvailableTriple_LaiZi(bool hasLaizi, uint8_t cardIndex, CardVector& triples); 

	CardVector RequireSingle(int count, CardVector& excludeIndex, bool hasLaiZi);
	CardVector RequireDouble(int count, CardVector& excludeIndex, bool hasLaiZi);
	std::vector<CardVector> ListTripleChainExtra(const CardVector & extra, int len);
	std::vector<CardVector> IsolateCards(bool sub=false);

	std::vector<CardStyle> LongestStyle();
	CardStyle MinValueStyle();
	std::vector<CardStyle> FindAvailableTake(CardStyle & lastStyle, bool hasLaiZi);

	bool CanTake(const CardStyle& lastStyle)const;
	bool HandCards::CanTake(const CardStyle & lastStyle, CardStyle &retStyle)const;
	CardVector GetCardsValue(uint8_t cardIndex, int count)const;
	CardStyle FindLessCardStyle(const CardStyle& style)const;
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
	std::vector<uint8_t> GetCardsValueByStyle(const CardStyle& style)const; 
	std::vector<uint8_t>  GetCardsByStyle(const CardStyle& style)const; 
	CardStyle LastShot();
};



