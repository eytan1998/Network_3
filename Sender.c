#include "helper.h"
#include "dinamic_array.h"

int main() {
    //1 read file
    char *text = readFile();

    //2 establish connection
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP socket created.\n");

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("[-] setsockopt(SO_REUSEADDR) failed");
    }

    struct sockaddr_in addr;
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == ERROR) {
        perror("[-]Connection error ");
        exit(1);
    }
    printf("[+]TCP socket connected.\n");

    //sending file size
    unsigned long un = strlen(text);
    printf("Sending file size is: %lu bytes, (~%lu MB)\n", strlen(text), strlen(text) / 1000000);
    send(sock, &un, sizeof(unsigned long), 0);


    //3 send first
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Sending first half of file...\n");
        send(sock, text, strlen(text) / 2, 0);
        printf("[+]First File Send.\n");

        //4 check auth
        bzero(buffer, BUFFER_SIZE);
        recv(sock, buffer, BUFFER_SIZE, 0);
        char *auth = xor16way(ID1, ID2);
        if (strncmp(buffer, auth, ID_LENGTH) == 0) {
            printf("[+]Authenticated\n");
        } else {
            printf("[-]Wrong authenticate: \"%s\"\n", buffer);
        }
        free(auth);

        //5 change CC algo
        changeCC(sock, RENO);

        //6 send second
        char *textTmp = (text + strlen(text) / 2);
        printf("Sending second half of file...\n");
        send(sock, textTmp, strlen(textTmp), 0);
        printf("[+]Second File Send.\n");

        //7 user choice
        char choice;
        printf("Send file again? [y/ any] ");
        scanf(" %c", &choice);
        if (choice != 'y') {
            send(sock, "exit", 4, 0);
            printf("Sending exit code...\n");
            break;
        }
        send(sock, "roll", 4, 0);
        printf("------------------------.\n");
        printf("Sending files again.\n");
        changeCC(sock, CUBIC);
    }
    //exit
    free(text);
    printf("[+]Disconnected from the receiver.\n");
    close(sock);
    return 0;
}