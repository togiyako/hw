#include "server.h"
#include <sys/stat.h>

int main() {
    ServerConfig config = load_config("server.conf");

    mkdir(config.storage_dir, 0777); 

    struct Server server = server_Constructor(config, launch);

    server.launch(&server);
    
    return 0;
}