#pragma once
#include <vector>
#include "Range.h"
#include "CardStyle.h"
class SplitMemnto;

class SplitType
{
	friend class SplitMemnto;//声明为友元类以便这个类可以访问所有的SplitType的成员

#define INSERT_SPLITTYPE(x)	x.push_back(cardIndex);\
	std::sort(x.begin(), x.end())
#define INSERT_CHAIN_SPLITTYPE(x) x.push_back(CardRange(startIndex, endIndex))
#define VECTOR_COPY(src,des)	des.resize(src.size());\
	std::copy(src.begin(), src.end(), des.begin())


private:
	std::vector<uint8_t> Boom; //14代表王炸
	std::vector<uint8_t>Single;
	std::vector<uint8_t>Double;
	std::vector<uint8_t>Triple;
	std::vector<CardRange>SingleChain;  //至少5张
	std::vector<CardRange>DoubleChain; //至少3张
	std::vector<CardRange>TripleChain;//至少2张


public:
	SplitType&operator=(const SplitType& rhs);

	void AddBoom(uint8_t cardIndex);
	void AddSingle(uint8_t cardIndex);
	void AddDouble(uint8_t cardIndex);
	void AddTriple(uint8_t cardIndex);
	void AddSingleChain(uint8_t startIndex, uint8_t endIndex);
	void AddDoubleChain(uint8_t startIndex, uint8_t endIndex);
	void AddTripleChain(uint8_t startIndex, uint8_t endIndex);

	CardStyle MinValueCardStyle();
	size_t MinStepCount(); //获取拆分后的牌的最小出完步数
	bool GetLastShotCardStyles(CardStyle* ref);
	CardStyle GetSingleChainStyle();
	CardStyle GetDoubleChainStyle();
	CardStyle GetTripleChainStyle();
	CardStyle GetTripleStyle();
	SplitType();
	~SplitType();
};

