
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include "helper.h"

#define FILE_PATH "send.txt"

void changeCC(int socket, char *protocol) {
    if (setsockopt(socket, IPPROTO_TCP, TCP_CONGESTION, protocol, strlen(protocol)) != 0) {
        perror("[-]Can't change CC\n");
        return;
    }
    printf("[+]CC changed to: %s\n", protocol);

}

char *readFile() {

    char *text;

    FILE *fp = fopen(FILE_PATH, "rb");
    if (fp == NULL) perror("[-]Can't open file "), exit(1);

    fseek(fp, 0L, SEEK_END);
    long length = ftell(fp);
    rewind(fp);

/* allocate memory for entire content */
    text = calloc(1, length + 1);
    if (text == NULL) fclose(fp), fputs("[-]memory alloc fails", stderr), exit(1);

/* copy the file into the buffer */
    if (fread(text, length, 1, fp) != 1) fclose(fp), free(text), fputs("[-]entire read fails", stderr), exit(1);

    fclose(fp);
    return text;
}


char *xoring(const char *a, const char *b) {
    int length = ID_LENGTH+1;
    char *c = malloc(length);
    for (int i = 0; i < length-1; i++) {
        // If the Character matches
        if (a[i] == b[i])
            c[i] = '0';
        else
            c[i] = '1';
    }
    c[length-1] = '\0';
    return c;
}