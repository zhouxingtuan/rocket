//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/5/28
// Time: 下午3:09
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__buffer__
#define __hive__buffer__

#include "base.h"
#include "log.h"

NS_HIVE_BEGIN

// Buffer 不可以超过 MAX_INT32
class Buffer : public RefObject
{
protected:
    char* m_pBuffer;
    // int m_length; // from RefObject
public:
    Buffer(void);
    Buffer(int referenceCount);
    virtual ~Buffer(void);

    void copyFrom(Buffer* pBuffer);
    void clear(void);
    int write(const char* ptr, int length, int offset);
    int read(char* ptr, int length, int offset);
    void resize(int newSize, int value);
    void resize(int newSize);
    void reserve(int newSize);
    char* data(void);
    int size(void);
};

NS_HIVE_END

#endif
