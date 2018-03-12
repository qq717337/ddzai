#pragma once
#include "TableHistory.h"
#include "UCTNode.h"
class UCTSearch
{
public:  
	static constexpr auto MAX_TREE_SIZE = 40'000'000;
	UCTSearch(TableHistory&& bh);
	~UCTSearch(); 
	//ģ��play������true���������ʤ�� ����������ִ��
	bool play_simulation(TableHistory& bh, UCTNode* const node); 
	//ģ������е�node������ѡ��õ�һ������ִ��
	Move think();
};

