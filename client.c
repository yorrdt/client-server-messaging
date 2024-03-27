#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>


#define MAX_BUFFER_SIZE 1024

int main(int argc, char** argv) {

    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (client_socket == -1) {
        perror("[ClientError]: error in calling socket()");
        exit(1);
    }

    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("[ClientError]: connection error");
        close(client_socket);
        exit(1);
    }

    char buffer[MAX_BUFFER_SIZE];
    int shouldExit = 0;
    while(!shouldExit) {

        printf("> ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        if (strcmp(buffer, "q\n") == 0) {
            shouldExit = 1;
        }

        size_t message_length = strlen(buffer);
        if (buffer[message_length - 1] == '\n') {
            buffer[message_length - 1] = '\0';
        }

        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
    }

    close(client_socket);

    return 0;
}