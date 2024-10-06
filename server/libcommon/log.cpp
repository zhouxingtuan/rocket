
#include "log.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <time.h>
#include <sys/time.h>

//thread libs
#include <pthread.h>
#include <semaphore.h>

#include <sys/io.h>
#include <fcntl.h>

#define LOG_MAX_CACHE_SIZE 10485760

class LogData;
void* LogThreadHandleFunction(void* pData);
void writeLogInThread(const std::string& fileName, const std::string& str);
void writeLogToLocalFile(const char* fileName, const char* str);

class LogFile
{
public:
	std::string m_fileName;		// 文件名
	int64_t m_fileLength;			// 文件长度
	int m_fileHandle;			// linux下文件句柄
public:
	LogFile(const std::string& name, const std::string& ext) : m_fileName(name+ext), m_fileLength(0), m_fileHandle(-1) {}
	virtual ~LogFile(void){
		closeReadWrite();
	}
public:
    inline bool openLog(void){
        if( !openReadWrite("ab+") ){
            return false;
        }
        if( !openReadWrite("rb+") ){
            return false;
        }
        fileSeek(0, SEEK_END);
        return true;
    }
	inline bool renameLog(const char* newName){
	    if( rename(m_fileName.c_str(), newName) < 0 ){
	        return false;
	    }
	    return true;
	}
    inline bool writeLog(const char* str, int64_t length){
//        int64_t length = strlen(str);
        if(length != fileWrite(str, 1, length)){
            return false;
        }
        m_fileLength += length;
        return true;
    }
    inline bool readLogAll(std::vector<char>& outVec){
        int64_t offset = 0;
        int64_t length = getLogLength();
        return readLog(offset, length, outVec);
    }
    inline bool readLog(int64_t offset, int64_t length, std::vector<char>& outVec){
        outVec.resize(length);
        int64_t readSize = seekRead(outVec.data(), 1, length, offset, SEEK_SET);
        if(readSize != length){
            outVec.resize(readSize);
            return false;
        }
        return true;
    }
    inline int64_t getLogLength(void) const {
        return m_fileLength;
    }
    inline const std::string& getFileName(void) const{
        return m_fileName;
    }
    // -------------------------
	bool openReadWrite(const char* mode){
		closeReadWrite();
		if(*mode == 'a'){
//		O_RDONLY:以只读方式打开文件
//        O_WRONLY:以只写方式打开文件
//        O_RDWR:以读写方式打开文
//          0666 = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
			m_fileHandle = open(m_fileName.c_str(), O_APPEND|O_CREAT, S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH);   // O_APPEND
		}else{
			m_fileHandle = open(m_fileName.c_str(), O_RDWR);
		}
		if(-1 == m_fileHandle){
			m_fileHandle = -1;
			fprintf(stderr, "openReadWrite io failed file=%s mode=%s\n", m_fileName.c_str(), mode);
			return false;
		}
        m_fileLength = writeTell();
//		fprintf(stderr, "openReadWrite ok file=%s\n", m_fileName.c_str());
		return true;
	}
	void closeReadWrite(void){
	    m_fileLength = 0;
		if(m_fileHandle >= 0){
			close(m_fileHandle);
			m_fileHandle = -1;
		}
	}
	inline void setFileName(const char* fileName){
		m_fileName = fileName;
	}
	inline int64_t seekRead(void * ptr, int64_t size, int64_t n, int64_t offset, int seek){
		fileSeek(offset, seek);
		return fileRead(ptr, size, n);
	}
	inline int64_t seekWrite(const void * ptr, int64_t size, int64_t n, int64_t offset, int seek){
		fileSeek(offset, seek);
		return fileWrite(ptr, size, n);
	}
	// block 文件读写操作
	inline int64_t fileRead(void * ptr, int64_t size, int64_t n){
		return read(m_fileHandle, ptr, size*n);
	}
	inline int64_t fileWrite(const void * ptr, int64_t size, int64_t n){
		return write(m_fileHandle, ptr, size*n);
	}
	inline int64_t fileSeek(int64_t offset, int seek){
		return lseek(m_fileHandle, offset, seek);
	}
	inline int64_t readTell(void){
		return fileSeek(0, SEEK_END);
	}
	inline int64_t writeTell(void){
		return fileSeek(0, SEEK_END);
	}
	inline int flush(void){
		return 0;
	}
	inline bool isOpen(void){
		return (-1 != m_fileHandle);
	}
};

typedef std::unordered_map<std::string, LogFile*> LogFileMap;
typedef std::pair<std::string, std::string> StringPair;
typedef std::deque<StringPair> StringPairQueue;

thread_local static char t_logBuffer[LOG_BUFFER_SIZE] = {0};
thread_local static char t_timeBuffer[32] = {0};
static int g_logLevel = 0;
static int g_maxLogSize = 100*1024*1024;
static bool g_useThread = false;
static bool g_useCache = false;
static WriteRemoteLogFunction g_delegateFunction = NULL;
static ThreadHandleFunction g_threadStartFunction = NULL;
static ThreadHandleFunction g_threadUpdateFunction = NULL;
static LogData* g_pLogData = NULL;

class LogData
{
public:
    std::map<std::string, std::string> m_cacheLog;
    std::string m_logFileName;
    LogFileMap m_logFileMap;
    StringPairQueue m_writeQueue;
    StringPairQueue m_readQueue;
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
    pthread_t m_thread;
public:
    LogData(void) : m_mutex(PTHREAD_MUTEX_INITIALIZER){

    }
    ~LogData(void){

    }
    void initialize(void){
        if(g_useThread){
            pthread_cond_init(&m_cond, NULL);
            pthread_mutex_init(&m_mutex, NULL);
            pthread_create(&m_thread, NULL, LogThreadHandleFunction, NULL);
            pthread_detach(m_thread);
        }
    }
    void update(void){
        pthread_mutex_lock(&m_mutex);
        m_writeQueue.swap(m_readQueue);
        while(m_readQueue.empty()){
            pthread_cond_wait(&m_cond, &m_mutex);
            m_writeQueue.swap(m_readQueue);
        }
        pthread_mutex_unlock(&m_mutex);
        for(auto &p : m_readQueue){
            writeLogInThread(p.first, p.second);
        }
        m_readQueue.clear();
    }
    void removeLogFile(const std::string& fileName){
        LogFileMap::iterator itCur = m_logFileMap.find(fileName);
        if(itCur != m_logFileMap.end()){
            delete itCur->second;
            m_logFileMap.erase(itCur);
        }
    }
    LogFile* getOrReviveLogFile(const std::string& fileName){
        LogFileMap::iterator itCur = m_logFileMap.find(fileName);
        if(itCur == m_logFileMap.end()){
            LogFile* f = new LogFile(fileName, "");
            if( f->openLog() ){
                m_logFileMap.insert(std::make_pair(fileName, f));
                return f;
            }else{
                delete f;
                fprintf(stderr, "open log file failed = %s\n", fileName.c_str());
                return NULL;
            }
        }
        return itCur->second;
    }
    void dumpCacheLog(std::map<std::string, std::string> &logMap){
        m_cacheLog.swap(logMap);
    }
    void writeLog(const char* fileName, const char* str){
        // 如果外部不处理，那么日志直接打印到本地C++日志文件
        if(g_useCache){
            std::string temp;
            pthread_mutex_lock(&m_mutex);
            std::map<std::string, std::string>::iterator itCur = m_cacheLog.find(fileName);
            if(itCur != m_cacheLog.end()){
                itCur->second.append(str);
                // 设置每个文件最多缓存的数量，防止日志占用大量内存爆满
                if(itCur->second.length() >= LOG_MAX_CACHE_SIZE){
                    itCur->second.swap(temp);
                }
            }else{
                m_cacheLog.insert(std::make_pair(std::string(fileName), std::string(str)));
            }
            pthread_mutex_unlock(&m_mutex);
            if(!temp.empty()){
                writeLogToLocalFile(fileName, temp.c_str());
            }
            return;
        }
        writeLogToLocalFile(fileName, str);
    }
    void writeLogToLocalFile(const char* fileName, const char* str){
        if(g_useThread){
            std::pair<std::string, std::string> p = std::make_pair(std::string(fileName), std::string(str));
            pthread_mutex_lock(&m_mutex);
            m_writeQueue.push_back(p);
            pthread_cond_signal(&m_cond);
            pthread_mutex_unlock(&m_mutex);
        }else{
            writeLogInThread(fileName, str);
        }
    }
    void writeLogInThread(const std::string& fileName, const std::string& str){
        LogFile* f = getOrReviveLogFile(fileName);
        if(NULL == f){
            return;
        }
        if( !f->writeLog(str.c_str(), (int64_t)str.length()) ){
            fprintf(stderr, "write log to file=%s failed log=%s", fileName.c_str(), str.c_str());
            removeLogFile(fileName);
            return;
        }
        if(f->getLogLength() >= g_maxLogSize){
            char tempBuffer[256] = {0};
            snprintf(tempBuffer, 256, "%s.%s", f->getFileName().c_str(), getTimeStringUSFile());
            f->renameLog(tempBuffer);
            if( !f->openLog() ){
                fprintf(stderr, "openLog file=%s failed", fileName.c_str());
                removeLogFile(fileName);
                return;
            }
        }
    }
    const std::string& getCppLogFileName(void){
        return m_logFileName;
    }
};

char* getLogBuffer(void){
    return t_logBuffer;
}
bool openCppLog(const char* fileName, bool useCache, bool useThread, ThreadHandleFunction startFunc, ThreadHandleFunction updateFunc){
    if(NULL != g_pLogData){
        return false;
    }
    g_pLogData = new LogData();
    g_pLogData->m_logFileName = std::string(fileName);
    g_useCache = useCache;
    g_useThread = useThread;
    g_threadStartFunction = startFunc;
    g_threadUpdateFunction = updateFunc;
    g_pLogData->initialize();
    return true;
}

void dumpCacheLog(std::map<std::string, std::string> &logMap){
    if(NULL != g_pLogData){
        g_pLogData->dumpCacheLog(logMap);
    }
}
const char* getCppLogFileName(void){
    if(NULL != g_pLogData){
        return g_pLogData->getCppLogFileName().c_str();
    }
    return "hive.log";
}
void setMaxLogSize(int logSize){
    g_maxLogSize = logSize;
}
int getMaxLogSize(void){
    return g_maxLogSize;
}
void setLogLevel(int level){
	g_logLevel = level;
}
int getLogLevel(void){
	return g_logLevel;
}
const char* getTimeString(void){
	time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm* ptm = localtime(&t);
	snprintf(t_timeBuffer, sizeof(t_timeBuffer), "%4d-%02d-%02d %02d:%02d:%02d",
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	return t_timeBuffer;
}
const char* getTimeStringUS(void){
	timeval t;
	gettimeofday( &t, NULL );
	struct tm* ptm = localtime(&t.tv_sec);
	snprintf(t_timeBuffer, sizeof(t_timeBuffer), "%4d-%02d-%02d %02d:%02d:%02d %06d",
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (int)t.tv_usec);
	return t_timeBuffer;
}
const char* getTimeStringUSFile(void){
	timeval t;
	gettimeofday( &t, NULL );
	struct tm* ptm = localtime(&t.tv_sec);
	snprintf(t_timeBuffer, sizeof(t_timeBuffer), "%4d-%02d-%02d_%02d-%02d-%02d_%06d",
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (int)t.tv_usec);
	return t_timeBuffer;
}

void writeCppLog(const char* str){
    writeRemoteLog(getCppLogFileName(), str);
}
void writeRemoteLog(const char* fileName, const char* str){
	if(NULL != g_delegateFunction){
		if( g_delegateFunction(fileName, str) ){
			return;
		}
	}
	if(NULL != g_pLogData){
	    g_pLogData->writeLog(fileName, str);
	}else{
	    fprintf(stderr, "[%s]%s", fileName, str);
	}
}
void setWriteRemoteLogFunction(WriteRemoteLogFunction function){
	g_delegateFunction = function;
}

void* LogThreadHandleFunction(void* arg){
    if(NULL != g_threadStartFunction){
        g_threadStartFunction();
    }
//	pid_t tid = gettid();
//	LOG_INFO("Log thread start tid=%d", (int)tid);
    while(true){
        g_pLogData->update();
        if(NULL != g_threadUpdateFunction){
            g_threadUpdateFunction();
        }
    }
    return NULL;
}
