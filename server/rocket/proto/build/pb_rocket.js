!function(t){
    t();
}(function () {
    var eproto = importModule("eproto");
    var pb_rocket={"rocket.rpc":[[4,0,"msg_id",0],[4,1,"callback_id",0],[4,2,"error_code",0],[4,3,"command",0],[4,4,"uid",0],[6,5,"body",0],[5,6,"msg",0]],"rocket.empty":[]};
    for(var name in pb_rocket){
        eproto.register(name, pb_rocket[name]);
    }
});
