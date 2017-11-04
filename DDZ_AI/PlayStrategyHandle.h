#pragma once
#include "SplitStrategy.h"


//�Ƿ���Խ���
//���ƿɽӣ�������Ҫ�ж�Ҫ��Ҫ�ӣ��ȸ���if�жϲ�ֳ�Chain of Responsibility
class PlayStrategyHandle
{
	PlayStrategyHandle*m_nextStrategyHandler;
public:
	PlayStrategyHandle(const PlayStrategyHandle&) = delete;
	PlayStrategyHandle();
	//�����Strategy�����кܶ����࣬���Խ����ʵ�������ת��

	virtual bool Handle(SplitStrategy * strategy, CardStyle & result) = 0;
	virtual ~PlayStrategyHandle();
	inline void setNextStrategy(PlayStrategyHandle* nextStrategy) {
		m_nextStrategyHandler = nextStrategy;
	}
};

//�����Ƿ���Խ���
class HandleCanTakeCard :public PlayStrategyHandle {
private:
	bool CanTake(SplitStrategy* strategy);
public:
	HandleCanTakeCard();
	virtual bool Handle(SplitStrategy * strategy, CardStyle & result)override;
	HandleCanTakeCard(const HandleCanTakeCard&) = delete;
};

//�����Ƿ�������Ž���
class HandleCanOptimiumTakeCard :public PlayStrategyHandle {
private:
	bool CanOptimiumTake(SplitStrategy* strategy);
public:
	HandleCanOptimiumTakeCard()=default;
	virtual bool Handle(SplitStrategy * strategy, CardStyle & result)override;
	HandleCanOptimiumTakeCard(const HandleCanOptimiumTakeCard&) = delete;
};
