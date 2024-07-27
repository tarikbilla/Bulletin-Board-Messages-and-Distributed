#ifndef SYNC_H
#define SYNC_H

void *sync_handler(void *arg);
void start_sync_server();
void broadcast_precommit(char *message);
void broadcast_commit(char *message);

#endif // SYNC_H
