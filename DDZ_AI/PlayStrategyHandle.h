#ifndef _PLAYSTRATEGYHANDLE_H_
#define _PLAYSTRATEGYHANDLE_H_
#include "CardStyle.h"
class PlayStrategyBase;
class SplitStrategy;
//是否可以接牌
//有牌可接，但是需要判断要不要接，等各种if判断拆分成Chain of Responsibility
class PlayStrategyHandle
{
public:
	PlayStrategyHandle(const PlayStrategyHandle&) = delete;
	PlayStrategyHandle() = default;
	//传入的Strategy可能有很多子类，可以进行适当的类型转换

	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result) = 0;
	virtual ~PlayStrategyHandle();
};
#endif
