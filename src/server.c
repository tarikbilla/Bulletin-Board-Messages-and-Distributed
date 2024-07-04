#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include "server.h"
#include "sync.h"

int bp;  // Client communication port
int sp;  // Synchronization port
char bbfile[256];
int nthreads;
char peers[10][256];  // Array to store peer information
int peer_count = 0;

int load_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open config file");
        return 0; // Return 0 on failure
    }
    fscanf(file, "THMAX=%d\n", &nthreads);
    fscanf(file, "BBPORT=%d\n", &bp);
    fscanf(file, "SYNCPORT=%d\n", &sp);
    fscanf(file, "BBFILE=%s\n", bbfile);
    char peer_line[256];
    while (fscanf(file, "PEER=%s\n", peer_line) != EOF) {
        strcpy(peers[peer_count], peer_line);
        peer_count++;
    }
    fclose(file);
    return 1; // Return 1 on success
}

void *client_handler(void *arg) {
    int client_socket = *(int*)arg;
    free(arg);
    char buffer[1024];
    FILE *file = fopen(bbfile, "a+");

    send(client_socket, "0.0 greeting\n", strlen("0.0 greeting\n"), 0);

    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            close(client_socket);
            break;
        }

        buffer[bytes_received] = '\0';
        if (strncmp(buffer, "WRITE", 5) == 0 || strncmp(buffer, "REPLACE", 7) == 0) {
            handle_write_request(buffer);
            send(client_socket, "3.0 WROTE\n", strlen("3.0 WROTE\n"), 0);
        } else if (strncmp(buffer, "USER", 4) == 0 || strncmp(buffer, "READ", 4) == 0) {
            // Handle other requests
        } else {
            send(client_socket, "Invalid command\n", strlen("Invalid command\n"), 0);
        }
    }

    fclose(file);
    return NULL;
}

void start_client_server() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(bp);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(1);
    }

    while (1) {
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, NULL, NULL);
        if (*client_socket == -1) {
            perror("Client connection failed");
            free(client_socket);
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, client_handler, client_socket);
    }

    close(server_socket);
}
