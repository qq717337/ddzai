#include "stdafx.h"
#include "c_api.h"
#include "GameTable.h"
#include "SmoothCard.h"
#include<algorithm>

APP_C_EXPORT void DealSmoothCard(uint8_t * playCard0, uint8_t * playCard1, uint8_t * playCard2, uint8_t * deskCard)
{
	srand((unsigned)time(NULL));
	SmoothCard *opt = new SmoothCard();
	opt->Optimized(nullptr, 3, 3);
	
	auto c0=opt->PlayerCardSet[0]->ToCardValues();
	std::memcpy( playCard0,c0.begin()._Ptr, c0.size());
	auto c1 = opt->PlayerCardSet[1]->ToCardValues();
	std::memcpy(playCard1,c1.begin()._Ptr, c1.size());
	auto c2 = opt->PlayerCardSet[2]->ToCardValues();
	std::memcpy( playCard2,c2.begin()._Ptr, c2.size());
	auto& d = opt->ExtraCard.ToCardValues();
	std::memcpy( deskCard,d.begin()._Ptr, d.size());
}
//如果出现异常则返回-1
APP_C_EXPORT uint8_t RobotPlay(uint8_t* ret, uint8_t len0, uint8_t len1, uint8_t len2, uint8_t * playCard0, uint8_t *playCard1, uint8_t *playCard2, uint8_t lastPlayCardLen, uint8_t* lastPlayCards, uint8_t lastPlayIdentity, uint8_t curPlayIdentity)
{
	API_BEGIN()
	uint8_t retLen = 255;
	CardSet* opt = new CardSet();
	opt->PlayerCardSet[0] = new HandCards(CardVector(playCard0, playCard0 + len0));
	opt->PlayerCardSet[1] = new HandCards(CardVector(playCard1, playCard1 + len1));
	opt->PlayerCardSet[2] = new HandCards(CardVector(playCard2, playCard2 + len2));

	//auto& extraCards = set.RandomFillLeft();

	GameTable table(*opt);
	CardVector lastPlayCard(lastPlayCards, lastPlayCards + lastPlayCardLen);
	//CardStyle lastPlayStyle = CardStyle::SingleChainStyle(CardIndex_3, CardIndex_7);
	CardStyle lastPlayStyle = CardStyle::FromCardsValue(lastPlayCard);
	CardStyle thisStyle;
	if (curPlayIdentity == lastPlayIdentity) {
		thisStyle = table.Play(EIdentity::EIdentity_(curPlayIdentity));
	}
	else {
		thisStyle = table.Take(EIdentity::EIdentity_(curPlayIdentity), EIdentity::EIdentity_(lastPlayIdentity), lastPlayStyle);
	}
	auto cards = table.GetHandCard(EIdentity::EIdentity_(curPlayIdentity))->GetCardsByStyle(thisStyle);
	retLen = cards.size();
	std::memcpy(ret,cards.begin()._Ptr, cards.size());
	return retLen;
	API_END()
}

