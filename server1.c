#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_UDP 8080
#define PORT_TCP 9090
#define BUFFER_SIZE 1024

// Function to reverse a string
void reverseString(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

int main() {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int udp_sock, tcp_sock, new_socket, valread;
    
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

    printf("Server listening for UDP string on port %d...\n", PORT_UDP);

    // Receive string from Client1 using UDP
    recvfrom(udp_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
    printf("String received via UDP: %s\n", buffer);

    // Reverse the string
    reverseString(buffer);
    printf("Reversed string: %s\n", buffer);

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

    // Send the reversed string to Client2
    send(new_socket, buffer, strlen(buffer), 0);
    printf("Reversed string sent to Client2\n");

    // Close sockets
    close(udp_sock);
    close(new_socket);
    close(tcp_sock);

    return 0;
}
