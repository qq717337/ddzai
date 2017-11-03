#pragma once
#include"CardSet.h"

class DecorateDealStrategy
{
protected:
	CardSet* _Card;
public:
	virtual void PreDeal() = 0;
	DecorateDealStrategy(CardSet* card);
	virtual ~DecorateDealStrategy();
};
class MultiBoomDealStrategy :
	public DecorateDealStrategy {
private:
	int	BoomCount;
public:
	virtual void PreDeal() override;
	MultiBoomDealStrategy(CardSet* card, int boomCount);
	virtual ~MultiBoomDealStrategy();
};

class SuperiorDealStrategy :
	public DecorateDealStrategy {
private:
	int	PlayerId;
public:
	virtual void PreDeal() override;
	SuperiorDealStrategy(CardSet* card, int playerId);
	virtual ~SuperiorDealStrategy();
};
