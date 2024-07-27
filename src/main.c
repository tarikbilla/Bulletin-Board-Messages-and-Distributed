#include <stddef.h>
#include <pthread.h>
#include "server.h"
#include "sync.h"

int main() {
    if (!load_config("src/config.txt")) {
        return 1;
    }

    pthread_t client_server_thread;
    pthread_t sync_server_thread;

    pthread_create(&client_server_thread, NULL, (void*)start_client_server, NULL);
    pthread_create(&sync_server_thread, NULL, (void*)start_sync_server, NULL);

    pthread_join(client_server_thread, NULL);
    pthread_join(sync_server_thread, NULL);

    return 0;
}
