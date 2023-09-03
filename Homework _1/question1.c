#include <stdio.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int n;
    int id;
    printf("Enter n: ");
    scanf("%d", &n);
    printf("The first process, no parent, id: %d\n", getpid());
    for(int i=0; i<=n; i++){
        id=fork();
        if(id>0)
        wait(NULL);
        if(id==0){
            printf("Child process id: %d\n", getpid());
            printf("Parent process id: %d\n", getppid());
            return 0;
        }
        if(i==n){
        if(i!=0){
            printf("Child process id: %d\n", getpid());
            printf("Parent process id: %d\n", getppid());
        }
        break;
        }
        id=fork();
        if(id>0){
            wait(NULL);
            if(i!=0)
            {
                printf("Child process id: %d\n", getpid());
                printf("Parent process id: %d\n", getppid()); 
            }
            return 0;
        }
    }
    return 0;
}