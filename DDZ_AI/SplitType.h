#pragma once
#include <vector>
#include <functional>
#include "Range.h"
#include "CardStyle.h"
//组合的不同拆分方法不建立不同的类了，而是在一个类中完成不同的组装策略，
//例如最优保留炸弹等的，或者最小步数的，或者为了接某一张牌，但是最优的没有，必须拆分对子这种情况，
class SplitMemnto;

class SplitType
{
	friend class SplitMemnto;//声明为友元类以便这个类可以访问所有的SplitType的成员
#define INSERT_SPLITTYPE(x)	x.push_back(cardIndex);\
	std::sort(x.begin(), x.end())
#define INSERT_CHAIN_SPLITTYPE(x) x.push_back(CardRange(startIndex, endIndex))


private:
	CardVector Boom;
	CardVector Single;
	CardVector Double;
	CardVector Triple;
	std::vector<CardRange>SingleChain;  //至少5张
	std::vector<CardRange>DoubleChain; //至少3张
	std::vector<CardRange>TripleChain;//至少2张


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
	size_t MinStepCount(bool exceptBoom = false)const; //获取拆分后的牌的最小出完步数
	size_t CardCount()const;
	bool GetLastShotCardStyle(CardStyle* ref)const;
	CardStyle GetSingleChainStyle()const;
	CardStyle GetDoubleChainStyle()const;
	CardStyle GetTripleChainStyle()const;
	CardStyle GetTripleStyle()const;
	std::vector<CardStyle>GetAllSplitStyle()const;
	//从SingleChain中获取一个单只,forceSplit=true则强制返回一个即使不满足最优拆解
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

