#ifndef __hive__object_pool__
#define __hive__object_pool__

#include "base.h"
#include "log.h"
#include <new>

NS_HIVE_BEGIN
/*--------------------------------------------------------------------*/
typedef struct ObjectHandle{
    union{
        struct{
            unsigned int 	index 		: 24;	// 下标
            unsigned int 	version 	: 8;	// 版本
        };
        unsigned int 		handle 		: 32;	// 句柄
    };
public:
    ObjectHandle(unsigned int h) : handle(h) {}
    ObjectHandle(unsigned int index, unsigned int version){ this->index = index; this->version=version; }
    ObjectHandle(void) : handle(0) {}
    ~ObjectHandle(void){}

    inline unsigned int getIndex(void) const { return this->index; }
    inline unsigned int getVersion(void) const { return this->version; }
    inline unsigned int getHandle(void) const { return this->handle; }
    inline void setIndex(unsigned int index){ this->index = index; }
    inline void increaseVersion(void){ ++(this->version); }
    inline bool operator==(unsigned int h) const { return (this->handle == h); }
    inline bool operator!=(unsigned int h) const { return (this->handle != h); }
    inline bool operator==(const ObjectHandle& h) const{ return (this->handle == h.handle); }
    inline bool operator!=(const ObjectHandle& h) const{ return (this->handle != h.handle); }
    inline bool operator<(unsigned int h) const { return (this->handle < h); }
    inline bool operator<(const ObjectHandle& h) const { return (this->handle < h.handle); }
    inline ObjectHandle& operator=(unsigned int h){ this->handle = h; return *this; }
    inline ObjectHandle& operator=(ObjectHandle& h){ this->handle = h.handle; return *this; }
}ObjectHandle;
/*--------------------------------------------------------------------*/
class BaseTypePool
{
public:
    BaseTypePool(void){}
    virtual ~BaseTypePool(void){}

    virtual void destroy(void* p) = 0;
    virtual void clear(void) = 0;
};
/*--------------------------------------------------------------------*/
class BaseObjectPool
{
public:
    BaseObjectPool(void){}
    virtual ~BaseObjectPool(void){}

    virtual char* allocObjectBuffer(size_t length) = 0;
};
/*--------------------------------------------------------------------*/
template<class U>
class TypePool : public BaseTypePool
{
public:
    BaseObjectPool* m_pObjectPool;
    std::vector<U*> m_idleObject;
    std::vector<U*> m_allObject;
public:
    TypePool(BaseObjectPool* pObjectPool) : BaseTypePool(), m_pObjectPool(pObjectPool) {}
    virtual ~TypePool(void){
        clear();
    }

    template <class... Args>
    U* create(Args&&... args){
        U* p;
        if(m_idleObject.empty()){
            char* ptr = m_pObjectPool->allocObjectBuffer(sizeof(U));
            p = new (ptr) U(std::forward<Args>(args)...);
            m_allObject.push_back(p);
        }else{
            p = m_idleObject.back();
            m_idleObject.pop_back();
            p->setParam(std::forward<Args>(args)...);
        }
        return p;
    }
    virtual void destroy(void* p){
        m_idleObject.push_back((U*)p);
    }
    virtual void clear(void){
        U* p;
        for(int i=0; i<(int)m_allObject.size(); ++i){
            p = m_allObject[i];
            p->~U();
        }
        m_allObject.clear();
        m_idleObject.clear();
    }
protected:

};
/*--------------------------------------------------------------------*/
const int OBJECT_POOL_BUFFER_BLOCK_LENGTH = 128*1024 - 16;
typedef struct ObjectPoolBuffer
{
    char* pBuffer;
    int length;
    int allocCount;
}ObjectPoolBuffer;
class ObjectPool : public BaseObjectPool
{
public:
    std::map<int, BaseTypePool*> m_typePoolMap;
    std::vector<void*> m_indexArray;
    std::vector<unsigned int> m_handleArray;
    std::vector<ObjectHandle> m_idleHandle;
    std::vector<ObjectPoolBuffer> m_bufferArray;
public:
    ObjectPool(void) : BaseObjectPool() {
        m_indexArray.resize(1, NULL);
        m_handleArray.resize(1, 0);
    }
    ~ObjectPool(void){
        clear();
    }

    virtual char* allocObjectBuffer(size_t length){
        char* ptr;
        size_t allocSize = calculateAlignSize(length);
        if( !checkExpandBufferArray(allocSize) ){
            LOG_ERROR("allocObjectBuffer failed allocSize=%lld", (long long int)allocSize);
            return NULL;
        }
        ObjectPoolBuffer& data = m_bufferArray.back();
        ptr = data.pBuffer + data.allocCount;
        data.allocCount += allocSize;
//        fprintf(stderr, "allocObjectBuffer length=%d\n", length);
        return ptr;
    }

    template <class U>
    U* getObject(unsigned int h){
        ObjectHandle handle(h);
        if(handle.index >= (unsigned int)m_indexArray.size()){
            return NULL;
        }
        if(h != m_handleArray[handle.index]){
            return NULL;
        }
        U* p = (U*)m_indexArray[handle.index];
//        if(h != p->getHandle()){
//            return NULL;
//        }
        return p;
    }
    template <class U, class... Args>
    U* create(Args&&... args){
        U* p;
        TypePool<U>* pPool;
        int objectType = U::objectType();      // 静态函数
        if(0 == objectType){
            LOG_ERROR("ObjectPool create objectType == 0");
            return NULL;
        }
        std::map<int, BaseTypePool*>::iterator itMap = m_typePoolMap.find(objectType);
        if(itMap == m_typePoolMap.end()){
            pPool = new TypePool<U>(this);
            m_typePoolMap.insert(std::make_pair(objectType, pPool));
        }else{
            pPool = (TypePool<U>*)itMap->second;
        }
        p = pPool->create(std::forward<Args>(args)...);
        ObjectHandle handle = getIdleHandle();
        p->setHandle(handle.handle);
        m_indexArray[handle.index] = p;
        m_handleArray[handle.index] = handle.handle;
        return p;
    }
    template <class U>
    void destroy(U* p){
        if(NULL == p){
            return;
        }
        ObjectHandle handle(p->getHandle());
        unsigned int index = handle.index;
        if(index >= (unsigned int)m_indexArray.size()){
            return;
        }
        if(p != m_indexArray[index]){
            return;
        }
        int objectType = p->getType();      // 继承函数
        p->clearData();                     // 清理数据
        putIdleObject(objectType, p);
        putIdleHandle(handle);
        m_indexArray[index] = NULL;
        m_handleArray[index] = 0;
    }
protected:
    void clear(void){
        std::map<int, BaseTypePool*>::iterator itMap = m_typePoolMap.begin();
        for( ; itMap != m_typePoolMap.end() ; ++itMap){
            itMap->second->clear();
            delete itMap->second;
        }
        m_typePoolMap.clear();
        for(int i=0; i<(int)m_bufferArray.size(); ++i){
            ObjectPoolBuffer& data = m_bufferArray[i];
            free(data.pBuffer);
        }
        m_bufferArray.clear();
        m_indexArray.clear();
        m_handleArray.clear();
        m_idleHandle.clear();
        m_indexArray.resize(1, NULL);
        m_handleArray.resize(1, 0);
    }
    inline bool checkExpandBufferArray(int allocSize){
        if(allocSize >= OBJECT_POOL_BUFFER_BLOCK_LENGTH){
            LOG_ERROR("checkExpandBufferArray allocSize=%d", allocSize);
            return false;
        }

        bool isNeedAlloc = false;
        if(m_bufferArray.empty()){
            isNeedAlloc = true;
        }else{
            ObjectPoolBuffer& data = m_bufferArray.back();
            if(data.allocCount + allocSize > data.length){
                isNeedAlloc = true;
            }
        }
        if(isNeedAlloc){
            ObjectPoolBuffer bufferData;
            bufferData.pBuffer = (char*)malloc(OBJECT_POOL_BUFFER_BLOCK_LENGTH);
            bufferData.length = OBJECT_POOL_BUFFER_BLOCK_LENGTH;
            bufferData.allocCount = 0;
            m_bufferArray.push_back(bufferData);
        }
        return true;
    }
    inline size_t calculateAlignSize(size_t length){
        size_t modValue = length % 16;
        if(0 == modValue){
            return length;
        }
        return length + 16 - modValue;
    }
    inline void putIdleObject(int objectType, void* p){
        std::map<int, BaseTypePool*>::iterator itMap = m_typePoolMap.find(objectType);
        if(itMap == m_typePoolMap.end()){
            LOG_ERROR("putIdleObject objectType = %d not create yet", objectType);
            return;
        }
        itMap->second->destroy(p);
    }
    inline void putIdleHandle(ObjectHandle handle){
        m_idleHandle.push_back(handle);
    }
    inline ObjectHandle getIdleHandle(void){
        if(!m_idleHandle.empty()){
            ObjectHandle handle = m_idleHandle.back();
            m_idleHandle.pop_back();
            handle.increaseVersion();
            return handle;
        }
        unsigned int index = (unsigned int)m_indexArray.size();
        m_indexArray.push_back(NULL);
        m_handleArray.push_back(0);
//        m_handleArray.resize(index + 1, NULL);
        ObjectHandle handle(index, 0);
        return handle;
    }
};
/*--------------------------------------------------------------------*/
NS_HIVE_END

#endif
