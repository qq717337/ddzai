#pragma once
#include "SplitStrategy.h"
#include "PlayStrategyBase.h"

//是否可以接牌
//有牌可接，但是需要判断要不要接，等各种if判断拆分成Chain of Responsibility
class PlayStrategyHandle
{
	PlayStrategyHandle*m_nextStrategyHandler;
public:
	PlayStrategyHandle(const PlayStrategyHandle&) = delete;
	PlayStrategyHandle()=default;
	//传入的Strategy可能有很多子类，可以进行适当的类型转换

	virtual bool Handle(PlayStrategyBase* playStrategy, SplitStrategy * splitStrategy, CardStyle & result) = 0;
	virtual ~PlayStrategyHandle();
	inline void setNextStrategy(PlayStrategyHandle* nextStrategy) {
		m_nextStrategyHandler = nextStrategy;
	}
};
