library rocket;
import 'dart:typed_data';
import 'eproto.dart' as eproto;


class empty
{
  void encode(eproto.DataWriter wb)
  {
    wb.packArrayHead(0);
  }
  void decode(eproto.DataReader rb)
  {
    int c = rb.unpackArrayHead();
    if (c <= 0) { return; }
    rb.unpackDiscard(c);
  }
  empty create() { return empty(); }

}
class rpc
{
  int msg_id = 0;
  int callback_id = 0;
  int error_code = 0;
  int command = 0;
  int uid = 0;
  Uint8List body = Uint8List(0);
  String msg = "";
  void encode(eproto.DataWriter wb)
  {
    wb.packArrayHead(7);
    wb.packInt(this.msg_id);
    wb.packInt(this.callback_id);
    wb.packInt(this.error_code);
    wb.packInt(this.command);
    wb.packInt(this.uid);
    wb.packBytes(this.body);
    wb.packString(this.msg);
  }
  void decode(eproto.DataReader rb)
  {
    int c = rb.unpackArrayHead();
    if (c <= 0) { return; }
    this.msg_id = rb.unpackInt();
    if (--c <= 0) { return; }
    this.callback_id = rb.unpackInt();
    if (--c <= 0) { return; }
    this.error_code = rb.unpackInt();
    if (--c <= 0) { return; }
    this.command = rb.unpackInt();
    if (--c <= 0) { return; }
    this.uid = rb.unpackInt();
    if (--c <= 0) { return; }
    this.body = rb.unpackBytes();
    if (--c <= 0) { return; }
    this.msg = rb.unpackString();
    if (--c <= 0) { return; }
    rb.unpackDiscard(c);
  }
  rpc create() { return rpc(); }

}



