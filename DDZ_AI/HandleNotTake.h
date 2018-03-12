#ifndef _HANDLENOTTAKE_H_
#define _HANDLENOTTAKE_H_
#include "PlayStrategyHandle.h"
class HandleNotTake :
	public PlayStrategyHandle
{
public:
	HandleNotTake()=delete;
	~HandleNotTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleNotTake(const HandleNotTake&) = delete;
};

#endif
