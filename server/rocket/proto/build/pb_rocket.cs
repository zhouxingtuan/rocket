using System;
using System.Text;
using System.Collections.Generic;
using Erpc;

namespace rocket
{
    class rpc : Proto
    {
        public uint msg_id;
        public uint callback_id;
        public int error_code;
        public int command;
        public uint uid;
        public byte[] body;
        public string msg;
        override public void Encode(WriteBuffer wb)
        {
            Eproto.PackArray(wb, 7);
            Eproto.PackInteger(wb, this.msg_id);
            Eproto.PackInteger(wb, this.callback_id);
            Eproto.PackInteger(wb, this.error_code);
            Eproto.PackInteger(wb, this.command);
            Eproto.PackInteger(wb, this.uid);
            Eproto.PackBytes(wb, this.body);
            Eproto.PackString(wb, this.msg);
        }
        override public void Decode(ReadBuffer rb)
        {
            long c = Eproto.UnpackArray(rb);
            if (c <= 0) { return; }
            Eproto.UnpackInteger(rb, ref this.msg_id);
            if (--c <= 0) { return; }
            Eproto.UnpackInteger(rb, ref this.callback_id);
            if (--c <= 0) { return; }
            Eproto.UnpackInteger(rb, ref this.error_code);
            if (--c <= 0) { return; }
            Eproto.UnpackInteger(rb, ref this.command);
            if (--c <= 0) { return; }
            Eproto.UnpackInteger(rb, ref this.uid);
            if (--c <= 0) { return; }
            Eproto.UnpackBytes(rb, ref this.body);
            if (--c <= 0) { return; }
            Eproto.UnpackString(rb, ref this.msg);
            if (--c <= 0) { return; }
            Eproto.UnpackDiscard(rb, c);
        }
        override public Proto Create() { return new rpc(); }
    }
    class empty : Proto
    {
        override public void Encode(WriteBuffer wb)
        {
            Eproto.PackArray(wb, 0);
        }
        override public void Decode(ReadBuffer rb)
        {
            long c = Eproto.UnpackArray(rb);
            if (c <= 0) { return; }
            Eproto.UnpackDiscard(rb, c);
        }
        override public Proto Create() { return new empty(); }
    }

}
