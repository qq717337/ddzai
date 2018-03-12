#ifndef _HANDLEAVOIDOTHERWIN_H_
#define _HANDLEAVOIDOTHERWIN_H_
#include "PlayStrategyHandle.h"
class HandleAvoidOtherWinPlay :
	public PlayStrategyHandle
{
public:
	HandleAvoidOtherWinPlay() = default;
	~HandleAvoidOtherWinPlay();
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleAvoidOtherWinPlay(const HandleAvoidOtherWinPlay&) = delete;
};
#endif
