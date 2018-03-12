#ifndef _HANDLETWOSTEPPLAY_H_
#define _HANDLETWOSTEPPLAY_H_
#include "PlayStrategyHandle.h"
class HandleTwoStepPlay :
	public PlayStrategyHandle
{
public:
	HandleTwoStepPlay()=default;
	~HandleTwoStepPlay();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleTwoStepPlay(const HandleTwoStepPlay&) = delete;
};

#endif
