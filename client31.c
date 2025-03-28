#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_UDP 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int udp_sock;
    struct sockaddr_in server_addr;
    float value_to_send;

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

    // Input float value from user
    printf("Enter a float value to send: ");
    scanf("%f", &value_to_send);

    // Send float value to server via UDP
    sendto(udp_sock, &value_to_send, sizeof(float), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Float value sent to server via UDP: %.2f\n", value_to_send);

    // Close socket
    close(udp_sock);
    return 0;
}
