#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_SIZE 100

// Function to calculate max, min, and average
void calculateResults(int arr[], int n, int *max, int *min, float *avg) {
    *max = arr[0];
    *min = arr[0];
    int sum = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] > *max)
            *max = arr[i];
        if (arr[i] < *min)
            *min = arr[i];
        sum += arr[i];
    }
    *avg = (float)sum / n;
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1, addrlen = sizeof(address);

    // Creating the socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching the socket to PORT 8080
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listening for incoming connections
    listen(server_fd, 3);
    printf("Server is listening on port %d...\n", PORT);

    // Accept client connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    int arr[MAX_SIZE], n;
    // Read array size from client
    read(new_socket, &n, sizeof(int));

    // Read array elements from client
    read(new_socket, arr, n * sizeof(int));

    // Calculate max, min, and avg
    int max, min;
    float avg;
    calculateResults(arr, n, &max, &min, &avg);

    // Send results to the client
    write(new_socket, &max, sizeof(int));
    write(new_socket, &min, sizeof(int));
    write(new_socket, &avg, sizeof(float));

    printf("Results sent to the client!\n");

    // Close the socket
    close(new_socket);
    close(server_fd);

    return 0;
}
