#ifndef __hive__shared_buffer__
#define __hive__shared_buffer__

#include "base.h"
#include <unordered_map>

NS_HIVE_BEGIN

typedef bool (*SharedBufferUpdateFunction)(void* data, const std::string& key, std::string& value);

class SharedBuffer : public SyncRW
{
public:
    std::unordered_map<std::string, std::string> m_bufferMap;
public:
    SharedBuffer(void);
    virtual ~SharedBuffer(void);

    void set(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value);
    bool del(const std::string& key);
    // 如果数据不存在，那么会直接设置新数据
    void update(const std::string& key, SharedBufferUpdateFunction func, void* data);
};

bool sb_create(int slotNumber);
void sb_set(const std::string& key, const std::string& value);
bool sb_get(const std::string& key, std::string& value);
bool sb_del(const std::string& key);
void sb_update(const std::string& key, SharedBufferUpdateFunction func, void* data);

NS_HIVE_END

#endif
