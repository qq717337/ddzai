// DDZ_AI.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "OptimizedCard.h"
#include"Recorder.h"
#include <time.h>
#include <iostream>
using namespace std;

int main()
{
	srand((unsigned)time(NULL));

	OptimizedCard *opt = new OptimizedCard();
	auto multiBoom = new MultiBoomDealStrategy(opt, 5);
	opt->Optimized(multiBoom, 3);
	std::string s = opt->ToString2();//必须要创建变量才可以
	const std::string name = "D:\\UnityProject\\DDZCardImage\\Card.exe "+s;
	system(name.c_str());
	system("pause");
	delete multiBoom;
    return 0;
}

