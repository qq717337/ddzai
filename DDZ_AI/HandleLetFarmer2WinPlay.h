#ifndef _HANDLELETFARMER2WINPLAY_H_
#define _HANDLELETFARMER2WINPLAY_H_
#include "PlayStrategyHandle.h"
//Farmer1在检测到Farmer2可以胜利的情况下打出让Farmer2胜利的牌
class HandleLetFarmer2WinPlay :
	public PlayStrategyHandle
{
public:
	HandleLetFarmer2WinPlay() = default;
	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result)override;
	HandleLetFarmer2WinPlay(const HandleLetFarmer2WinPlay&) = delete;
};

#endif
