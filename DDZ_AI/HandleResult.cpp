#include "stdafx.h"
#include "HandleResult.h"


HandleResult::HandleResult(const CardStyle & style)
{
}

HandleResult & HandleResult::AddBonusStep(int step)
{
	m_selfBonusStep = step;
	return *this;
}

HandleResult & HandleResult::OtherTakeThisBonusStep(int step)
{
	m_otherTakeThisBonusStep.push_back(step);
	return *this;
}

HandleResult & HandleResult::OtherTakeLeftStep(int step)
{
	m_otherTakeLeftStep.push_back(step);
	return *this;
}
