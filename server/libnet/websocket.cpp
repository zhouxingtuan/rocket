
#include "websocket.h"
#include "http_parser.h"

NS_HIVE_BEGIN

//inline int32 get_time_seed(void){
//    std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
//	int64 ts = (int64)std::chrono::duration_cast<std::chrono::seconds>(p.time_since_epoch()).count();
//	ts = ts % MAX_INT32;
//	return (int32)ts;
//}

#ifdef __GNUC__
#define LIKELY(expr) __builtin_expect(!!(expr), 1)
#define UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#define PRETTY_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define LIKELY(expr) expr
#define UNLIKELY(expr) expr
#define PRETTY_FUNCTION_NAME ""
#endif

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#define CHECK(expr)                                                           \
  do {                                                                        \
    if (UNLIKELY(!(expr))) {                                                  \
      static const char* const args[] = { __FILE__, STRINGIFY(__LINE__),      \
                                          #expr, PRETTY_FUNCTION_NAME };      \
      assert(&args);                                                    \
    }                                                                         \
  } while (0)

#define CHECK_EQ(a, b) CHECK((a) == (b))

#define base64_encoded_size(size) ((size + 2 - ((size + 2) % 3)) / 3 * 4)
#define ACCEPT_KEY_LENGTH base64_encoded_size(20)
static size_t base64_encode(const char* src, size_t slen, char* dst, size_t dlen) {
  // We know how much we'll write, just make sure that there's space.
  CHECK(dlen >= base64_encoded_size(slen) &&
        "not enough space provided for base64 encode");

  dlen = base64_encoded_size(slen);

  unsigned a;
  unsigned b;
  unsigned c;
  unsigned i;
  unsigned k;
  unsigned n;

  static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz"
                              "0123456789+/";

  i = 0;
  k = 0;
  n = slen / 3 * 3;

  while (i < n) {
    a = src[i + 0] & 0xff;
    b = src[i + 1] & 0xff;
    c = src[i + 2] & 0xff;

    dst[k + 0] = table[a >> 2];
    dst[k + 1] = table[((a & 3) << 4) | (b >> 4)];
    dst[k + 2] = table[((b & 0x0f) << 2) | (c >> 6)];
    dst[k + 3] = table[c & 0x3f];

    i += 3;
    k += 4;
  }

  if (n != slen) {
    switch (slen - n) {
      case 1:
        a = src[i + 0] & 0xff;
        dst[k + 0] = table[a >> 2];
        dst[k + 1] = table[(a & 3) << 4];
        dst[k + 2] = '=';
        dst[k + 3] = '=';
        break;

      case 2:
        a = src[i + 0] & 0xff;
        b = src[i + 1] & 0xff;
        dst[k + 0] = table[a >> 2];
        dst[k + 1] = table[((a & 3) << 4) | (b >> 4)];
        dst[k + 2] = table[(b & 0x0f) << 2];
        dst[k + 3] = '=';
        break;
    }
  }

  return dlen;
}
static const char CLOSE_FRAME[] = {'\x88', '\x00'};
enum ws_decode_result {
  FRAME_OK, FRAME_INCOMPLETE, FRAME_CLOSE, FRAME_ERROR
};
static void generate_accept_string(const std::string& client_key, char (*buffer)[ACCEPT_KEY_LENGTH]) {
    // Magic string from websockets spec.
    static const char ws_magic[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::string input(client_key + ws_magic);
    char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(&input[0]), input.size(), reinterpret_cast<unsigned char*>(hash));
    base64_encode(hash, sizeof(hash), *buffer, sizeof(*buffer));
}
// Constants for hybi-10 frame format.

typedef int OpCode;

const OpCode kOpCodeContinuation = 0x0;
const OpCode kOpCodeText = 0x1;
const OpCode kOpCodeBinary = 0x2;
const OpCode kOpCodeClose = 0x8;
const OpCode kOpCodePing = 0x9;
const OpCode kOpCodePong = 0xA;

const unsigned char kFinalBit = 0x80;
const unsigned char kReserved1Bit = 0x40;
const unsigned char kReserved2Bit = 0x20;
const unsigned char kReserved3Bit = 0x10;
const unsigned char kOpCodeMask = 0xF;
const unsigned char kMaskBit = 0x80;
const unsigned char kPayloadLengthMask = 0x7F;

const size_t kMaxSingleBytePayloadLength = 125;
const size_t kTwoBytePayloadLengthField = 126;
const size_t kEightBytePayloadLengthField = 127;
const size_t kMaskingKeyWidthInBytes = 4;

std::string SEC_WEBSOCKET_PROTOCOL_HEADER = "Sec-WebSocket-Protocol";
std::string SEC_WEBSOCKET_PROTOCOL_HEADER_LOWER_CASE = "sec-websocket-protocol";
std::string SEC_WEBSOCKET_KEY_HEADER = "Sec-WebSocket-Key";
std::string SEC_WEBSOCKET_KEY_HEADER_LOWER_CASE = "sec-websocket-key";
std::string SEC_WEBSOCKET_ACCEPT_HEADER = "Sec-WebSocket-Accept";
std::string SEC_WEBSOCKET_PROTOCOL_HEADER_ADD = "Sec-WebSocket-Protocol: ";


std::vector<char> accept_upgrade_response(const std::string& accept_key, const std::string& accept_protocol){
    char accept_string[ACCEPT_KEY_LENGTH];
    generate_accept_string(accept_key, &accept_string);
    const char accept_ws_prefix[] = "HTTP/1.1 101 Switching Protocols\r\n"
                                "Upgrade: websocket\r\n"
                                "Connection: Upgrade\r\n"
                                "Sec-WebSocket-Accept: ";
    const char accept_ws_suffix[] = "\r\n\r\n";
    const char line_suffix[] = "\r\n";
    std::vector<char> reply(accept_ws_prefix, accept_ws_prefix + sizeof(accept_ws_prefix) - 1);
    reply.insert(reply.end(), accept_string, accept_string + sizeof(accept_string));
    // 检查 Sec-WebSocket-Protocol
    if(!accept_protocol.empty()){
        reply.insert(reply.end(), line_suffix, line_suffix + sizeof(line_suffix) - 1);
        reply.insert(reply.end(), SEC_WEBSOCKET_PROTOCOL_HEADER_ADD.c_str(), SEC_WEBSOCKET_PROTOCOL_HEADER_ADD.c_str() + SEC_WEBSOCKET_PROTOCOL_HEADER_ADD.length());
        reply.insert(reply.end(), accept_protocol.c_str(), accept_protocol.c_str() + accept_protocol.length());
    }
    reply.insert(reply.end(), accept_ws_suffix, accept_ws_suffix + sizeof(accept_ws_suffix) -1);
    return reply;
}
void encode_ws_frame_head(std::vector<char>& frame, size_t data_length, bool client_frame){
    OpCode op_code = kOpCodeBinary;
    frame.push_back(kFinalBit | op_code);
    if (data_length <= kMaxSingleBytePayloadLength) {
        unsigned char second_byte = static_cast<unsigned char>(data_length);
        if(client_frame){
            second_byte |= kMaskBit;
        }
        frame.push_back(static_cast<char>(second_byte));
    } else if (data_length <= 0xFFFF) {
        unsigned char second_byte = static_cast<unsigned char>(kTwoBytePayloadLengthField);
        if(client_frame){
            second_byte |= kMaskBit;
        }
        frame.push_back(static_cast<char>(second_byte));
        frame.push_back((data_length & 0xFF00) >> 8);
        frame.push_back(data_length & 0xFF);
    } else {
        unsigned char second_byte = static_cast<unsigned char>(kEightBytePayloadLengthField);
        if(client_frame){
            second_byte |= kMaskBit;
        }
        frame.push_back(static_cast<char>(second_byte));
        char extended_payload_length[8];
        size_t remaining = data_length;
        // Fill the length into extended_payload_length in the network byte order.
        for (int i = 0; i < 8; ++i) {
            extended_payload_length[7 - i] = remaining & 0xFF;
            remaining >>= 8;
        }
        frame.insert(frame.end(), extended_payload_length, extended_payload_length + 8);
        CHECK_EQ(0, remaining);
    }
}
void encode_ws_frame_body(std::vector<char>& frame, const char* ptr, size_t data_length, bool client_frame){
    encode_ws_frame_head(frame, data_length, client_frame);
    if(client_frame){
//        unsigned int current_time = (unsigned int)time(NULL);
        unsigned int current_time = (unsigned int)get_time_seed();
        char* masking_key = (char*)(void*)(&current_time);
        frame.insert(frame.end(), masking_key, masking_key + kMaskingKeyWidthInBytes);
//        LOG_DEBUG("masking_key 0x%x 0x%x 0x%x 0x%x", (int)masking_key[0], (int)masking_key[1], (int)masking_key[2], (int)masking_key[3]);
        for (size_t i = 0; i < data_length; ++i){
            frame.insert(frame.end(), ptr[i] ^ masking_key[i % kMaskingKeyWidthInBytes]);
//            LOG_DEBUG("encode i=%d c=0x%x", (int)i, (int)ptr[i]);
        }  // Unmask the payload.
    }else{
        frame.insert(frame.end(), ptr, ptr + data_length);
    }
}
ws_decode_result decode_frame(const std::vector<char>& buffer, bool client_frame, int* bytes_consumed, std::vector<char>* output, bool* compressed){
    *bytes_consumed = 0;
    if (buffer.size() < 2){
        return FRAME_INCOMPLETE;
    }

    auto it = buffer.begin();

    unsigned char first_byte = *it++;
    unsigned char second_byte = *it++;

    bool final = (first_byte & kFinalBit) != 0;
    bool reserved1 = (first_byte & kReserved1Bit) != 0;
    bool reserved2 = (first_byte & kReserved2Bit) != 0;
    bool reserved3 = (first_byte & kReserved3Bit) != 0;
    int op_code = first_byte & kOpCodeMask;
    bool masked = (second_byte & kMaskBit) != 0;
    *compressed = reserved1;
    if (!final || reserved2 || reserved3){
        return FRAME_ERROR;  // Only compression extension is supported.
    }

    bool closed = false;
    switch (op_code) {
        case kOpCodeClose:
            closed = true;
            break;
        case kOpCodeText:
            break;
        case kOpCodeBinary:
            break;
        case kOpCodeContinuation:  // We don't support binary frames yet.
        case kOpCodePing:          // We don't support binary frames yet.
        case kOpCodePong:          // We don't support binary frames yet.
        default:
            LOG_ERROR("We don't support binary frames yet op_code=0x%x", op_code);
            return FRAME_ERROR;
    }

    // In Hybi-17 spec client MUST mask its frame.
    if (client_frame && !masked) {
        return FRAME_ERROR;
    }

    uint64_t payload_length64 = second_byte & kPayloadLengthMask;
    if (payload_length64 > kMaxSingleBytePayloadLength) {
        int extended_payload_length_size;
        if (payload_length64 == kTwoBytePayloadLengthField) {
            extended_payload_length_size = 2;
        } else if (payload_length64 == kEightBytePayloadLengthField) {
            extended_payload_length_size = 8;
        } else {
            return FRAME_ERROR;
        }
        if ((buffer.end() - it) < extended_payload_length_size){
            return FRAME_INCOMPLETE;
        }
        payload_length64 = 0;
        for (int i = 0; i < extended_payload_length_size; ++i) {
            payload_length64 <<= 8;
            payload_length64 |= static_cast<unsigned char>(*it++);
        }
    }

    static const uint64_t max_payload_length = 0x7FFFFFFFFFFFFFFFull;
    static const size_t max_length = SIZE_MAX;

    if(client_frame){
        if (payload_length64 > max_payload_length || payload_length64 > max_length - kMaskingKeyWidthInBytes) {
            // WebSocket frame length too large.
            return FRAME_ERROR;
        }
        size_t payload_length = static_cast<size_t>(payload_length64);

        if (buffer.size() - kMaskingKeyWidthInBytes < payload_length){
            return FRAME_INCOMPLETE;
        }
        std::vector<char>::const_iterator masking_key = it;
        std::vector<char>::const_iterator payload = it + kMaskingKeyWidthInBytes;
//        LOG_DEBUG("masking_key 0x%x 0x%x 0x%x 0x%x", (int)masking_key[0], (int)masking_key[1], (int)masking_key[2], (int)masking_key[3]);
        for (size_t i = 0; i < payload_length; ++i){
            output->insert(output->end(), payload[i] ^ masking_key[i % kMaskingKeyWidthInBytes]);
//            LOG_DEBUG("decode i=%d c=0x%x", (int)i, (int)(payload[i] ^ masking_key[i % kMaskingKeyWidthInBytes]));
        }  // Unmask the payload.

        size_t pos = it + kMaskingKeyWidthInBytes + payload_length - buffer.begin();
        *bytes_consumed = pos;
    }else{
        if (payload_length64 > max_payload_length || payload_length64 > max_length) {
            // WebSocket frame length too large.
            return FRAME_ERROR;
        }
        size_t payload_length = static_cast<size_t>(payload_length64);

        if (buffer.size() < payload_length){
            return FRAME_INCOMPLETE;
        }
        output->insert(output->end(), it, it + payload_length);
        size_t pos = it + payload_length - buffer.begin();
        *bytes_consumed = pos;
    }
    return closed ? FRAME_CLOSE : FRAME_OK;
}
inline int parse_ws_frames(std::function<void(std::vector<char>&)> handler, bool client_frame, const std::vector<char>& buffer, int& result){
    int bytes_consumed = 0;
    std::vector<char> output;
    bool compressed = false;
    ws_decode_result r =  decode_frame(buffer, client_frame, &bytes_consumed, &output, &compressed);
    // Compressed frame means client is ignoring the headers and misbehaves
    if (compressed || r == FRAME_ERROR) {
        LOG_ERROR("compressed=%d || r(%d) == FRAME_ERROR(%d)", compressed, r, FRAME_ERROR);
        bytes_consumed = 0;
        result = -1;
    } else if (r == FRAME_CLOSE) {
        bytes_consumed = 0;
        result = -1;
    } else if (r == FRAME_OK) {
        handler(output);
    }
    return bytes_consumed;
}
int loop_parse_ws_frames(std::function<void(std::vector<char>&)> handler, std::function<void(size_t)> moveOffset, bool client_frame, std::vector<char>& buffer){
    int processed = 0;
    int result = 0;
    do {
        processed = parse_ws_frames(handler, client_frame, buffer, result);
        // 2. Fix the data size & length
        if (processed > 0) {
            buffer.erase(buffer.begin(), buffer.begin() + processed);
            moveOffset(buffer.size());
        }
    } while (processed > 0 && !buffer.empty());
    return result;
}

unsigned char ToHex(unsigned char x)
{
    return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}

std::string url_encode(const char* str, unsigned int length)
{
    std::string strTemp = "";
    for (size_t i = 0; i < (size_t)length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

std::string url_decode(const char* str, unsigned int length)
{
    std::string strTemp = "";
    for (size_t i = 0; i < (size_t)length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            assert(i + 2 < (size_t)length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high*16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}

NS_HIVE_END
