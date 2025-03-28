#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_SIZE 100

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    // Creating socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 addresses from text to binary
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    int arr[MAX_SIZE], n;

    // Input array size
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Input array elements
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Send array size to server
    send(sock, &n, sizeof(int), 0);

    // Send array to server
    send(sock, arr, n * sizeof(int), 0);

    // Receive results from server
    int max, min;
    float avg;
    read(sock, &max, sizeof(int));
    read(sock, &min, sizeof(int));
    read(sock, &avg, sizeof(float));

    // Display results
    printf("\nResults received from server:\n");
    printf("Maximum: %d\n", max);
    printf("Minimum: %d\n", min);
    printf("Average: %.2f\n", avg);

    // Close the socket
    close(sock);

    return 0;
}
