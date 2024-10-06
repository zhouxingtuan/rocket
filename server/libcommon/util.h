//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/7/23
// Time: 下午2:01
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__util__
#define __hive__util__

#include "base.h"

NS_HIVE_BEGIN
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#define BINARY_HASH_SEED 5381

uint32_t binary_djb_hash(const char* cstr, unsigned int length);
uint32 djb_hash(const char* str, uint32 length);

void binary_encrypt_quick(char* ptr, unsigned int length, const char* key);		// 快速加密
void binary_decrypt_quick(char* ptr, unsigned int length, const char* key);		//

void binary_encrypt(char* ptr, unsigned int length, const char* key);			// 普通加密，速度和加密程度居中
void binary_decrypt(char* ptr, unsigned int length, const char* key);			//

void binary_encrypt_hard(char* ptr, unsigned int length, const char* key);		// 强度较高的加密
void binary_decrypt_hard(char* ptr, unsigned int length, const char* key);		//

NS_HIVE_END

#endif
