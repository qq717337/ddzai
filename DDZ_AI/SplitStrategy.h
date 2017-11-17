#pragma once
#include "SplitStrategyBase.h"
#include "HandleResult.h"
#include <functional>
//每一个继承的子类负责额外的Split信息，例如在地主出牌且自己的牌比较多的时候，
//采取MinStepSplitStrategy ,接牌的时候采用另外的拆分策略，需要把需要接的牌给传进去
class SplitStrategy :
	public SplitStrategyBase
{
protected:
	std::vector<SplitType> m_splitTypeVector;
	std::vector<size_t> m_splitMinStepVector;
	std::vector<HandleResult> m_optimiumStyle;
	//经过拆分后留下的BoomStyle
	std::vector<HandleResult> m_optimiumBoomStyle;
	std::vector<HandleResult> m_availableStyle;
	void sortSplitType();
public:
	virtual void Reset();
	const SplitType& MinStepSplit()const;
	virtual void Split();
	virtual void OptimiumTake(const CardStyle & style);
	virtual void AvailableTake(const CardStyle & style);
	std::vector<const SplitType* > GetSplitType(int moreStepThanMinStep);
	inline const std::vector<HandleResult>& GetOptimiumStyle() const { return m_optimiumStyle; }
	inline const std::vector<HandleResult>& GetOptimiumBoomStyle() const { return m_optimiumBoomStyle; }
	inline const std::vector<HandleResult>& GetAvailableStyle() const { return m_availableStyle; }
	SplitStrategy(std::shared_ptr< HandCards>cards);
	virtual ~SplitStrategy();
};