#include "server.h"
#include "sync.h"
#include <stdio.h>

int main() {
    if (!load_config("src/config.txt")) {
        fprintf(stderr, "Failed to load configuration\n");
        return 1;
    }else{
        printf("loaded the config file....\n");  // Debug statement
    }
    start_sync_server();
    start_client_server();
    return 0;
}
