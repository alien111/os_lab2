#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
 

 
void server(int fd1[2], int fd2[2]){
    printf("server\n");
    close(fd1[0]); // close reading end of first pipe
    close(fd2[1]); // close writing end of second pipe
 
    int writefd = fd1[1], readfd = fd2[0];

    FILE * write = fopen("output.txt", "w");
 
 
    int size;

    read(readfd, &size, sizeof(int));

    int array[size];
    for (int i = 0; i < size; i++) {
        read(readfd, &array[i], sizeof(int));
        array[i] *= 10;
    }

    fprintf(write, "%d\n", size);

    for (int i = 0; i < size; i++) {
        fprintf(write, "%d ", array[i]);
    }

    fclose(write);
    close(writefd);
    close(readfd);
}
 
void client(int fd1[2], int fd2[2]){
    printf("client\n");
    close(fd1[1]);
    close(fd2[0]);
 
    int writefd = fd2[1], readfd = fd1[0];
 
    FILE * read = fopen("input.txt", "r");

    int size;

    fscanf(read, "%d", &size);

    int array[size];

    for (int i = 0; i < size; i++) {
        fscanf(read, "%d", &array[i]);
    }

    write(writefd, &size, sizeof(int));

    for (int i = 0; i < size; i++) {
        write(writefd, &array[i], sizeof(int));
    }

    fclose(read);

    close(writefd);
    close(readfd);
    exit(0);
}
 
int main(){
    // fd - read, write
    // fd1 - for parent to write
    // fd2 - for parent to read
    int fd1[2], fd2[2];
    if(pipe(fd1) == -1){
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if(pipe(fd2) == -1){
        fprintf(stderr, "Pipe failed");
        return 1;
    }
 
 
    pid_t childPID = fork();
    if(childPID == 0)client(fd1, fd2);
    else if (childPID > 0) server(fd1, fd2);
    else {
        fprintf(stderr, "fork() was not finished successfully");
        return 1;
    }
    return 0;
}