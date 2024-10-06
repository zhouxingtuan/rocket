
#ifndef __hive__websocket__
#define __hive__websocket__

#include "common.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

NS_HIVE_BEGIN

extern std::string SEC_WEBSOCKET_PROTOCOL_HEADER;
extern std::string SEC_WEBSOCKET_PROTOCOL_HEADER_LOWER_CASE;
extern std::string SEC_WEBSOCKET_KEY_HEADER;
extern std::string SEC_WEBSOCKET_KEY_HEADER_LOWER_CASE;
extern std::string SEC_WEBSOCKET_ACCEPT_HEADER;

std::vector<char> accept_upgrade_response(const std::string& accept_key, const std::string& accept_protocol);
void encode_ws_frame_head(std::vector<char>& frame, size_t data_length, bool client_frame);
void encode_ws_frame_body(std::vector<char>& frame, const char* ptr, size_t data_length, bool client_frame);
int loop_parse_ws_frames(std::function<void(std::vector<char>&)> handler, std::function<void(size_t)> moveOffset, bool client_frame, std::vector<char>& buffer);

std::string url_encode(const char* str, unsigned int length);
std::string url_decode(const char* str, unsigned int length);

NS_HIVE_END

#endif
