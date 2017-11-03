#pragma once
#include <string>

#define CARD_SMALL_JOKER 0x01
#define CARD_LARGE_JOKER 0x02
#define CARD_VALUE_LEN 15
namespace Identity {
	enum EIdentity_ {
		Lord = 0,
		Farmer1 = 1,
		Farmer2 = 2,
	};
};
enum ECardIndex {
	CardIndex_3 = 0,
	CardIndex_4 = 1,
	CardIndex_5 = 2,
	CardIndex_6 = 3,
	CardIndex_7 = 4,
	CardIndex_8 = 5,
	CardIndex_9 = 6,
	CardIndex_10 = 7,
	CardIndex_J = 8,
	CardIndex_Q = 9,
	CardIndex_K = 10,
	CardIndex_A = 11,
	CardIndex_2 = 12,
	CardIndex_SmallJoker = 13,
	CardIndex_LargeJoker = 14,
};
const int CardIndex_JokerBoom = CardIndex_SmallJoker;
extern std::string CardNameTable[CARD_VALUE_LEN];
namespace ECardStyle {
	enum ECardStyle_ {
		Invalid = 0,
		Single = 1,
		Double = 2,
		Triple_Zero = 3,
		Triple_One = 31,
		Triple_Two = 32,
		Boom = 4,
		Quad_Two = 42,
		Single_Chain = 5,
		Double_Chain = 22,
		Triple_Chain_Zero = 230,
		Triple_Chain_One = 231,
		Triple_Chain_Two = 232,
	};
}