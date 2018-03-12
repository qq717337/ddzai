#include "stdafx.h"
#include "Move.h"


Move::Move()
{
}

Move::Move(std::vector<uint8_t> inputCard, CardStyle inputStyle)
{
	cards = std::move(inputCard);
	style = std::move(inputStyle);
}

bool Move::isPass()
{
	return cards.empty();
}


Move::~Move()
{
}
