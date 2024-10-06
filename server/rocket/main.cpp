
#include "common.h"
#include "net.h"
#include "main_handler.h"
#include "rocket_handler.h"
#include "level.h"

USING_NS_HIVE;

void parseArgs(int argc, char *argv[], std::string& ip, uint16& port, uint16& https_port,
    int& log_level, int& thread_number, int& sb_slot_number, std::string& private_key, std::string& public_pem,
    std::string& module_name, std::string& db_name){
    for(int i=1; i<argc; ++i){
        if(strcmp(argv[i], "-h") == 0){
            ++i;
            ip = std::string(argv[i]);
        }
        else if(strcmp(argv[i], "-p") == 0){
            ++i;
            port = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "-sslp") == 0){
            ++i;
            https_port = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "-l") == 0){
            ++i;
            log_level = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "-t") == 0){
            ++i;
            thread_number = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "-sn") == 0){
            ++i;
            sb_slot_number = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "-pem") == 0){
            ++i;
            public_pem = std::string(argv[i]);
        }
        else if(strcmp(argv[i], "-key") == 0){
            ++i;
            private_key = std::string(argv[i]);
        }
        else if(strcmp(argv[i], "-m") == 0){
            ++i;
            module_name = std::string(argv[i]);
        }
        else if(strcmp(argv[i], "-db") == 0){
            ++i;
            db_name = std::string(argv[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    daemon(1, 1);
	srand(time(NULL));
	// 使用默认的信号处理
	defaultSignalHandle();

    std::string ip = "0.0.0.0";
    std::string private_key;
    std::string public_pem;
    std::string script_file_name = "script.main";
    std::string module_name = "service";
    std::string db_name = "leveldb";
    std::string log_file_name = "rocket.log";
    uint16 port = 80;
    uint16 https_port = 443;
    int log_level = 0;
    int thread_number = 1;
    int sb_slot_number = 10;
    uint32 handle = 0;

	parseArgs(argc, argv, ip, port, https_port, log_level, thread_number, sb_slot_number, private_key, public_pem, module_name, db_name);
    setLogLevel(log_level);
    if(!module_name.empty()){
        log_file_name = module_name + ".log";
    }
	openCppLog(log_file_name.c_str(), false, true);

    if(script_file_name.empty()){
        LOG_ERROR("main script_file_name is empty");
        sleep(1);
        return 1;
    }
    if(module_name.empty()){
        LOG_ERROR("main module_name is empty like (-m service)");
        sleep(1);
        return 1;
    }

    if(!db_open(db_name)){
        LOG_ERROR("main db_open failed db_name=%s", db_name.c_str());
        sleep(1);
        return 1;
    }
    sb_create(sb_slot_number);

    RocketHandler::setScriptFileName(script_file_name);
    RocketHandler::setModuleName(module_name);
    std::vector<RocketHandler*> vecThread;
    for(int i=0; i<thread_number; ++i){
        RocketHandler* pHandler = new RocketHandler();
        vecThread.push_back(pHandler);
    }

    MainHandler mainHandler;
    mainHandler.initialize(vecThread, private_key, public_pem);
    if(!public_pem.empty()){
        handle = mainHandler.getManager()->openHttpsListener(ip, https_port);
        if(handle == 0){
            LOG_ERROR("main openHttpsListener failed ip=%s https_port=%d", ip.c_str(), https_port);
            sleep(1);
            return 1;
        }
    }
    handle = mainHandler.getManager()->openHttpListener(ip, port);
    if(handle == 0){
        LOG_ERROR("main openHttpListener failed ip=%s port=%d", ip.c_str(), port);
        sleep(1);
        return 1;
    }
    for(int i=0; i<thread_number; ++i){
        RocketHandler* pHandler = (RocketHandler*)vecThread[i];
        pHandler->startThread();
    }

    mainHandler.startLoop();

	return 0;
}























