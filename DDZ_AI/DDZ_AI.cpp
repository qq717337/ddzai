// DDZ_AI.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "OptimizedCard.h"
#include "SmoothCard.h"
#include"Recorder.h"
#include <time.h>
#include <iostream>
#include "OpenCVEntry.h"
using namespace std;

int main()
{
	SmoothCard *opt = new SmoothCard();
	auto multiBoom = new MultiBoomDealStrategy(opt, 1);
	opt->Optimized(nullptr, {0,3,0});
	std::string s = opt->ToString2();//必须要创建变量才可以
	const std::string name = "D:\\UnityProject\\DDZCardImage\\Card.exe " + s;
	system(name.c_str());
	system("pause");
	delete multiBoom;
	delete opt;
    return 0;
}

