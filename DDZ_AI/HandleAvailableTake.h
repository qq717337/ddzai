#ifndef _HANDLEAVAILABLETAKE_H_
#define _HANDLEAVAILABLETAKE_H_
#include "PlayStrategyHandle.h"
class HandleAvailableTake :
	public PlayStrategyHandle
{
public:
	HandleAvailableTake()=default;
	~HandleAvailableTake();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleAvailableTake(const HandleAvailableTake&) = delete;
};

#endif
