#pragma once
#include <memory>
class UCTNode
{
public: 
	using node_ptr_t = std::unique_ptr<UCTNode>;
	UCTNode();
	~UCTNode();
private:
	bool hasChildren;
	int visits;
	float m_score;
};

