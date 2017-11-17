#pragma once
#include <vector>
#include <functional>
#include "Range.h"
#include "CardStyle.h"
//��ϵĲ�ͬ��ַ�����������ͬ�����ˣ�������һ��������ɲ�ͬ����װ���ԣ�
//�������ű���ը���ȵģ�������С�����ģ�����Ϊ�˽�ĳһ���ƣ��������ŵ�û�У������ֶ������������
class SplitMemnto;

class SplitType
{
	friend class SplitMemnto;//����Ϊ��Ԫ���Ա��������Է������е�SplitType�ĳ�Ա
#define INSERT_SPLITTYPE(x)	x.push_back(cardIndex);\
	std::sort(x.begin(), x.end())
#define INSERT_CHAIN_SPLITTYPE(x) x.push_back(CardRange(startIndex, endIndex))


private:
	CardVector Boom;
	CardVector Single;
	CardVector Double;
	CardVector Triple;
	std::vector<CardRange>SingleChain;  //����5��
	std::vector<CardRange>DoubleChain; //����3��
	std::vector<CardRange>TripleChain;//����2��


public:
	std::string ToString();
	void AddBoom(uint8_t cardIndex);
	void AddSingle(uint8_t cardIndex);
	void AddDouble(uint8_t cardIndex);
	void AddTriple(uint8_t cardIndex);
	void AddSingleChain(uint8_t startIndex, uint8_t endIndex);
	void AddDoubleChain(uint8_t startIndex, uint8_t endIndex);
	void AddTripleChain(uint8_t startIndex, uint8_t endIndex);

	CardStyle MinValueCardStyle()const;
	size_t MinStepCount(bool exceptBoom = false)const; //��ȡ��ֺ���Ƶ���С���경��
	size_t CardCount()const;
	bool GetLastShotCardStyle(CardStyle* ref)const;
	CardStyle GetSingleChainStyle()const;
	CardStyle GetDoubleChainStyle()const;
	CardStyle GetTripleChainStyle()const;
	CardStyle GetTripleStyle()const;
	std::vector<CardStyle>GetAllSplitStyle()const;
	//��SingleChain�л�ȡһ����ֻ,forceSplit=true��ǿ�Ʒ���һ����ʹ���������Ų��
	bool RequireSingleFromChain(size_t requireCount, CardVector  & out, bool forceSplit = false)const;
	bool RequireDoubleFromChain(size_t requireCount, CardVector  & out, bool forceSplit = false)const;
	bool RequireFromAll(size_t requireCount, CardVector  & outSingleIndex, CardVector  & outDoubleIndex, CardVector  & outTripleIndex,
		std::function<void(CardVector  &, CardVector  &, CardVector  &)> redefineFunc);
	virtual void Reset();
	SplitType();
	~SplitType();
	SplitType&operator=(const SplitType& rhs);
	inline bool SplitType::operator==(const SplitType & rhs) const
	{
		return(rhs.Boom == Boom && rhs.Single == Single && rhs.Double == Double && rhs.Triple == Triple &&
			rhs.SingleChain == SingleChain && rhs.DoubleChain == DoubleChain && rhs.TripleChain == TripleChain);
	}

	inline bool SplitType::operator!=(const SplitType & rhs) const
	{
		return(rhs.Boom != Boom || rhs.Single != Single || rhs.Double != Double || rhs.Triple != Triple ||
			rhs.SingleChain != SingleChain || rhs.DoubleChain != DoubleChain || rhs.TripleChain != TripleChain);
	}
	inline bool InSingle(uint8_t cardIndex)const { return std::find(Single.begin(), Single.end(), cardIndex) != Single.end(); }
	inline bool InDouble(uint8_t cardIndex)const { return std::find(Double.begin(), Double.end(), cardIndex) != Double.end(); }
	inline const bool HasJokerBoom()const { return std::find(Boom.begin(), Boom.end(), CardIndex_JokerBoom) != Single.end(); }

	inline const CardVector & GetBoom()const { return Boom; }
	inline const CardVector & GetSingle()const { return Single; }
	inline const CardVector & GetDouble() const { return Double; }
	inline const CardVector & GetTriple()const { return Triple; }
	inline const std::vector<CardRange>& GetSingleChain() const { return SingleChain; }
	inline const std::vector<CardRange>& GetDoubleChain() const { return DoubleChain; }
	inline const std::vector<CardRange>& GetTripleChain() const { return TripleChain; }

	inline static const CardStyle* MaxEndValueStyle(const std::vector<CardStyle> inStyles) {
		int maxStyle = 0;
		return std::max_element(inStyles.cbegin(), inStyles.cend(), [](CardStyle x, CardStyle y) {
			return x.EndValue < y.EndValue;
		})._Ptr;
	}
};

