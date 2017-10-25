#include "stdafx.h"
#include "CardSet.h"
#include <iostream>
#include <algorithm>
CardSet::CardSet()
{
	player_num = 3;
}


CardSet::~CardSet()
{
}

uint8_t* CardSet::NewCards()
{
	uint8_t* r=new uint8_t[51];

	for (int c = 1; c < 5; c++) {
		for (int i = 3; i < 16; i++) {
			r[(c - 1) * 13 + (i-3)] = (16 * c + i);
		}
	}
	r[49] = 1;
	r[50] = 2;
	return r;
}

std::set<uint8_t> CardSet::ResetPlayerCard(int randomDealCount) {
	uint8_t* pureDesk = NewCards();
	std::random_shuffle(pureDesk, pureDesk + 51);
	//for (int i = 0; i < 51; i++) {
	//	std::cout << pureDesk[i] << std::endl;
	//}
	int index = 0;
	for (int i = 0; i < player_num; i++) {
		for (int j = 0; j < randomDealCount; j++) {

			++index;
		}
	}
	return DeskCardSet;
	//("desk cards length:=%d,value=%x\n", p.DeskCardSet.Cardinality(), p.DeskCardSet.ToIntSlice())
	//	fmt.Printf("p0=%x , p1=%x , p2=%x\n", p.PlayerCardSet[0].ToIntSlice(), p.PlayerCardSet[1].ToIntSlice(), p.PlayerCardSet[2].ToIntSlice())
}
