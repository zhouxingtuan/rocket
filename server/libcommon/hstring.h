
#ifndef __hive__string__
#define __hive__string__

#include <string>

NS_HIVE_BEGIN

class HString
{
public:
    char* m_pData;
    int m_length;

    HString(void) : m_pData(NULL), m_length(0) {}
    HString(HString& s) : m_pData(s.m_pData), m_length(s.m_length) {}
    HString(const HString& s) : m_pData(s.m_pData), m_length(s.m_length) {}
    HString(const char* ptr, int length) : m_pData((char*)ptr), m_length(length) {}
    HString(const std::string& s) : m_pData((char*)s.c_str()), m_length(s.length()) {}
    ~HString(void){}

	void replace(const char* ptr, int length){
		m_pData = (char*)ptr;
		m_length = length;
	}
    char* c_str(void){ return m_pData; }
    int length(void){ return m_length; }
    std::string toString(void) const { return std::string(m_pData, m_length); }

    bool operator==(const std::string& s) const {
        return (m_length == (int)s.length() && memcmp(m_pData, s.c_str(), m_length) == 0);
    }
    bool operator==(const HString& s) const {
        return (m_length == s.m_length && memcmp(m_pData, s.m_pData, m_length) == 0);
    }
    bool operator!=(const std::string& s) const {
        if(m_length != (int)s.length()){
            return true;
        }
        if(memcmp(m_pData, s.c_str(), m_length) == 0){
            return false;
        }
        return true;
    }
    bool operator!=(const HString& s) const {
        if(m_length != s.m_length){
            return true;
        }
        if(memcmp(m_pData, s.m_pData, m_length) == 0){
            return false;
        }
        return true;
    }
    bool operator<(const HString& s) const {
        int minLength = std::min(m_length, s.m_length);
        int compValue = memcmp(m_pData, s.m_pData, minLength);
        if(compValue < 0){
            return true;
        }
        if(compValue > 0){
            return false;
        }
        if(m_length == s.m_length){
            return false;
        }
        if(m_length < s.m_length){
            return true;
        }
        return false;
    }
    HString& operator=(const HString& s){
        m_pData = s.m_pData;
        m_length = s.m_length;
        return *this;
    }
};

NS_HIVE_END

// 间接调用原生哈希算法，这样HString就可以用作map，unordered_map的key
namespace std
{
    template<>
    struct hash<hive::HString>
    {
        size_t operator() (const hive::HString& s) const noexcept
        {
            return  hash<decltype(s.toString())>()(s.toString());

            //auto t = make_tuple(s.getName(), s.getPhone());
            //size_t value = boost::hash_value(t);
            //return value;     // make_tuple(s.getName(), s.getPhone()) 等价于 tuple<string, long long>()(s.getName(), s.getPhone())
        }
    }; // 间接调用原生Hash.
}

#endif
