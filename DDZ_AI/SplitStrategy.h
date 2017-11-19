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
	bool m_hasSplit;
	bool m_hasGetOptimium;
	bool m_hasGetAvailable;
public:
	virtual void Reset();
	const SplitType& MinStepSplit()const;
	virtual void Split();
	virtual void OptimiumTake(const CardStyle & style);
	virtual void AvailableTake(const CardStyle & style);
	std::vector<const SplitType* > GetSplitType(int moreStepThanMinStep);
	inline const std::vector<HandleResult>& GetOptimiumStyle() const { _ASSERT(m_hasGetOptimium); return m_optimiumStyle; }
	inline const std::vector<HandleResult>& GetOptimiumBoomStyle() const { _ASSERT(m_hasGetOptimium);  return m_optimiumBoomStyle; }
	inline const std::vector<HandleResult>& GetAvailableStyle() const { _ASSERT(m_hasGetAvailable);  return m_availableStyle; }
	inline bool HasSplit() { return m_hasSplit; }
	inline bool HasAvailable() { return m_hasGetAvailable; }
	inline bool HasOptimium() { return m_hasGetOptimium; }
	SplitStrategy(std::shared_ptr< HandCards>cards);
	virtual ~SplitStrategy();
};