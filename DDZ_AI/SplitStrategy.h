#pragma once
#include "SplitStrategyBase.h"
#include <functional>
//ÿһ���̳е����ฺ������Split��Ϣ�������ڵ����������Լ����ƱȽ϶��ʱ��
//��ȡMinStepSplitStrategy ,���Ƶ�ʱ���������Ĳ�ֲ��ԣ���Ҫ����Ҫ�ӵ��Ƹ�����ȥ
class SplitStrategy :
	public SplitStrategyBase
{
protected:
	std::vector<SplitType> m_splitTypeVector;
	std::vector<size_t> m_splitMinStepVector;
public:
	const SplitType& MinStepSplit()const;
	virtual void Split();
	virtual std::vector<CardStyle> OptimizeTake(const CardStyle & style);
	SplitStrategy(std::shared_ptr< HandCards>cards);
	virtual ~SplitStrategy();
};