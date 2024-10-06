
#ifndef __hive__log__
#define __hive__log__

#include <cstdint>
#include <string>
#include <map>

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_ERROR 2
#define LOG_BUFFER_SIZE 8192

typedef bool (*WriteRemoteLogFunction)(const char* fileName, const char* str);
typedef void (*ThreadHandleFunction)(void);

extern void setMaxLogSize(int logSize);
extern int getMaxLogSize(void);
extern void setLogLevel(int level);
extern int getLogLevel(void);
extern const char* getTimeString(void);
extern const char* getTimeStringUS(void);
extern const char* getTimeStringUSFile(void);

extern char* getLogBuffer(void);
extern bool openCppLog(const char* fileName, bool useCache, bool useThread, ThreadHandleFunction startFunc=NULL, ThreadHandleFunction updateFunc=NULL);
extern void writeCppLog(const char* str);
extern const char* getCppLogFileName(void);

extern void dumpCacheLog(std::map<std::string, std::string> &logMap);
extern void writeRemoteLog(const char* fileName, const char* str);
extern void setWriteRemoteLogFunction(WriteRemoteLogFunction function);

#define LOG_DEBUG(fmt, args...)\
if(getLogLevel()<=LOG_LEVEL_DEBUG){\
	snprintf(getLogBuffer(), LOG_BUFFER_SIZE, "[DEBUG %s][%s:%s():%d] " fmt "\n", getTimeStringUS(), __FILE__, __FUNCTION__, __LINE__, ##args);\
	writeCppLog(getLogBuffer());\
}

#define LOG_INFO(fmt, args...)\
if(getLogLevel()<=LOG_LEVEL_INFO){\
	snprintf(getLogBuffer(), LOG_BUFFER_SIZE, "[INFO  %s][%s:%s():%d] " fmt "\n", getTimeStringUS(), __FILE__, __FUNCTION__, __LINE__, ##args);\
	writeCppLog(getLogBuffer());\
}

#define LOG_ERROR(fmt, ...)\
if(getLogLevel()<=LOG_LEVEL_ERROR){\
	snprintf(getLogBuffer(), LOG_BUFFER_SIZE, "[ERROR %s][%s:%s():%d] " fmt "\n", getTimeStringUS(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	writeCppLog(getLogBuffer());\
}

#define log_debug LOG_DEBUG
#define log_info LOG_INFO
#define log_error LOG_ERROR

#endif
