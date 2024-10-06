
#include "level.h"
#include "leveldb/write_batch.h"
#include "leveldb/filter_policy.h"
#include "leveldb/cache.h"

NS_HIVE_BEGIN

static leveldb::DB *g_pDB = NULL;

unsigned int crc32(unsigned int crc, const unsigned char* data, int64 count){
    return crc32c_extend(crc, data, count);
}
int64 snappy_compress(const char* input, int64 input_length, std::string& compressed){
    return snappy::Compress(input, input_length, &compressed);
}
bool snappy_uncompress(const char* compressed, int64 compressed_length, std::string& uncompressed){
    return snappy::Uncompress(compressed, compressed_length, &uncompressed);
}

bool db_open(const std::string& name){
    leveldb::Options options;
    options.create_if_missing = true;
    options.write_buffer_size = 16 * 1024 * 1024;
    options.max_open_files = 65535;
    options.block_cache = leveldb::NewLRUCache(128 * 1024 * 1024);  // 128MB cache
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);

    // open
    leveldb::Status status = leveldb::DB::Open(options, name, &g_pDB);
    if(status.ok()){
        return true;
    }
    LOG_ERROR("db_open failed error: %s", status.ToString().c_str());
    return false;
}
bool db_set(const std::string& key, const std::string& value){
    leveldb::Slice sKey(key);
    leveldb::Slice sValue(value);
    leveldb::Status status = g_pDB->Put(leveldb::WriteOptions(), sKey, sValue);
    if(status.ok()){
        return true;
    }
    LOG_ERROR("db_set failed error: %s", status.ToString().c_str());
    return false;
}
bool db_get(const std::string& key, std::string& value){
    leveldb::Slice sKey(key);
    leveldb::Status status = g_pDB->Get(leveldb::ReadOptions(), sKey, &value);
    if(status.ok()){
        return true;
    }
    return false;
}
bool db_del(const std::string& key){
    leveldb::Slice sKey(key);
    leveldb::Status status = g_pDB->Delete(leveldb::WriteOptions(), sKey);
    if(status.ok()){
        return true;
    }
    return false;
}
// array{ {op_type, op_key, op_value} } -- op_type 1 更新； 2 删除；
bool db_batch(lua_State* L){
    leveldb::WriteBatch batch;
	/* table 放在索引 't' 处 */
	lua_pushnil(L);  /* 第一个 key */
	while (lua_next(L, 1) != 0) {
		/* 用一下 'key' （在索引 -2 处） 和 'value' （在索引 -1 处） */
		//	 printf("%s - %s\n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
		int op_type = ((int)tolua_tonumber(L,-3,0));
		size_t key_length = 0;
		size_t value_length = 0;
		const char* key = ((const char*) lua_tolstring(L,-2,&key_length));
		const char* value = ((const char*) lua_tolstring(L,-1,&value_length));
        leveldb::Slice sKey(key, key_length);
        leveldb::Slice sValue(value, value_length);
        if(1 == op_type){
            batch.Put(sKey, sValue);
        }else{
            batch.Delete(sKey);
        }
        LOG_DEBUG("db_batch op_type=%d key=%s value=%s", op_type, key, value);
		/* 移除 'value' ；保留 'key' 做下一次迭代 */
		lua_pop(L, 1);
	}
	bool result = db_batch(&batch);
	return result;
//    leveldb::Status status = g_pDB->Write(leveldb::WriteOptions(), &batch);
//    if(status.ok()){
//        return true;
//    }
//    return false;
}
bool db_batch(leveldb::WriteBatch* batch){
    leveldb::Status status = g_pDB->Write(leveldb::WriteOptions(), batch);
    if(status.ok()){
        return true;
    }
    return false;
}

NS_HIVE_END
