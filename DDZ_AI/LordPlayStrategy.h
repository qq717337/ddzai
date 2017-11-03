#pragma once
#include "PlayStrategyBase.h"
#include "PlayStrategyHandle.h"
#include <memory>
class LordPlayStrategy :
	public PlayStrategyBase
{
private:
	std::unique_ptr<PlayStrategyHandle>m_handlerCanTake;
	std::unique_ptr<PlayStrategyHandle>handler02;
	std::unique_ptr<PlayStrategyHandle>handler03;
public:
	virtual CardStyle Play() override;
	virtual CardStyle Take(Identity::EIdentity_ lastIdentity, const CardStyle & lastStyle) override;
	virtual int Identity() override;

	LordPlayStrategy(const std::vector<uint8_t>& cardsValue);
	LordPlayStrategy(const std::set<uint8_t, CardSetCompare>& cardsValue);
	void InitResponsibility();
	~LordPlayStrategy();
};

