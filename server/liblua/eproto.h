#ifndef __eproto_h__
#define __eproto_h__

#include "common.h"

extern "C" {
#include "lua.h"
}

#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "wsock32.lib")
#else
#include <arpa/inet.h>
#endif

#include <vector>
#include <string>
#include <unordered_map>

LUALIB_API int luaopen_eproto(lua_State *L);

//namespace eproto{
NS_HIVE_BEGIN

typedef enum DataType
{
    ep_type_nil = 1,
    ep_type_bool,
    ep_type_float,
    ep_type_int,
    ep_type_string,
    ep_type_bytes,
    ep_type_array,
    ep_type_map,
    ep_type_message,
    ep_type_max,
}DataType;

#define WRITE_BUFFER_SIZE 4096
typedef struct WriteBuffer{
	unsigned char* buffer;
	unsigned int bufferSize;
	unsigned int offset;
	int err;

	WriteBuffer(void) : buffer(NULL), bufferSize(0), offset(0), err(0) {
	    bufferSize = WRITE_BUFFER_SIZE - 16;
		buffer = (unsigned char*)malloc(bufferSize);

	}
	~WriteBuffer(void){
	    if(buffer != NULL){
	        free(buffer);
	        buffer = NULL;
	    }
	}
	unsigned char* data(void){ return (unsigned char*)buffer; }
	unsigned int size(void) const { return offset; }
	void write(const unsigned char* ptr, unsigned int length){
		allocBuffer(offset + length);
		memcpy(buffer + offset, ptr, length);
		offset += length;
	}
	void rwrite(const unsigned char* ptr, unsigned int length){
		allocBuffer(offset + length);
		for(int i=(int)length-1; i>=0; --i){
			buffer[offset++] = ptr[i];
		}
	}
	void push(unsigned char b){
		allocBuffer(offset + 1);
		buffer[offset++] = b;
	}
	void add(unsigned char b, unsigned char v){
		allocBuffer(offset + 2);
		buffer[offset++] = b;
		buffer[offset++] = v;
	}
	void add(unsigned char b, const unsigned char* ptr, unsigned int length){
		allocBuffer(offset + 1 + length);
		buffer[offset++] = b;
		memcpy(buffer + offset, ptr, length);
		offset += length;
	}
	void add(unsigned char b, unsigned char v, const unsigned char* ptr, unsigned int length){
		allocBuffer(offset + 2 + length);
		buffer[offset++] = b;
		buffer[offset++] = v;
		memcpy(buffer + offset, ptr, length);
		offset += length;
	}
	void add(unsigned char b, unsigned short v, const unsigned char* ptr, unsigned int length){
		allocBuffer(offset + 3 + length);
		buffer[offset++] = b;
		*((unsigned short*)(buffer+offset)) = v;
		offset += 2;
		memcpy(buffer + offset, ptr, length);
		offset += length;
	}
	void add(unsigned char b, unsigned int v, const unsigned char* ptr, unsigned int length){
		allocBuffer(offset + 5 + length);
		buffer[offset++] = b;
		*((unsigned int*)(buffer+offset)) = v;
		offset += 4;
		memcpy(buffer + offset, ptr, length);
		offset += length;
	}
	void radd(unsigned char b, const unsigned char* ptr, unsigned int length){
		allocBuffer(offset + 1 + length);
		buffer[offset++] = b;
		for(int i=(int)length-1; i>=0; --i){
			buffer[offset++] = ptr[i];
		}
	}
	void clear(void){
		offset = 0;
		err = 0;
	}
	void allocBuffer(unsigned int size){
		if(size < bufferSize){
			return;
		}
		unsigned int new_length = bufferSize + WRITE_BUFFER_SIZE;
		if(new_length < size){
			new_length = size;
		}
		unsigned char* new_buf = (unsigned char*)realloc(buffer, new_length);
		if(NULL != new_buf){
			buffer = new_buf;
		}else{
			new_buf = (unsigned char*)malloc(new_length);
			memcpy(new_buf, buffer, bufferSize);
			free(buffer);
			buffer = new_buf;
		}
		bufferSize = new_length;
	}
	void setError(int e){ err |= e; }
	int getError(void) const { return err; }
}WriteBuffer;

typedef struct ReadBuffer{
    unsigned char *ptr;
    unsigned int length;
    unsigned int offset;
    int err;
    ReadBuffer(void) : ptr(NULL), length(0), offset(0), err(0){}
    ReadBuffer(unsigned char *p, unsigned int len) : ptr(p), length(len), offset(0), err(0){}
    unsigned char* data(void){ return ptr; }
    void moveOffset(unsigned int len){ offset += len; }
    unsigned char moveNext(void){
        return ptr[offset++];
    }
    unsigned char* offsetPtr(void) { return ptr + offset; }
    bool isOffsetEnd(void) const { return offset >= length; }
    void clear(void){
        ptr = NULL;
        length = 0;
        offset = 0;
        err = 0;
    }
    void setError(int e){ err |= e; }
    int getError(void) const { return err; }
    unsigned int left(void) {
//        if(offset > length){
//            return 0;
//        }
        return length - offset;
    }
}ReadBuffer;

typedef struct ProtoElement{
	unsigned int type : 8;
	unsigned int index : 24;
	union{
		// map数据结构
	    struct{
	        unsigned int key : 16;
	        unsigned int value : 16;
	    };
	    // 通用数据，message
        unsigned int id;
	};
	std::string name;
	ProtoElement(void) : type(0), index(0), id(0){}
	void set(unsigned int type, unsigned int index, const std::string& name, unsigned int id){
	    this->type = type;
	    this->index = index;
        this->id = id;
        this->name = name;
	}
	void set(unsigned int type, unsigned int index, const std::string& name, unsigned int key, unsigned int value){
	    this->type = type;
	    this->index = index;
        this->key = key;
        this->value = value;
        this->name = name;
	}
}ProtoElement;
typedef std::vector<ProtoElement> ProtoElementVector;
typedef std::vector<ProtoElementVector> ProtoVector;
typedef std::unordered_map<std::string, unsigned int> ProtoIndexMap;

class ProtoManager
{
public:
    ProtoVector m_protoVector;
    ProtoIndexMap m_indexMap;
public:
    ProtoManager(void){}
    ~ProtoManager(void){}

    ProtoElementVector* findProto(const std::string& path){
        ProtoIndexMap::iterator itCur = m_indexMap.find(path);
        if(itCur != m_indexMap.end()){
            return &(m_protoVector[itCur->second - ep_type_max]);
        }
        return NULL;
    }
    ProtoElementVector* findProto(unsigned int id){
        if(id < ep_type_max){
            return NULL;
        }
        id -= ep_type_max;
        if(id < m_protoVector.size()){
            return &(m_protoVector[id]);
        }
        return NULL;
    }
    unsigned int findProtoID(const std::string& path){
        ProtoIndexMap::iterator itCur = m_indexMap.find(path);
        if(itCur != m_indexMap.end()){
            return itCur->second;
        }
        return 0;
    }
    unsigned int registerProto(const std::string& path){
        unsigned int id = findProtoID(path);
        if(id == 0){
            id = m_protoVector.size() + ep_type_max;
            m_protoVector.push_back(ProtoElementVector());
            m_indexMap.insert(std::make_pair(path, id));
        }
        return id;
    }
    ProtoElement* setElement(unsigned int type, unsigned int index, const std::string& name, unsigned int id, ProtoElementVector* pVec){
//        if(index == 0){
//            return NULL;
//        }
        ProtoElement* pe;
        if(index >= (unsigned int)pVec->size()){
            pVec->resize(index+1);
        }
        // 外部index从0开始
        pe = &((*pVec)[index]);
        pe->set(type, index, name, id);
        return pe;
    }
    ProtoElement* setElement(unsigned int type, unsigned int index, const std::string& name, unsigned int key, unsigned int value, ProtoElementVector* pVec){
//        if(index == 0){
//            return NULL;
//        }
        ProtoElement* pe;
        if(index >= (unsigned int)pVec->size()){
            pVec->resize(index+1);
        }
        // 外部index从0开始
        pe = &((*pVec)[index]);
        pe->set(type, index, name, key, value);
        return pe;
    }
    // 数据类型是 message
    void registerElement(const std::string& path, unsigned int type, unsigned int index, const std::string& name, const std::string& valueName){
		unsigned int valueID = registerProto(valueName);
		registerElement(path, type, index, name, valueID);
    }
    // 数据类型是 常用数据类型
    void registerElement(const std::string& path, unsigned int type, unsigned int index, const std::string& name, unsigned int valueID){
        unsigned int protoID = registerProto(path);
        ProtoElementVector* pVec = findProto(protoID);
        setElement(type, index, name, valueID, pVec);
    }
    // 注册map时使用，key为常用数据类型，value为message数据类型
    void registerElement(const std::string& path, unsigned int type, unsigned int index, const std::string& name, unsigned int key, const std::string& valueName){
        unsigned int valueID = registerProto(valueName);
        registerElement(path, type, index, name, key, valueID);
    }
    // 注册map时使用，key，value都是常用数据类型
    void registerElement(const std::string& path, unsigned int type, unsigned int index, const std::string& name, unsigned int key, unsigned int value){
        unsigned int protoID = registerProto(path);
        ProtoElementVector* pVec = findProto(protoID);
        setElement(type, index, name, key, value, pVec);
    }
};

typedef struct ProtoState{
    WriteBuffer* pWriteBuffer;
    ProtoManager* pManager;
}ProtoState;

//}; // end namespace ep

NS_HIVE_END

#endif