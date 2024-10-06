#ifndef __hive__level__
#define __hive__level__

#include "common.h"
#include "net.h"

#include "lua.hpp"
#include "tolua++.h"
#include "crc32c/crc32c.h"
#include "snappy.h"
#include "leveldb/db.h"

NS_HIVE_BEGIN

unsigned int crc32(unsigned int crc, const unsigned char* data, int64 count);
int64 snappy_compress(const char* input, int64 input_length, std::string& compressed);
bool snappy_uncompress(const char* compressed, int64 compressed_length, std::string& uncompressed);

bool db_open(const std::string& name);
bool db_set(const std::string& key, const std::string& value);
bool db_get(const std::string& key, std::string& value);
bool db_del(const std::string& key);
bool db_batch(lua_State* L);    // array{ {op_type, op_key, op_value} } -- op_type 1 更新； 2 删除；
bool db_batch(leveldb::WriteBatch* batch);

NS_HIVE_END

#endif
