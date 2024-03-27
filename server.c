#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char **argv) {

    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket == -1) {
        perror("[ServerError]: error in calling socket()");
        exit(1);
    }

    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    char server_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server_address.sin_addr), server_ip, INET_ADDRSTRLEN);

    printf("[ServerMessage]: server %s:%d\n", server_ip, ntohs(server_address.sin_port));

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("[ServerError]: bind() call failed");
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 5) == -1) {
        perror("[ServerError]: listen() call failed");
        close(server_socket);
        exit(1);
    }

    printf("The server is running. Waiting for client connection...\n");

    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int client_socket = accept(server_socket, (struct sockaddr *) &clientAddress, &clientAddressLen);
    if (client_socket < 0) {
        perror("[ServerError]: accept() call failed");
        exit(1);
    }

    printf("Client connected\n");

    while (1) {

        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytesRead = recv(client_socket, buffer, MAX_BUFFER_SIZE - 1, 0);
        if (bytesRead < 0) {
            perror("Error reading message");
            exit(EXIT_FAILURE);
        } else if (bytesRead == 0) {
            printf("The client closed the connection");
            break;
        }

        printf("[CLIENT]: %s\n", buffer);

        if (strcmp(buffer, "q\n") == 0) {
            printf("Quit");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
    }

    close(client_socket);
    close(server_socket);

    return 0;
}