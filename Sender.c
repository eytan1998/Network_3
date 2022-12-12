#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "helper.h"



int main() {
    //read file
    char *text = readFile();

    //establish connection
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP socket created.\n");

    struct sockaddr_in addr;
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = inet_addr(IP);

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("[-]Connection error ");
        exit(1);
    }
    printf("[+]TCP socket connected.\n");



    //send first
    char buffer[SIZE];
    step3:;
    send(sock, text, strlen(text) / 2, 0);
    printf("[+]First File Send.\n");


    //check auth
    bzero(buffer, SIZE);
    recv(sock, buffer, SIZE, 0);
    if (strcmp(buffer, AUTH) == 0) {
        printf("[+]Authenticated\n");
    } else
        printf("[-]Wrong authenticate: %s\n", buffer);

    //change CC algo
    changeCC(sock, RENO);

    //send second
    text += strlen(text) / 2;
    send(sock, text, strlen(text), 0);

    //user choice
    char choice;
    printf("Send file again? [y/ any]");
    scanf(" %c", &choice);
    if (choice == 'y') {
        printf("Sending files again.\n");

        changeCC(sock, CUBIC);
        goto step3;

    } else {
        send(sock, "exit", 4, 0);
        printf("Sent exit code.\n");
    }
    //exit
    close(sock);
    printf("[+]Disconnected from the receiver.\n");


    return 0;
}