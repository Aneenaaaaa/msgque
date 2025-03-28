#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_TCP 9090
#define SERVER_IP "127.0.0.1"

int main() {
    int tcp_sock, squared_num;
    struct sockaddr_in server_addr;

    // Create TCP socket
    tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_TCP);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(tcp_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("TCP connection failed");
        exit(EXIT_FAILURE);
    }

    // Receive squared integer from server
    read(tcp_sock, &squared_num, sizeof(int));
    printf("Squared integer received from server: %d\n", squared_num);

    // Close socket
    close(tcp_sock);
    return 0;
}
