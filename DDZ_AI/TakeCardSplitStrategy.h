#ifndef _TAKECARDSPLITSTRATEGY_H_
#define _TAKECARDSPLITSTRATEGY_H_
#include "SplitStrategy.h"
//��������������֣�һ��ʼ������С��֣�Ȼ��������Ļ����Ͻ��н�һ���Ĳ��

class TakeCardSplitStrategy :
	public SplitStrategy
{
public:
	TakeCardSplitStrategy(std::shared_ptr<HandCards> cards,const CardStyle& style);
	~TakeCardSplitStrategy();
};

#endif
