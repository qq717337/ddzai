#ifndef _HANDCARDS_H_
#define _HANDCARDS_H_

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
	virtual CardVector  AvailableBoom(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableTriple(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableDouble(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableSingle(bool bigger, uint8_t cardIndex)const;
	virtual CardVector  AvailableChain(int len, int count)const override;
	virtual CardVector  AvailableChain(int len, int count, bool bigger, uint8_t cardIndex)const;
	CardVector AvailableChain_LaiZi(int len, int count, int8_t cardStartIndex)const;
	CardVector AvailableSingleChain_LaiZi(int8_t cardStartIndex, int len)const;
	CardVector AvailableDoubleChain_LaiZi(int8_t cardStartIndex, int len)const;
	CardVector AvailableTripleChain_LaiZi(int8_t cardStartIndex, int len)const;
	bool FindAvailableTriple_LaiZi(bool hasLaizi, uint8_t cardIndex, CardVector& triples)const;

	CardVector RequireSingle(int count, CardVector& excludeIndex, bool hasLaiZi)const;
	CardVector RequireDouble(int count, CardVector& excludeIndex, bool hasLaiZi)const;
	std::vector<CardVector> ListTripleChainExtra(const CardVector & extra, int len)const;
	std::vector<CardVector> FindIsolateCards()const;
	CardVector FindIsolateCardsIndex()const;
	std::vector<CardVector> SubIsolateCards();

	//取最长的风格
	std::vector<CardStyle> FindLeastCardPlay(bool useLaiZi)const;
	std::vector<CardStyle> LongestStyle(bool useLaiZi)const;
	//按照最小值取风格
	std::vector<CardStyle> MinValueStyle(bool useLaiZi)const;
	std::vector<CardStyle> FindAvailableTake(CardStyle & lastStyle, bool hasLaiZi)const;

	bool CanTake(const CardStyle& lastStyle)const;
    bool CanTake(const CardStyle & lastStyle, CardStyle &retStyle)const;
	CardVector GetCardsValue(uint8_t cardIndex, int count)const;
	CardStyle FindLessCardStyle(const CardStyle& style)const;
	std::vector<uint8_t> GetCardsValueByStyle(const CardStyle& style)const;
	std::vector<uint8_t>  GetCardsByStyle(const CardStyle& style)const;
	CardStyle LastShot();
	std::vector<CardRange> AvailableTripleChainRange(bool hasLaiZi = false)const;
	std::vector<CardRange> AvailableDoubleChainRange(bool hasLaiZi = false)const;
	std::vector<CardRange> AvailableSingleChainRange(bool hasLaiZi = false)const;
	inline std::set<uint8_t, CardSetCompare>&Data() {
		return  CardsSet;
	}
	inline const std::set<uint8_t, CardSetCompare>&Data()const {
		return  CardsSet;
	}
	inline int Count(int cardIndex)const {
		return CardCount[cardIndex];
	}
	inline const CardVector & Count()const {
		return CardCount;
	}
};


#endif
