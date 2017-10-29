#pragma once
#include <vector>
#include "Range.h"
#include "CardStyle.h"
class SplitMemnto;

class SplitType
{
	friend class SplitMemnto;//����Ϊ��Ԫ���Ա��������Է������е�SplitType�ĳ�Ա

#define INSERT_SPLITTYPE(x)	x.push_back(cardIndex);\
	std::sort(x.begin(), x.end())
#define INSERT_CHAIN_SPLITTYPE(x) x.push_back(CardRange(startIndex, endIndex))
#define VECTOR_COPY(src,des)	des.resize(src.size());\
	std::copy(src.begin(), src.end(), des.begin())


private:
	std::vector<uint8_t> Boom; //14������ը
	std::vector<uint8_t>Single;
	std::vector<uint8_t>Double;
	std::vector<uint8_t>Triple;
	std::vector<CardRange>SingleChain;  //����5��
	std::vector<CardRange>DoubleChain; //����3��
	std::vector<CardRange>TripleChain;//����2��


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
	size_t MinStepCount(); //��ȡ��ֺ���Ƶ���С���경��
	bool GetLastShotCardStyles(CardStyle* ref);
	CardStyle GetSingleChainStyle();
	CardStyle GetDoubleChainStyle();
	CardStyle GetTripleChainStyle();
	CardStyle GetTripleStyle();
	SplitType();
	~SplitType();
};

