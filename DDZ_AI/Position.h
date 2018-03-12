#pragma once
#include "SplitType.h"
#include <memory>
class Position
{
public:
	Position();
	std::vector <uint8_t> cards;
	std::unique_ptr<SplitType>split;
	~Position();
};

