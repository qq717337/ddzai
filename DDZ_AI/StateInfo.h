#pragma once
#include "CardConst.h"
#include "Move.h"
#include "HandCards.h"
#include "Position.h"
#include <memory>
class StateInfo
{
public:
	StateInfo();
	~StateInfo();
	StateInfo* previous;
	Position* getCurPosition();
private:
	//当前节点处的玩家身份
	EIdentity::EIdentity_ sideToMove;
	std::unique_ptr<Position> handCards[3];
	Move move;
};

