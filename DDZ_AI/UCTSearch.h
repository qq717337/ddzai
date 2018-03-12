#pragma once
#include "TableHistory.h"
#include "UCTNode.h"
class UCTSearch
{
public:  
	static constexpr auto MAX_TREE_SIZE = 40'000'000;
	UCTSearch(TableHistory&& bh);
	~UCTSearch(); 
	//模拟play，返回true代表产生了胜负 ，不在向下执行
	bool play_simulation(TableHistory& bh, UCTNode* const node); 
	//模拟出所有的node，并挑选最好的一条方案执行
	Move think();
};

