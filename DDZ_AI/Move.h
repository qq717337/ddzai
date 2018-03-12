#pragma once
#include "CardStyle.h"
#include "CardConst.h"
struct Move
{
	std::vector<uint8_t> cards;
	CardStyle style;
	EIdentity::EIdentity_ sideToMove;
	Move();
	Move(std::vector<uint8_t> inputCard, CardStyle inputStyle);

	bool isPass();
	~Move();
};

