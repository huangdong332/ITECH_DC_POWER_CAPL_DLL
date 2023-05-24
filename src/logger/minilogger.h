#ifndef MYLOG_H
#define MYLOG_H
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define LOG_INFO(...) FileLogger("I",__VA_ARGS__)
#define LOG_DEBUG(...) FileLogger("D",__VA_ARGS__)
#define LOG_ERROR(...) FileLogger("E",__VA_ARGS__)
#define LOG_TRACE(...) FileLogger("T",__VA_ARGS__)
#define LOG_WARN(...) FileLogger("W",__VA_ARGS__)
#define LOG_FATAL(...) FileLogger("F",__VA_ARGS__)

#define log_info(...) Logger("I",__VA_ARGS__)
#define log_debug(...) Logger("D",__VA_ARGS__)
#define log_error(...) Logger("E",__VA_ARGS__)
#define log_trace(...) Logger("T",__VA_ARGS__)
#define log_warn(...) Logger("W",__VA_ARGS__)
#define log_fatal(...) Logger("F",__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif
void FileLoggerInit(const char *fileName);
void Logger(const char* tag, const char* messages,...);
void FileLogger(const char* tag, const char* message,...);
#ifdef __cplusplus
}
#endif
#endif