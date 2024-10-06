#ifndef __hive__file__
#define __hive__file__

// 开启大文件
# define _LARGE_FILE       1
# ifndef _FILE_OFFSET_BITS
#   define _FILE_OFFSET_BITS 64
# endif
# define _LARGEFILE_SOURCE 1

#include <sys/io.h>
#include <sys/mman.h>

#include "base.h"

NS_HIVE_BEGIN

// sysconf(_SC_PAGESIZE)
#define BLOCK_SIZE 4096				// 每个文件块的大小
//#define MAX_EXPAND_BLOCK_SIZE 67108864	// 64M，最大保存的单个文件块长度
#define MAX_EXPAND_BLOCK_SIZE 16777216	// 16M，最大保存的单个文件块长度

class File
{
protected:
	std::string m_fileName;		// 文件名
	int64 m_fileLength;			// 文件长度
	int64 m_seekCursor;         // 当前指针
	int m_fileHandle;			// linux下文件句柄
    int m_expandSize;           // 文件自动扩展的大小
public:
    explicit File(const std::string& name);
    File(void);
    virtual ~File(void);

    virtual bool openFile(bool createNew);
    virtual void closeFile(void);
    virtual bool touchFile(const char* checkHead, int checkLength);
    // offset >= 0从头部开始计算偏移； offset < 0 从尾部开始计算偏移
    virtual bool writeData(int64 offset, const char* pData, int64 length);
    virtual bool readData(int64 offset, char* pData, int64 length);
    virtual bool fastReadData(int64 offset, char* pData, int64 length);
    virtual bool fastWriteData(int64 offset, const char* pData, int64 length, bool clearAll);

	bool renameFile(const std::string& newName){
	    if( rename(m_fileName.c_str(), newName.c_str()) < 0 ){
	        return false;
	    }
	    return true;
	}
	bool renameFile(const char* newName){
	    if( rename(m_fileName.c_str(), newName) < 0 ){
	        return false;
	    }
	    return true;
	}
	bool removeFile(const std::string& fileName){
	    if( remove(fileName.c_str()) < 0 ){
	        return false;
	    }
	    return true;
	}
	bool removeFile(const char* fileName){
	    if( remove(fileName) < 0 ){
	        return false;
	    }
	    return true;
	}
    void setFileName(const char* name){ m_fileName = name; }
    const std::string& getFileName(void){ return m_fileName; }
    int64 getFileLength(void){ return m_fileLength; }
    int getExpandSize(void){ return m_expandSize; }
    void setExpandSize(int exp);
    int getFileHandle(void){ return m_fileHandle; }
	int64 fileSeek(int64 offset, int seek);
	int64 readTell(void){
	    return writeTell();
//		return fileSeek(0, SEEK_END);
	}
	int64 writeTell(void){
	    int64 length = lseek(m_fileHandle, 0, SEEK_END);
	    m_seekCursor = length;
	    return length;
//		return fileSeek(0, SEEK_END);
	}
	int flush(void){
		return 0;
	}
	bool isOpen(void);

    // 特殊使用读取的函数
	char* fastOpenRead(int64 offset, int64 length, int64& mapLength, int64& mapOffset);
	void fastCloseRead(char* pAddr, int64 mapLength);
protected:
    inline void correctOffset(int64* offset){
        if(*offset < 0){
            *offset = m_fileLength + *offset;
        }
    }
    bool openReadWrite(const char* mode);
	inline int64 seekRead(void * ptr, int64 size, int64 n, int64 offset, int seek){
		fileSeek(offset, seek);
		return fileRead(ptr, size, n);
	}
	inline int64 seekWrite(const void * ptr, int64 size, int64 n, int64 offset, int seek){
		fileSeek(offset, seek);
		return fileWrite(ptr, size, n);
	}
	int64 fileRead(void * ptr, int64 size, int64 n);
	int64 fileWrite(const void * ptr, int64 size, int64 n);
};
// ===========================================================================
class MapFile : public File
{
protected:
    typedef std::vector<bool> BitVector;
    BitVector m_dirtyMark;
    char* m_pAddr;
public:
    explicit MapFile(const std::string& name);
    MapFile(void);
    virtual ~MapFile(void);

    virtual bool openFile(bool createNew, int initSize=BLOCK_SIZE);
    virtual void closeFile(void);
    // offset >= 0从头部开始计算偏移； offset < 0 从尾部开始计算偏移
    virtual bool writeData(int64 offset, const char* pData, int64 length);
    virtual bool readData(int64 offset, char* pData, int64 length);

    char* getDataPtr(int64 offset);
    bool syncFile(bool saveAll, bool asyncSave);
    bool syncBlock(int64 offset, int64 length, bool asyncSave);
    void moveTo(int64 to, int64 offset, int64 length);
    void markDirtyBlock(int64 offset, int64 length, bool mark);
protected:
    bool openMapFile(void);
    void closeMapFile(void);
    bool checkSync(bool asyncSave);
    bool syncBlockDirectory(int64 offset, int64 length, bool asyncSave);
};

NS_HIVE_END

#endif
