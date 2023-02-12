#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFSIZE 8196

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char message[BUFSIZE], server_reply[BUFSIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    // Send a GET request
    char *file_name = argv[1];
    sprintf(message, "GET /%s HTTP/1.1\r\n\r\n", file_name);
    if (send(sock, message, strlen(message), 0) < 0) {
        puts("Send failed");
        return 1;
    }

    // Receive file contents
    FILE *fp = fopen(file_name, "w");
    if (fp != NULL) {
        int read_size;
        while ((read_size = recv(sock, server_reply, BUFSIZE, 0)) > 0) {
            fwrite(server_reply, 1, read_size, fp);
        }
        fclose(fp);
    }

    close(sock);
    return 0;
}