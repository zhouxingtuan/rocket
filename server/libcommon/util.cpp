//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/7/23
// Time: 下午2:02
// To change this template use File | Settings | File Templates.
//

#include "util.h"

NS_HIVE_BEGIN

uint32_t binary_djb_hash(const char* cstr, unsigned int length){
    char* str = const_cast<char*>(cstr);
//    uint32_t hash = 5381;
    uint32_t hash = BINARY_HASH_SEED;
    while (length){
        hash += (hash << 5) + (*str++);
        --length;
    }
    return (hash & 0x7FFFFFFF);
}
uint32 djb_hash(const char* str, uint32 length){
    return (uint32)binary_djb_hash(str, length);
}
// tiny加密
void tiny_encrypt(uint32_t* v, uint32_t* k, uint32_t round){
	uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
	uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
	uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
	for (i=0; i < round; i++) {                       /* basic cycle start */
		sum += delta;
		v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
		v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
	}                                              /* end cycle */
	v[0]=v0; v[1]=v1;
}
// tiny解密
void tiny_decrypt(uint32_t* v, uint32_t* k, uint32_t round, uint32_t move){
	uint32_t v0=v[0], v1=v[1], sum/*=0xC6EF3720*/, i;  /* set up */
	uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
	sum = delta << move;
	uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
	for (i=0; i<round; i++) {                         /* basic cycle start */
		v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
		v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
		sum -= delta;
	}                                              /* end cycle */
	v[0]=v0; v[1]=v1;
}

// 如果当前要加密的长度大于8，那么多出来的字节不再加密；如果长度小于8，不进行加密操作
// 快速加密
void binary_encrypt_quick(char* ptr, unsigned int length, const char* key){
    char* stop_ptr = ptr + (length-length%8);
    uint32_t* u_key = (uint32_t*)key;
    while(ptr < stop_ptr){
		tiny_encrypt( (uint32_t*)ptr, u_key, 16 );
    	ptr += 8;
    };
}
void binary_decrypt_quick(char* ptr, unsigned int length, const char* key){
    char* stop_ptr = ptr + (length-length%8);
    uint32_t* u_key = (uint32_t*)key;
    while(ptr < stop_ptr){
		tiny_decrypt( (uint32_t*)ptr, u_key, 16, 4 );
    	ptr += 8;
    };
}
// 普通加密，速度和加密程度居中
void binary_encrypt(char* ptr, unsigned int length, const char* key){
    char* stop_ptr = ptr + (length-length%8);
    uint32_t* u_key = (uint32_t*)key;
    while(ptr < stop_ptr){
		tiny_encrypt( (uint32_t*)ptr, u_key, 32 );
    	ptr += 8;
    };
}
void binary_decrypt(char* ptr, unsigned int length, const char* key){
    char* stop_ptr = ptr + (length-length%8);
    uint32_t* u_key = (uint32_t*)key;
    while(ptr < stop_ptr){
		tiny_decrypt( (uint32_t*)ptr, u_key, 32, 5 );
    	ptr += 8;
    };
}
// 强度较高的加密
void binary_encrypt_hard(char* ptr, unsigned int length, const char* key){
    char* stop_ptr = ptr + (length-length%8);
    uint32_t* u_key = (uint32_t*)key;
    while(ptr < stop_ptr){
		tiny_encrypt( (uint32_t*)ptr, u_key, 64 );
    	ptr += 8;
    };
}
void binary_decrypt_hard(char* ptr, unsigned int length, const char* key){
    char* stop_ptr = ptr + (length-length%8);
    uint32_t* u_key = (uint32_t*)key;
    while(ptr < stop_ptr){
		tiny_decrypt( (uint32_t*)ptr, u_key, 64, 6 );
    	ptr += 8;
    };
}

NS_HIVE_END
