#pragma once
#include "SplitStrategyBase.h"
#include "HandleResult.h"
#include <functional>
//ÿһ���̳е����ฺ������Split��Ϣ�������ڵ����������Լ����ƱȽ϶��ʱ��
//��ȡMinStepSplitStrategy ,���Ƶ�ʱ���������Ĳ�ֲ��ԣ���Ҫ����Ҫ�ӵ��Ƹ�����ȥ
class SplitStrategy :
	public SplitStrategyBase
{
protected:
	std::vector<SplitType> m_splitTypeVector;
	std::vector<size_t> m_splitMinStepVector;
	std::vector<HandleResult> m_optimiumStyle;
	//������ֺ����µ�BoomStyle
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