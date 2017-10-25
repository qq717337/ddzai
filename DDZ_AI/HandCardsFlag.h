#pragma once

#include "CardConst.h"
#include"stdafx.h"
#include <vector>
#include<string>
class HandCardsFlag
{
protected:
	uint8_t** Flags;
public:
	HandCardsFlag();
	HandCardsFlag(bool);//显示不进行任何的创建
	uint8_t	GetFlag(uint8_t color, uint8_t index);
	~HandCardsFlag();
	static uint8_t  CardValueToIndex(uint8_t cardValue);
	static uint8_t  CardColorIndexToValue(uint8_t color, uint8_t index);
	void RemoveChain(uint8_t startIndex, uint8_t endIndex, uint8_t count);
	void  RemoveIndex(uint8_t index, uint8_t count);
	virtual size_t Size();
	std::string FlagString();
};

