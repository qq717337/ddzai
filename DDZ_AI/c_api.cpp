#include "stdafx.h"
#include "c_api.h"
#include "GameTable.h"

APP_C_EXPORT uint8_t RobotPlay( uint8_t* ret, uint8_t len0, uint8_t len1, uint8_t len2, uint8_t * playCard0, uint8_t *playCard1, uint8_t *playCard2, uint8_t lastPlayCardLen, uint8_t* lastPlayCards, uint8_t lastPlayIdentity, uint8_t curPlayIdentity)
{
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

	auto thisStyle = table.Take(EIdentity::EIdentity_(curPlayIdentity), EIdentity::EIdentity_(lastPlayIdentity), lastPlayStyle);
	auto cards = table.GetHandCard(EIdentity::EIdentity_(curPlayIdentity))->GetCardsByStyle(thisStyle);
	retLen = cards.size();
	for (size_t i = 0; i < cards.size(); i++) {
		ret[i] = cards[i];
	}
	return retLen;
}

