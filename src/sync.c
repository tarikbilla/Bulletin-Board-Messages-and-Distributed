#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sync.h"
#include "server.h"

extern int sp;
extern char peers[10][256];
extern int peer_count;

void *sync_handler(void *arg) {
    printf("Handling sync connection...\n");  // Debug statement
    int sync_socket = *(int*)arg;
    free(arg);
    char buffer[1024];

    while (1) {
        int bytes_received = recv(sync_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            close(sync_socket);
            break;
        }

        buffer[bytes_received] = '\0';
        if (strncmp(buffer, "precommit", 9) == 0) {
            send(sync_socket, "positive\n", strlen("positive\n"), 0);
        } else if (strncmp(buffer, "commit", 6) == 0) {
            // Handle commit
            send(sync_socket, "positive\n", strlen("positive\n"), 0);
        } else {
            send(sync_socket, "negative\n", strlen("negative\n"), 0);
        }
    }

    printf("Sync handler finished\n");  // Debug statement
    return NULL;
}

void start_sync_server() {
    printf("Starting sync server...\n");  // Debug statement
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(sp);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(1);
    }

    printf("Sync server listening on port %d\n", sp);  // Debug statement

    while (1) {
        int *sync_socket = malloc(sizeof(int));
        *sync_socket = accept(server_socket, NULL, NULL);
        if (*sync_socket == -1) {
            perror("Client connection failed");
            free(sync_socket);
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, sync_handler, sync_socket);
    }

    close(server_socket);
}

void broadcast_precommit(char *message) {
    printf("Broadcasting precommit message...\n");  // Debug statement
    for (int i = 0; i < peer_count; i++) {
        char *peer = peers[i];
        char host[256];
        int port;
        sscanf(peer, "%[^:]:%d", host, &port);

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            perror("Socket creation failed");
            continue;
        }

        struct sockaddr_in peer_address;
        peer_address.sin_family = AF_INET;
        peer_address.sin_port = htons(port);
        inet_pton(AF_INET, host, &peer_address.sin_addr);

        if (connect(sock, (struct sockaddr*)&peer_address, sizeof(peer_address)) == -1) {
            perror("Connection failed");
            close(sock);
            continue;
        }

        send(sock, "precommit", strlen("precommit"), 0);
        char response[1024];
        recv(sock, response, sizeof(response), 0);
        if (strncmp(response, "negative", 8) == 0) {
            printf("Precommit failed for peer %s\n", peer);
        }

        close(sock);
    }
    printf("Precommit broadcast finished\n");  // Debug statement
}

void broadcast_commit(char *message) {
    printf("Broadcasting commit message...\n");  // Debug statement
    for (int i = 0; i < peer_count; i++) {
        char *peer = peers[i];
        char host[256];
        int port;
        sscanf(peer, "%[^:]:%d", host, &port);

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            perror("Socket creation failed");
            continue;
        }

        struct sockaddr_in peer_address;
        peer_address.sin_family = AF_INET;
        peer_address.sin_port = htons(port);
        inet_pton(AF_INET, host, &peer_address.sin_addr);

        if (connect(sock, (struct sockaddr*)&peer_address, sizeof(peer_address)) == -1) {
            perror("Connection failed");
            close(sock);
            continue;
        }

        send(sock, "commit", strlen("commit"), 0);
        char response[1024];
        recv(sock, response, sizeof(response), 0);
        if (strncmp(response, "negative", 8) == 0) {
            printf("Commit failed for peer %s\n", peer);
        }

        close(sock);
    }
    printf("Commit broadcast finished\n");  // Debug statement
}
