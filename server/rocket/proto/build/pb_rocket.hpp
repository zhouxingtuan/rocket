#ifndef __pb_rocket_hpp__
#define __pb_rocket_hpp__

#include "eproto.hpp"

namespace rocket
{
    class empty : public eproto::Proto
    {
    public:
        empty() : eproto::Proto() {}
        virtual ~empty(){ Clear(); }
        virtual void Clear()
        {
        }
        virtual void Encode(eproto::Writer& wb)
        {
            wb.pack_array(0);
        }
        virtual void Decode(eproto::Reader& rb)
        {
            long long int c = rb.unpack_array();
            if (c <= 0) { return; }
            rb.unpack_discard(c);
        }
        virtual eproto::Proto* Create() { return empty::New(); }
        virtual void Destroy() { empty::Delete(this); }
        static empty* New() { empty* p = new empty(); p->retain(); return p; }
        static void Delete(empty* p) { if(NULL != p){ p->release(); } }
        virtual std::string ClassName() { return "rocket::empty"; }
    };
    class rpc : public eproto::Proto
    {
    public:
        unsigned int msg_id;
        unsigned int callback_id;
        int error_code;
        int command;
        unsigned int uid;
        std::vector<char> body;
        std::string msg;
        rpc() : eproto::Proto(), msg_id(0), callback_id(0), error_code(0), command(0), uid(0) {}
        virtual ~rpc(){ Clear(); }
        void body_Append(void* p, size_t len){ int offset=(int)this->body.size(); this->body.resize(offset+len); memcpy(this->body.data()+offset, (char*)p, len); }
        virtual void Clear()
        {
            this->msg_id = 0;
            this->callback_id = 0;
            this->error_code = 0;
            this->command = 0;
            this->uid = 0;
            this->body.clear();
            this->msg = "";
        }
        virtual void Encode(eproto::Writer& wb)
        {
            wb.pack_array(7);
            wb.pack_int(this->msg_id);
            wb.pack_int(this->callback_id);
            wb.pack_int(this->error_code);
            wb.pack_int(this->command);
            wb.pack_int(this->uid);
            wb.pack_bytes(this->body);
            wb.pack_string(this->msg);
        }
        virtual void Decode(eproto::Reader& rb)
        {
            long long int c = rb.unpack_array();
            if (c <= 0) { return; }
            rb.unpack_int(this->msg_id);
            if (--c <= 0) { return; }
            rb.unpack_int(this->callback_id);
            if (--c <= 0) { return; }
            rb.unpack_int(this->error_code);
            if (--c <= 0) { return; }
            rb.unpack_int(this->command);
            if (--c <= 0) { return; }
            rb.unpack_int(this->uid);
            if (--c <= 0) { return; }
            rb.unpack_bytes(this->body);
            if (--c <= 0) { return; }
            rb.unpack_string(this->msg);
            if (--c <= 0) { return; }
            rb.unpack_discard(c);
        }
        virtual eproto::Proto* Create() { return rpc::New(); }
        virtual void Destroy() { rpc::Delete(this); }
        static rpc* New() { rpc* p = new rpc(); p->retain(); return p; }
        static void Delete(rpc* p) { if(NULL != p){ p->release(); } }
        virtual std::string ClassName() { return "rocket::rpc"; }
    };

};


#endif
