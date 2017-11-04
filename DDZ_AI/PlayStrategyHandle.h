#pragma once
#include "SplitStrategy.h"


//是否可以接牌
//有牌可接，但是需要判断要不要接，等各种if判断拆分成Chain of Responsibility
class PlayStrategyHandle
{
	PlayStrategyHandle*m_nextStrategyHandler;
public:
	PlayStrategyHandle(const PlayStrategyHandle&) = delete;
	PlayStrategyHandle();
	//传入的Strategy可能有很多子类，可以进行适当的类型转换

	virtual bool Handle(SplitStrategy * strategy, CardStyle & result) = 0;
	virtual ~PlayStrategyHandle();
	inline void setNextStrategy(PlayStrategyHandle* nextStrategy) {
		m_nextStrategyHandler = nextStrategy;
	}
};

//处理是否可以接牌
class HandleCanTakeCard :public PlayStrategyHandle {
private:
	bool CanTake(SplitStrategy* strategy);
public:
	HandleCanTakeCard();
	virtual bool Handle(SplitStrategy * strategy, CardStyle & result)override;
	HandleCanTakeCard(const HandleCanTakeCard&) = delete;
};

//处理是否存在最优接牌
class HandleCanOptimiumTakeCard :public PlayStrategyHandle {
private:
	bool CanOptimiumTake(SplitStrategy* strategy);
public:
	HandleCanOptimiumTakeCard()=default;
	virtual bool Handle(SplitStrategy * strategy, CardStyle & result)override;
	HandleCanOptimiumTakeCard(const HandleCanOptimiumTakeCard&) = delete;
};
