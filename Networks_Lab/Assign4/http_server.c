#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFSIZE 8196

int main(int argc, char *argv[]) {
    int server_socket, client_socket, read_size;
    struct sockaddr_in server, client;
    char client_message[BUFSIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        // print the error message
        perror("bind failed. Error");
        return 1;
    }

    puts("bind done");

    // Listen
    listen(server_socket, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    int c = sizeof(struct sockaddr_in);

    // accept connection from an incoming client
    client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_socket < 0) {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    // Receive a message from client
    memset(client_message, 0, BUFSIZE);
    read_size = recv(client_socket, client_message, BUFSIZE, 0);

    // Check if it's a GET request
    if (strncmp(client_message, "GET /", 5) == 0) {
        char *file_name = client_message + 5;
        char *end = strstr(file_name, " HTTP/");
        if (end != NULL) {
            *end = '\0';
        }

        // Open the file requested by the client
        FILE *fp = fopen(file_name, "r");
        if (fp != NULL) {
            char buffer[BUFSIZE];
            int length;

            // Send HTTP header
            sprintf(buffer, "HTTP/1.1 200 OK\r\n");
            send(client_socket, buffer, strlen(buffer), 0);
            sprintf(buffer, "Content-Type: application/octet-stream\r\n");
            send(client_socket, buffer, strlen(buffer), 0);
            sprintf(buffer, "\r\n");
            send(client_socket, buffer, strlen(buffer), 0);

            // Send the file contents
            while ((length = fread(buffer, 1, BUFSIZE, fp)) > 0)
                        {
                send(client_socket, buffer, length, 0);
            }

            // Close file and client socket
            fclose(fp);
        } else {
            // If file not found, send 404 error
            char buffer[BUFSIZE];
            sprintf(buffer, "HTTP/1.1 404 Not Found\r\n");
            send(client_socket, buffer, strlen(buffer), 0);
            sprintf(buffer, "Content-Type: text/html\r\n");
            send(client_socket, buffer, strlen(buffer), 0);
            sprintf(buffer, "\r\n");
            send(client_socket, buffer, strlen(buffer), 0);
            sprintf(buffer, "<html><body><h1>404 Not Found</h1></body></html>\r\n");
            send(client_socket, buffer, strlen(buffer), 0);
        }
    }

    // Close the client socket
    close(client_socket);

    return 0;
}
