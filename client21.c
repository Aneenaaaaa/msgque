#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_UDP 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int udp_sock, num;
    struct sockaddr_in server_addr;

    // Create UDP socket
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_UDP);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Input integer from user
    printf("Enter an integer to send: ");
    scanf("%d", &num);

    // Send integer to server via UDP
    sendto(udp_sock, &num, sizeof(int), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Integer sent to server via UDP: %d\n", num);

    // Close socket
    close(udp_sock);
    return 0;
}
