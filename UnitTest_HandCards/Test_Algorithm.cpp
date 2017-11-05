#include "stdafx.h"
#include "../DDZ_AI/common_algorithm.h"

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_Split
{
	TEST_CLASS(UnitTest2)
	{
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			srand((unsigned)time(NULL));
			Logger::WriteMessage("Test initialized.\n"); //用于输出信息  
		}
		TEST_METHOD_CLEANUP(TearDown)
		{
			Logger::WriteMessage("Test completed.\n");
		}

		TEST_METHOD(TestPerm)
		{
			int b[10];
			perm(10, b);
			std::vector<uint8_t>a;
			perm<uint8_t>(10, a);
		}
		TEST_METHOD(TestLog4cpp)
		{
			log4cpp::Appender* syslogAppender;

			syslogAppender = new log4cpp::OstreamAppender("syslogdummy", &std::cout); 
			log4cpp::Appender* appender = new log4cpp::FileAppender("default", "D:\\CommondCode\\DDZ_AI\\Log\\1.log");	
			log4cpp::Category& root = log4cpp::Category::getRoot();
			root.addAppender(syslogAppender);
			root.addAppender(appender);
			root.info("hello");
		}
	};
}