
#include "shared_buffer.h"
#include "util.h"
#include "log.h"

NS_HIVE_BEGIN

SharedBuffer::SharedBuffer(void) : SyncRW(){

}
SharedBuffer::~SharedBuffer(void){

}

void SharedBuffer::set(const std::string& key, const std::string& value){
    std::unordered_map<std::string, std::string>::iterator itMap;
    lock();
    itMap = m_bufferMap.find(key);
    if(itMap != m_bufferMap.end()){
        itMap->second = value;
    }else{
        m_bufferMap[key] = value;
    }
    unlock();
}
bool SharedBuffer::get(const std::string& key, std::string& value){
    std::unordered_map<std::string, std::string>::iterator itMap;
    bool isFind = false;
    value.clear();
    rlock();
    itMap = m_bufferMap.find(key);
    if(itMap != m_bufferMap.end()){
        value = itMap->second;
        isFind = true;
    }
    unlock();
    return isFind;
}
bool SharedBuffer::del(const std::string& key){
    std::unordered_map<std::string, std::string>::iterator itMap;
    bool isFind = false;
    lock();
    itMap = m_bufferMap.find(key);
    if(itMap != m_bufferMap.end()){
        m_bufferMap.erase(itMap);
        isFind = true;
    }
    unlock();
    return isFind;
}
void SharedBuffer::update(const std::string& key, SharedBufferUpdateFunction func, void* data){
    std::unordered_map<std::string, std::string>::iterator itMap;
    std::string value;
    lock();
    itMap = m_bufferMap.find(key);
    if(itMap != m_bufferMap.end()){
        if( func(data, key, itMap->second) ){
            // 完成更新动作，直接修改value返回值
        }
    }else{
        if( func(data, key, value) ){
            m_bufferMap[key] = value;
        }
    }
    unlock();
}

static std::vector<SharedBuffer*>* g_pVecSB = NULL;

SharedBuffer* get_sb(const std::string& key){
    uint32 h = binary_djb_hash(key.c_str(), key.length());
    int index = h % g_pVecSB->size();
//    LOG_DEBUG("get_sb key=%s index=%d h=%u", key.c_str(), index, h);
    return (*g_pVecSB)[index];
}
bool sb_create(int slotNumber){
    if(NULL != g_pVecSB){
        return false;
    }
    g_pVecSB = new std::vector<SharedBuffer*>();
    g_pVecSB->resize(slotNumber, NULL);
    for(int i=0; i<slotNumber; ++i){
        SharedBuffer* pBuffer = new SharedBuffer();
        (*g_pVecSB)[i] = pBuffer;
    }
    return true;
}
void sb_set(const std::string& key, const std::string& value){
    get_sb(key)->set(key, value);
}
bool sb_get(const std::string& key, std::string& value){
    return get_sb(key)->get(key, value);
}
bool sb_del(const std::string& key){
    return get_sb(key)->del(key);
}
void sb_update(const std::string& key, SharedBufferUpdateFunction func, void* data){
    get_sb(key)->update(key, func, data);
}

NS_HIVE_END
