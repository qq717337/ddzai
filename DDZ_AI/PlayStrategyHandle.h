#pragma once
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"

//�Ƿ���Խ���
//���ƿɽӣ�������Ҫ�ж�Ҫ��Ҫ�ӣ��ȸ���if�жϲ�ֳ�Chain of Responsibility
class PlayStrategyHandle
{
	PlayStrategyHandle*m_nextStrategyHandler;
public:
	PlayStrategyHandle(const PlayStrategyHandle&) = delete;
	PlayStrategyHandle()=default;
	//�����Strategy�����кܶ����࣬���Խ����ʵ�������ת��

	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result) = 0;
	virtual ~PlayStrategyHandle();
	inline void setNextStrategy(PlayStrategyHandle* nextStrategy) {
		m_nextStrategyHandler = nextStrategy;
	}
};
