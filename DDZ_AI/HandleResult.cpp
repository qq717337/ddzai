#include "stdafx.h"
#include "HandleResult.h"


HandleResult::HandleResult(const CardStyle & style)
{
	m_resultStyle = style;
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

bool HandleResult::operator==(const HandleResult & rhs) const
{
	return m_resultStyle == rhs.m_resultStyle&& m_selfBonusStep == rhs.m_selfBonusStep&&m_otherTakeLeftStep == rhs.m_otherTakeLeftStep&& m_otherTakeThisBonusStep == rhs.m_otherTakeThisBonusStep;
}
