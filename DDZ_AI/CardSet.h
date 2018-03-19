#ifndef _CARDSET_H_
#define _CARDSET_H_
#include <set>
#include <memory>
#include<string>
#include "HandCards.h"
class CardSet
{
private:
	int player_num;
	uint8_t*  NewCards();
protected:
public:
	HandCards ExtraCard;
	HandCards DeskCardSet;
	std::vector<HandCards*>PlayerCardSet;
	CardSet();
    CardSet(const CardSet& rhs);
	virtual ~CardSet();
	void Update();
	size_t LeftCount(int playerId);
	void ResetPlayerCard(int randomDealCount);
	void DealIndex(int playerIndex, uint8_t cardIndx, int count = 1);
	void DealCard(int playerIndex, uint8_t cardValue);
	void DealChain(int playerIndex, uint8_t startIndex, uint8_t endIndex, int count = 1);

	void RandomFillLeft();

	virtual std::string ToString();
	virtual std::string ToString2();

#pragma region inline function

	inline void DealJokerBoom(int playerIndex) {
		DealIndex(playerIndex, CardIndex_SmallJoker);
		DealIndex(playerIndex, CardIndex_LargeJoker);
	}

	inline bool HasSmallJoker() {
		return DeskCardSet.GetFlag(CardIndex_SmallJoker, 0) != 0;
	}

	inline  bool HasLargeJoker() {
		return DeskCardSet.GetFlag(CardIndex_LargeJoker, 0) != 0;
	}
#pragma endregion

};
#endif
