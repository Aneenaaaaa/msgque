#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_UDP 8080
#define PORT_TCP 9090

int main() {
    int udp_sock, tcp_sock, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    float received_value, modified_value;

    // ============================
    // UDP SOCKET SETUP
    // ============================
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_UDP);

    // Bind UDP socket
    if (bind(udp_sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening for UDP float value on port %d...\n", PORT_UDP);

    // Receive float value from Client1 using UDP
    recvfrom(udp_sock, &received_value, sizeof(float), 0, (struct sockaddr *)&client_addr, &addr_len);
    printf("Value received from Client1: %.2f\n", received_value);

    // Increase the value by power of 1.5
    modified_value = pow(received_value, 1.5);
    printf("Modified value (raised to power 1.5): %.2f\n", modified_value);

    // ============================
    // TCP SOCKET SETUP
    // ============================
    tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_port = htons(PORT_TCP);

    // Bind TCP socket
    if (bind(tcp_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("TCP bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connection from Client2
    listen(tcp_sock, 3);
    printf("Server waiting for TCP connection on port %d...\n", PORT_TCP);

    // Accept connection from Client2
    new_socket = accept(tcp_sock, (struct sockaddr *)&client_addr, &addr_len);
    if (new_socket < 0) {
        perror("TCP accept failed");
        exit(EXIT_FAILURE);
    }

    // Send the modified value to Client2
    send(new_socket, &modified_value, sizeof(float), 0);
    printf("Modified value sent to Client2: %.2f\n", modified_value);

    // Close sockets
    close(udp_sock);
    close(new_socket);
    close(tcp_sock);

    return 0;
}
