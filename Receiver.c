#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include "helper.h"


int main() {

    //establish connection
    socklen_t addr_size;

    double firstTime;
    double secondTime;

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    addr_size = sizeof(client_addr);

    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to: %s:%d\n", IP, PORT);

    if (listen(server_sock, 5) == -1) {
        perror("[-]Listen error");
        exit(1);
    }
    printf("Listening...\n");

    int client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &addr_size);
    printf("[+]Sender connected.\n");

    //get half file BUFFER_SIZE

    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    recv(client_sock, buffer, BUFFER_SIZE, 0);
    int receiveSize = (int)ntohl(*(int *) buffer);
    int segment = receiveSize / BUFFER_SIZE + 1;
    printf("Received half file size is: %d bytes, (~%d MB)\n", receiveSize, receiveSize / 1000000);

    //sending first part
    int receiveSizeTmp;
    while (1) {
        receiveSizeTmp = receiveSize;
        clock_t start = clock();
        int counter = 1;

        while (1) {
            bzero(buffer, BUFFER_SIZE);
            int e = (int)recv(client_sock, buffer, BUFFER_SIZE, 0);
            printf("segment %d of %d (%d Bytes left)\n", counter, segment, receiveSizeTmp -= e);
            if (e <= 0 || counter++ >= segment) {
                break;
            }
        }
        clock_t end = clock();
        firstTime = (float) (end - start) / CLOCKS_PER_SEC;
        printf("[+]Receiver got first part.\n");

        //sending authentication
        char *auth = xoring(ID1, ID2);
        send(client_sock, auth, ID_LENGTH, 0);
        free(auth);
        printf("[+]Receiver sent auth.\n");

        //change cc Algo
        changeCC(server_sock, RENO);

        //sending second part
        start = clock();
        counter = 1;
        receiveSizeTmp = receiveSize;

        while (1) {
            bzero(buffer, BUFFER_SIZE);
            int e = (int)recv(client_sock, buffer, BUFFER_SIZE, 0);
            printf("segment %d of %d (%d Bytes left)\n", counter, segment, receiveSizeTmp -= e);

            if (e <= 0 || counter++ >= segment) {
                break;
            }
        }
        end = clock();
        secondTime = (float) (end - start) / CLOCKS_PER_SEC;
        printf("[+]Receiver got second part.\n");

        bzero(buffer, BUFFER_SIZE);
        recv(client_sock, buffer, strlen("exit"), 0);
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        printf("Receiving files again.\n");
        changeCC(client_sock, CUBIC);
    }
    //get "user input"


    //exit
    printf("[+]Received exit massage.\n");
    printf("FirstTime: %f\n", firstTime);
    printf("SecondTime: %f\n", secondTime);
    printf("AverageTime: %f\n", (firstTime + secondTime) / 2);

    close(server_sock);
    printf("[+]Socket closed.\n");

    return 0;
}