#include "server.h"
#include <sys/stat.h>
#include <signal.h>
int main() {

    signal(SIGCHLD, SIG_IGN);

    ServerConfig config = load_config("server.conf");

    logger_init(config.log_file);

    mkdir(config.storage_dir, 0777); 

    struct Server server = server_Constructor(config, launch);

    LOG_DEBUG("Configuration loaded. Root: %s", config.root_dir);
    
    server.launch(&server);
    
    return 0;
}