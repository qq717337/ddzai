#ifndef _LOG_H_
#define _LOG_H_
#include <cstdio>
#include<cstdlib>
#include <cstdarg>
#include<cstring>
#ifdef USE_LOG4CPP
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#endif

#ifndef		CHECK
#define CHECK(condition)								\
    if(!(condition))Log::Fatal("Check failed: " #condition \
        " at %s, line %d ,\n",__FILE__,__LINE__);
#endif

#ifndef CHECK_NOTNULL
#define CHECK_NOTNULL (pointer)\
    if ((pointer) == nullptr) Log::Fatal( #pointer " Can't be NULL");
#endif
#ifndef LOGLEVEL_H
#define LOGLEVEL_H
enum class LogLevel :int64_t {
    Fatal = -1,
    Warning = 0,
    Info = 1,
    Debug = 2,
};
#endif
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
    //static getAppender
    // static log4cpp::Appender* appender = new log4cpp::FileAppender("default", "D:\\CommondCode\\DDZ_AI\Log\\1.log");
public:

#ifdef USE_LOG4CPP
    static void InfoF(const std::string& s) {
        static log4cpp::Appender* syslogAppender = new log4cpp::OstreamAppender("syslogdummy", &std::cout);
        static log4cpp::Appender* appender = new log4cpp::FileAppender("default", "D:\\CommondCode\\DDZ_AI\\Log\\1.log");
        auto& root=log4cpp::Category::getRoot();
        if (root.getAllAppenders().size() == 0) {
            // root.addAppender(syslogAppender);
            root.addAppender(appender);
        }
        root.info(s);
    }
#endif

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
#endif