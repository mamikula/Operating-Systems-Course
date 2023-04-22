#include <sys/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include "header.h"

client clients_queues[MAX_CLIENTS];
int server_queue;
void handle_message(message *msg);
void stop_client(int client_id);

void init_client(message *msg){
    int q_id = msg->q_id;
    pid_t client_pid = msg->sender_pid;
    int id;

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_queues[i].active == false){
            id = i;
            break;
        }
        id = -111;
    }
    if(id == -111){
        printf("No space for another client!\n");
        exit(3);
    }

    client c = {
            .client_queue = q_id,
            .client_pid = client_pid,
            .active = true
    };
    clients_queues[id] = c;
    printf("[SERVER] Client with id: %d, qid: %d created!\n", id, q_id);

    message response = {
            .type = INIT,
            .sender_id = id
    };
    send_message(q_id, &response);
}

void init_clients_queue_arr(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        client c = {c.active = false};
        clients_queues[i] = c;
    }
}

void stop_server(){
    printf("[SERVER] Stopping server\n");

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_queues[i].active){
            printf("[SERVER] Sending STOP to client id: %d, qid: %d\n", i, clients_queues[i].client_queue);
            kill(clients_queues[i].client_pid, SIGINT);
            stop_client(i);
        }
    }
    delete_queue(server_queue);
    printf("\n SERVER STOPPED \n");
}

void stop_client(int client_id){
    if(clients_queues[client_id].active){
        printf("[SERVER] Disconnecting client: %d from queue\n", client_id);
        message msg = {.type = STOP};
        send_message(clients_queues[client_id].client_queue, &msg);
        client c = {.active = false};
        clients_queues[client_id] = c;
    }
}

void list_clients(int client_id){
    printf("\nCLIENTS:\n");
    char buff[MAX_LEN];
    char temp[32];
    buff[0] = '\0';

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_queues[i].active){
            printf("Client id: %d, qid: %d\n", i, clients_queues[client_id].client_queue);
            sprintf(temp, "%d\n", i);
            strcat(buff, temp);
        }
    }
    printf("\n END OF LIST\n");

    message msg = {
            .type = LIST
    };

    strcpy(msg.text, buff);
    send_message(clients_queues[client_id].client_queue, &msg);
}

void send_to_one(int sender_id, int receiver_id, char* text) {
    if (clients_queues[receiver_id].active == false) {
        printf("[SERVER] Client id: %d doesnt exist, cannot send him message\n", receiver_id);
        exit(11);
    }

    message msg = {
            .type = ONE,
            .sender_id = sender_id,
            .send_time = time(NULL)
    };
    strcpy(msg.text, text);
    send_message(clients_queues[receiver_id].client_queue, &msg);
}

void send_to_all(int sender_id, char* text){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_queues[i].active && i != sender_id){
            send_to_one(sender_id, i, text);
        }
    }
}

void save_msg(message *msg){ //troche zmienic
    FILE *f_ptr = fopen("result.txt", "a");
    if(!f_ptr){
        printf("open file error\n");
        exit(20);
    }

    time_t time_now = time(NULL);
    struct tm *local_time = localtime(&time_now);

    fprintf(f_ptr, "%d-%02d-%02d %02d:%02d:%02d\nClient id: %d\nMessage type: %ld\nMessage body:\n'%s'\n\n",
            local_time->tm_year + 1900,
            local_time->tm_mon + 1,
            local_time->tm_mday,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec,
            msg->sender_id,
            msg->type,
            msg->text
    );
    fclose(f_ptr);
}

void handle_msg(message* msg){
    save_msg(msg);

    switch (msg->type) {
        case STOP:
            printf("[SERVER] STOP received\n");
            stop_client(msg->sender_id);
            break;
        case LIST:
            printf("[SERVER] LIST received\n");
            list_clients(msg->sender_id);
            break;
        case INIT:
            printf("[SERVER] INIT received\n");
            init_client(msg);
            break;
        case ALL:
            printf("[SERVER] 2ALL received\n");
            send_to_all(msg->sender_id, msg->text);
            break;
        case ONE:
            printf("[SERVER] 2ONE received\n");
            send_to_one(msg->sender_id, msg->receiver_id, msg->text);
            break;
        default:
            printf("[SERVER] Wrong message!\n");
            exit(6);
    }
}

void handle_sigint(){
    printf("[SERVER] I received signal: SIGINT\n");
    exit(0);
}

int main(){
    printf("\n[SERVER] Started running...\n");

    atexit(stop_server);

    key_t key = ftok(home_path(), ID); //ftok - convert a pathname and a project identifier to a System V IPC key

    if(key == -1){
        printf("Key generate error\n");
        return 1;
    }

    server_queue = msgget(key, IPC_EXCL | IPC_CREAT | 0666); //msgget - get a System V message queue identifier

    if(server_queue == -1){

        if (errno == EEXIST){
            printf("Queue already exist\n");
            server_queue = msgget(key,  0666);
        }else{
            printf("Server queue create error\n");
            return 2;
        }

    }

    signal(SIGINT, handle_sigint);
    printf("server queue id: %d\n", server_queue);
    init_clients_queue_arr();

    message msg;
    while(true){
        int msgsz = MSG_SIZE;
        if(msgrcv(server_queue, &msg, msgsz, -INIT-1, 0) == -1){
            printf("Receive msg error");
            return 4;
        }
        handle_msg(&msg);
    }

    return 0;
}

























