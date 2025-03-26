
 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h> 
#include <sys/wait.h> 
 
#define SHM_SIZE 1024   
 
struct student { 
    char name[10]; 
    int rollno; 
    int marks; 
    int rank; 
}; 
 
// Function to sort students by marks and assign ranks 
void sort(struct student *stud, int n) { 
    for (int i = 0; i < n - 1; i++) { 
        for (int j = i + 1; j < n; j++) { 
            if (stud[i].marks < stud[j].marks) { 
                struct student temp = stud[i]; 
                stud[i] = stud[j]; 
                stud[j] = temp; 
            } 
        } 
    } 
    for (int i = 0; i < n; i++) { 
        stud[i].rank = i + 1; 
    } 
} 
 
int main() { 
    struct student *stud; 
    key_t key = ftok("shmfile", 65);  // Generate unique key 
 
    // Create shared memory 
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT); 
    if (shmid == -1) { 
        perror("shmget failed"); 
        return 1; 
    } 
 
    // Attach shared memory to parent process 
    stud = (struct student *)shmat(shmid, NULL, 0); 
    if (stud == (struct student *)(-1)) { 
        perror("shmat failed"); 
        return 1; 
    } 
 
    int n; 
    printf("Enter the number of students: "); 
    scanf("%d", &n); 
 
    printf("\nEnter student details:\n"); 
    for (int i = 0; i < n; i++) { 
        printf("Student %d Name: ", i + 1); 
        scanf("%s", stud[i].name); 
        printf("RollNo: "); 
        scanf("%d", &stud[i].rollno); 
        printf("Marks: "); 
        scanf("%d", &stud[i].marks); 
    } 
 
    // Detach shared memory in parent 
    shmdt(stud); 
 
    // Fork a child process 
    if (fork() == 0) { 
        sleep(2);  // Wait to ensure parent writes data 
 
        // Child attaches to the same shared memory 
        int shmid_read = shmget(key, SHM_SIZE, 0666); 
        if (shmid_read == -1) { 
            perror("shmget failed"); 
            return 1; 
        } 
 
        struct student *stud_read = (struct student *)shmat(shmid_read, NULL, 0); 
        if (stud_read == (struct student *)(-1)) { 
            perror("shmat failed"); 
            return 1; 
        } 
 
        // Sort and assign ranks 
        sort(stud_read, n); 
 
        printf("\n--- Student Rankings ---\n"); 
        for (int i = 0; i < n; i++) { 
            printf("Rank %d: %s - %d Marks\n", stud_read[i].rank, stud_read[i].name, 
stud_read[i].marks); 
        } 
 
        // Detach and delete shared memory in child 
        shmdt(stud_read); 
        shmctl(shmid_read, IPC_RMID, NULL);  // Delete shared memory 
        exit(0); 
    } 
 
    wait(NULL);  // Parent waits for child to complete 
    return 0; 
} 