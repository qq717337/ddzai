#pragma once
#include <vector>
#include <memory>
#include "StateInfo.h"
class TableHistory
{
public:
	TableHistory();
	~TableHistory();
	std::vector<std::unique_ptr<StateInfo>> states;
	
};

