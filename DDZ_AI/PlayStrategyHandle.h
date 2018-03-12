#ifndef _PLAYSTRATEGYHANDLE_H_
#define _PLAYSTRATEGYHANDLE_H_
#include "CardStyle.h"
class PlayStrategyBase;
class SplitStrategy;
//�Ƿ���Խ���
//���ƿɽӣ�������Ҫ�ж�Ҫ��Ҫ�ӣ��ȸ���if�жϲ�ֳ�Chain of Responsibility
class PlayStrategyHandle
{
public:
	PlayStrategyHandle(const PlayStrategyHandle&) = delete;
	PlayStrategyHandle() = default;
	//�����Strategy�����кܶ����࣬���Խ����ʵ�������ת��

	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result) = 0;
	virtual ~PlayStrategyHandle();
};
#endif
