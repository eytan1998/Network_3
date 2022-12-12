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

    //sending first part

    char buffer[SIZE];
    step3:;// if user want to send again
    clock_t start = clock();
    recv(client_sock, buffer, SIZE, 0);
    clock_t end = clock();
    float firstTime = (float) (end - start) / CLOCKS_PER_SEC;
    printf("[+]Receiver got first part.\n");

    //sending authentication
    send(client_sock, AUTH, strlen(AUTH), 0);//TODO

    //change cc Algo
    changeCC(server_sock, RENO);

    //sending second part
    bzero(buffer, SIZE);
    start = clock();
    recv(client_sock, buffer, SIZE, 0);//TODO
    end = clock();
    float secondTime = (float) (end - start) / CLOCKS_PER_SEC;
    printf("[+]Receiver got second part.\n");

    //get "user input"
    bzero(buffer, SIZE);
    recv(client_sock, buffer, strlen("exit"), 0);
    if (strcmp(buffer, "exit") != 0) {
        printf("Receiving files again.\n");
        changeCC(client_sock, CUBIC);
        goto step3;
    }
    //exit
    printf("[+]Received exit massage.\n");
    printf("FirstTime: %f\n", firstTime);
    printf("SecondTime: %f\n", secondTime);
    printf("AverageTime: %f\n", (firstTime + secondTime) / 2);
    close(client_sock);
    close(server_sock);
    printf("[+]Socket closed.\n");

    return 0;
}