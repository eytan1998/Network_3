#include "helper.h"

#define FILE_PATH "send.txt"

void receiveFile(int socket, char buffer[BUFFER_SIZE], int size) {
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        int bytesReceived = (int) recv(socket, buffer, BUFFER_SIZE, 0);
        size -= bytesReceived;
        if (bytesReceived == 0 || size <= 0) break;
        if (bytesReceived < 0) {
            printf("[-]Error while receiving.\n");
            break;
        }
    }
}

void changeCC(int socket, char *protocol) {
    if (setsockopt(socket, IPPROTO_TCP, TCP_CONGESTION, protocol, strlen(protocol)) != 0) {
        perror("[-]Can't change CC\n");
        return;
    }
    printf("[+]CC changed to: %s\n", protocol);
}

char* readFile() {

    char *text;

    FILE *fp = fopen(FILE_PATH, "rb");
    if (fp == NULL) perror("[-]Can't open file "), exit(1);

    fseek(fp, 0L, SEEK_END);
    long length = ftell(fp);
    rewind(fp);

    //allocate memory
    text = calloc(1, length + 1);
    if (text == NULL)fclose(fp), fputs("[-]memory alloc fails", stderr), exit(1);

    //copy the file into "text"
    if (fread(text, length, 1, fp) != 1) fclose(fp), free(text), fputs("[-]entire read fails", stderr), exit(1);

    fclose(fp);
    printf("[+]read file successfully.\n");
    return text;
}

char* xor16way(const char *a, const char *b) {
    int length = ID_LENGTH + 1;
    char *c = malloc(length);
    for (int i = 0; i < length - 1; i++) {
        // If the Character matches
        if (a[i] == b[i])
            c[i] = '0';
        else
            c[i] = '1';
    }
    c[length - 1] = '\0';
    return c;
}