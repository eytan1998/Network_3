#include <sys/time.h>
#include "helper.h"
#include "dinamic_array.h"

int main() {
    //establish connection
    int receiver_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (receiver_socket == ERROR) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP Receiver socket created.\n");

    struct sockaddr_in receiver_addr, client_addr;
    memset(&receiver_addr, '\0', sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = PORT;
    receiver_addr.sin_addr.s_addr = inet_addr(IP);
    socklen_t addr_size = sizeof(client_addr);

    if (bind(receiver_socket, (struct sockaddr *) &receiver_addr, sizeof(receiver_addr)) == ERROR) {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to: %s:%d\n", IP, PORT);

    if (listen(receiver_socket, 5) == ERROR) {
        perror("[-]Listen error");
        exit(1);
    }
    printf("Listening...\n");

    int connection_socket = accept(receiver_socket, (struct sockaddr *) &client_addr, &addr_size);
    printf("[+]Sender connected.\n");

    //get half file size from sender
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    recv(connection_socket, buffer, BUFFER_SIZE, 0);
    int receiveSize = (int) ntohl(*(int *) buffer);
    printf("Received file size is: %d bytes, (~%d MB)\n", receiveSize, receiveSize / 1000000);


    Array dinamic_arr;
    initArray(&dinamic_arr, 4);

    double firstTime;
    double secondTime;
    struct timeval start, end;

    while (1) {
        //sending first part

        gettimeofday(&start, NULL);
        receiveFile(connection_socket, buffer, receiveSize / 2  );
        gettimeofday(&end, NULL);


        //calculate first part
        firstTime = (double )(end.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
        firstTime += (double )(end.tv_usec - start.tv_usec) / 1000.0;   // us to ms

        printf("[+]Receiver got first part.\n");

        //sending authentication
        char *auth = xor16way(ID1, ID2);
        send(connection_socket, auth, ID_LENGTH, 0);
        free(auth);
        printf("[+]Receiver sent auth.\n");

        //change cc Algo
        changeCC(receiver_socket, RENO);

        //sending second part
        gettimeofday(&start, NULL);
        receiveFile(connection_socket, buffer, receiveSize / 2+ ((receiveSize%2==0)?0:1));
        gettimeofday(&end, NULL);


        //calculate second part
        secondTime = (double )(end.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
        secondTime += (double )(end.tv_usec - start.tv_usec) / 1000.0;   // us to ms

        printf("[+]Receiver got second part.\n");

        //save times
        insertArray(&dinamic_arr, firstTime, secondTime);

        //get "user input"
        bzero(buffer, BUFFER_SIZE);
        recv(connection_socket, buffer, strlen("exit"), 0);
        if (strcmp(buffer, "exit") == 0) {
            printf("[+]User choose to exit.\n");
            break;
        }
        printf("------------------------.\n");
        printf("[+]User choose send files again.\n");
        changeCC(connection_socket, CUBIC);
    }


    //exit, print times
    printTimes(&dinamic_arr);
    printf("\nFirst part average: %f millisecond\n",
           firstElementsAverage(&dinamic_arr));
    printf("Second part average: %f millisecond\n",
           secondElementsAverage(&dinamic_arr));
    printf("Average time of both parts: %f millisecond\n",
           allElementsAverage(&dinamic_arr));


    freeArray(&dinamic_arr);
    close(receiver_socket);
    printf("[+]Socket closed.\n");

    return 0;
}

