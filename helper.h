

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

char * xoring(const char *a, const char* b);

