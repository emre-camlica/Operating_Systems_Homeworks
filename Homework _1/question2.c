#include <stdio.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void recursive_hierarchy(int id, int m, int M);

int main(int argc, char *argv[]){
    int n, m;
    int id=0;
    printf("Enter n and m respectively: ");
    scanf("%d %d", &n, &m);
    printf("The first process, no parent, id: %d\n", getpid());
    for(int i=0; i<=n; i++){
        recursive_hierarchy(id, m, m);
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
            if(i!=0){
                printf("Child process id: %d\n", getpid());
                printf("Parent process id: %d\n", getppid()); 
            }
            return 0;
        }
    }
    return 0;
}

void recursive_hierarchy(int id, int m, int M){
    if(M==0){
        return;
    }
    if(m==0){
        printf("Child process id: %d\n", getpid());
        printf("Parent process id: %d\n", getppid());
        exit(0);
    }
    
    id=fork();
    if(id==0)
    recursive_hierarchy(id, m-1, M);

    if(id>0){
        wait(NULL);
        if(m==M){
            return;
        }
        else{
        printf("Child process id: %d\n", getpid());
        printf("Parent process id: %d\n", getppid());
        exit(0);
        }
    }
}