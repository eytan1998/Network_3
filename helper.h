#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define ID1  "0001100011101101"
#define ID2  "0000110010001111"
#define ID_LENGTH 16

#define RENO "reno"
#define CUBIC "cubic"
#define BUFFER_SIZE 1024
#define PORT 5567
#define IP "127.0.0.1"


void changeCC(int socket,char* protocol);

char *readFile();

char * xor16way(const char *a, const char* b);

