#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>

struct shared_data {
    char selection[100];
    int balance;
};

int main() {
    int shm_id, pipe_fd[2];
    void *shared_mem;
    pid_t process_id;
    struct shared_data *bank_data;
    char message_buffer[200];
    
    shm_id = shmget((key_t)201, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if(shm_id == -1) {
        perror("shmget");
        exit(1);
    }
    
    shared_mem = shmat(shm_id, NULL, 0);
    if(shared_mem == (void *)-1) {
        perror("shmat");
        exit(1);
    }
    
    bank_data = (struct shared_data *)shared_mem;

    if(pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(1);
    }

    printf("Provide Your Input from Given Options:\n");
    printf("1. Type a to Add Money\n2. Type w to Withdraw Money\n3. Type c to Check Balance\n");
    read(0, bank_data->selection, sizeof(bank_data->selection));
    bank_data->balance = 1000;
    bank_data->selection[strcspn(bank_data->selection,"\n")] = 0;
    printf("Your selection: %s\n\n", bank_data->selection);

    process_id = fork();
    if(process_id < 0) {
        perror("fork");
        exit(1);
    }
    else if(process_id == 0) {
        if(strcmp(bank_data->selection, "a") == 0) {
            int amount;
            printf("Enter amount to be added:\n");
            scanf("%d", &amount);
            if(amount > 0) {
                bank_data->balance += amount;
                printf("Balance added successfully\n");
                printf("Updated balance after addition:\n%d\n", bank_data->balance);
            }
            else {
                printf("Adding failed, Invalid amount\n");
            }
        }
        else if(strcmp(bank_data->selection, "w") == 0) {
            int amount;
            printf("Enter amount to be withdrawn:\n");
            scanf("%d", &amount);
            if(amount > 0 && amount <= bank_data->balance) {
                bank_data->balance -= amount;
                printf("Balance withdrawn successfully\n");
                printf("Updated balance after withdrawal:\n%d\n", bank_data->balance);
            }
            else {
                printf("Withdrawal failed, Invalid amount\n");
            }
        }
        else if(strcmp(bank_data->selection, "c") == 0) {
            printf("Your current balance is:\n%d\n", bank_data->balance);
        }
        else {
            printf("Invalid selection\n");
        }
        
        strcpy(message_buffer, "Thank you for using");
        write(pipe_fd[1], message_buffer, sizeof(message_buffer));
        close(pipe_fd[1]);
        exit(0);
    }
    else {
        close(pipe_fd[1]);
        read(pipe_fd[0], message_buffer, sizeof(message_buffer));
        printf("%s\n", message_buffer);
        close(pipe_fd[0]);
        
        // Wait for the child process to finish without using wait(NULL)
        int status;
        while (1) {
            if (kill(process_id, 0) == -1) {
                break; // Child process has finished
            }
            usleep(100); // Sleep for a short duration to avoid busy waiting
        }
        
        shmctl(shm_id, IPC_RMID, NULL);
    }

    return 0;
}
