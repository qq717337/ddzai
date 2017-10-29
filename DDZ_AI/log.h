#pragma once
#include <cstdio>
#include<cstdlib>
#include <cstdarg>
#include<cstring>

#ifndef		CHECK
#define CHECK(condition)								\
	if(!(condition))Log::Fatal("Check failed: " #condition \
		" at %s, line %d ,\n",__FILE__,__LINE__);
#endif

#ifndef CHECK_NOTNULL
#define CHECK_NOTNULL (pointer)\
	if ((pointer) == nullptr) Log::Fatal( #pointer " Can't be NULL");
#endif 
enum class LogLevel :int64_t {
	Fatal = -1,
	Warning = 0,
	Info = 1,
	Debug = 2,
};

class Log {
private:
	static void Write(LogLevel level, const char* level_str, const char* format, va_list val) {
		if (level <= GetLevel()) {
			printf("[Dolores] [%s] ", level_str);
			vprintf(format, val);
			printf("\n");
			fflush(stdout);
		}
	}
	// a trick to use static variable in header file. 
	// May be not good, but avoid to use an additional cpp file
	static LogLevel& GetLevel() { static LogLevel level = LogLevel::Debug; return level; }

public:
	static void ResetLogLevel(LogLevel level) {
		GetLevel() = level;
	}

	static void Fatal(const char* format, ...) {
		va_list val;
		va_start(val, format);
		fprintf(stderr, "[Dolores] [Fatal] ");
		vfprintf(stderr, format, val);
		fprintf(stderr, "\n");
		fflush(stderr);
		va_end(val);
		exit(1);
	}

	static void Debug(const char* format, ...) {
		va_list val;
		va_start(val, format);
		Write(LogLevel::Debug, "Debug", format, val);
		va_end(val);
	}

	static void Info(const char *format, ...) {
		va_list val;
		va_start(val, format);
		Write(LogLevel::Info, "Info", format, val);
		va_end(val);
	}

	static void Warning(const char *format, ...) {
		va_list val;
		va_start(val, format);
		Write(LogLevel::Warning, "Warning", format, val);
		va_end(val);
	}
};