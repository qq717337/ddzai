#pragma once
#include"CardStyle.h"
class HandleResult
{
public:
	inline const CardStyle&  Style()const { return m_resultStyle; }
	HandleResult() = default;
	HandleResult(const CardStyle& style);
	HandleResult& AddBonusStep(int step);
	HandleResult& OtherTakeThisBonusStep(int step);
	HandleResult& OtherTakeLeftStep(int step);
	
private:
	//���ӵĲ���
	int m_selfBonusStep;
	//������Ҫ������������ɵĲ�������,0����������ס������Ӳ�ס����û�����vector
	std::vector<int> m_otherTakeThisBonusStep;
	//�����˽������֮����Ҫ���ٲ��ſ���Ӯ
	std::vector<int> m_otherTakeLeftStep;

	//�м�����ҿ��Խ������
	const size_t OtherCanTakeCount()const {
		return m_otherTakeThisBonusStep.size();
	}
	CardStyle m_resultStyle;
};

