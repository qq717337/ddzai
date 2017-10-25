#pragma once
#include <set>

class CardSet
{
private:
	int player_num;
public:
	const int RANDOM_DEALCARDS_NUM = 5; 
	
	std::set<uint8_t> DeskCardSet;
	std::set<uint8_t>PlayerCardSet[3];
	CardSet();
	~CardSet();
	uint8_t* NewCards();
	std::set<uint8_t> ResetPlayerCard( int randomDealCount);
};

