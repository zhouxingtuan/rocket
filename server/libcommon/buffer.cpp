//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/5/28
// Time: 下午3:09
// To change this template use File | Settings | File Templates.
//

#include "buffer.h"

NS_HIVE_BEGIN

Buffer::Buffer(void) : RefObject(), m_pBuffer(NULL) {

}
Buffer::Buffer(int referenceCount) : RefObject(referenceCount), m_pBuffer(NULL){

}
Buffer::~Buffer(void){
    if(NULL != m_pBuffer){
        free(m_pBuffer);
        m_pBuffer = NULL;
    }
}

void Buffer::copyFrom(Buffer* pBuffer){
	this->resize(pBuffer->size());
	memcpy(this->data(), pBuffer->data(), pBuffer->size());
}
void Buffer::clear(void){
    m_length = 0;
}
int Buffer::write(const char* ptr, int length, int offset){
    int needSize = offset + length;
    if( this->size() < needSize ){
        this->resize(needSize);
    }
    char* writeTo = (char*)(this->data()) + offset;
    memcpy(writeTo, ptr, length);
    return length;
}
int Buffer::read(char* ptr, int length, int offset){
    int needSize = offset + length;
    if( (int)this->size() < needSize){
        return 0;
    }
    char* readFrom = (char*)(this->data()) + offset;
    memcpy(ptr, readFrom, length);
    return length;
}
void Buffer::resize(int newSize, int value){
    reserve(newSize);
    int oldLength = m_length;
    m_length = newSize;
    if(newSize > oldLength){
        memset(m_pBuffer+oldLength, value, newSize-oldLength);
    }
}
void Buffer::resize(int newSize){
    reserve(newSize);
    m_length = newSize;
}
void Buffer::reserve(int newSize){
    char* pBuffer = (char*)realloc(m_pBuffer, newSize);
    if(NULL == pBuffer){
        LOG_ERROR("reserve failed newSize=%d", newSize);
        return;
    }
    m_pBuffer = pBuffer;
}
char* Buffer::data(void){
    return m_pBuffer;
}
int Buffer::size(void){
    return m_length;
}

NS_HIVE_END
