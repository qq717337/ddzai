#pragma once
#include "SplitStrategy.h"
//���Ƚ�ĳ��Style�Ĳ�ֲ���,�����Լ������жϻ᲻�ᱻ�����˽�ס�������Լ���С����޷�����ʱ��ǿ�ƽ��Ʋ�ȡ�Ĳ�ֲ���
class PriorityTakeSplitStrategy :
	public SplitStrategy
{
public:
	PriorityTakeSplitStrategy(std::shared_ptr<HandCards> cards, const CardStyle& takeStyle);
	~PriorityTakeSplitStrategy();
	virtual void Split()override;
	virtual void OptimiumTake(const CardStyle & style)override;
	virtual void AvailableTake(const CardStyle & style)override;
	bool CanTake();
};

