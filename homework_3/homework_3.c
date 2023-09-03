#include <stdio.h>
#include <stdlib.h>

int find_available(int resources[5], int requests[5][5], int finished[5]);
void allocate(int i, int resources[5], int requests[5][5], int allocations [5][5], int finished[5]);
void release(int resources[5], int allocations[5][5], int finished[5]);

int main(){
    int resources [5];
    int allocations [5][5];
    int requests [5][5];
    int finished [5]={0,0,0,0,0};
    FILE *fptr;
    int i, j, n=-1;
    int val; // To store file contents.
    fptr = fopen("resources.txt", "r");
    while (!feof(fptr))
    {
        for(i=0; i<5; i++){
            fscanf(fptr, "%d", &val);
            resources[i] = val;   
        }
    }
    fclose(fptr);

    fptr = fopen("allocations.txt", "r");
    while (!feof(fptr))
    {
        for(j=0; j<5; j++){
            for(i=0; i<5; i++){
            fscanf(fptr, "%d", &val);
            allocations[j][i] = val;   
            }
        }
    }
    fclose(fptr);

    fptr = fopen("requests.txt", "r");
    while (!feof(fptr))
    {
        for(j=0; j<5; j++){
            for(i=0; i<5; i++){
            fscanf(fptr, "%d", &val);
            requests[j][i] = val;   
            }
        }
    }
    fclose(fptr);

    for(i=0; i<5; i++){
        printf("Information for process : P%d:\n", i+1);
        printf("Allocated resources : ");
        for(j=0; j<5; j++){
            printf("R%d:%d ", j+1, allocations[i][j]);
        }
        printf("\n");
        printf("Resource request : ");
        for(j=0; j<5; j++){
            printf("R%d:%d ", j+1, requests[i][j]);
        }
        printf("\n");
    }
    
    /*Subtracting allocated resources from all resources to get the available resources:*/
    for(i=0; i<5; i++){
        for(j=0; j<5; j++){
            resources[i]-=allocations[j][i];
        }
    }
    
    printf("\nRunning order for processes: ");
    while(finished[0]+finished[1]+finished[2]+finished[3]+finished[4]!=5){
    n=find_available(resources, requests, finished);

    if(n==-1){
        printf("\nThere is a deadlock. ");
        for(int i=0; i<5; i++){
            if(finished[i]==0){
                printf("P%d ", i+1);
            }
        }
        printf("are the cause of deadlock.");
        break;
    }
    allocate(n, resources, requests, allocations, finished);
    printf("P%d ", n+1);
    while (n!=-1)
    {
        n=find_available(resources, requests, finished);
        if(n==-1)
            break;

        allocate(n, resources, requests, allocations, finished);
        printf("P%d ", n+1);
    }
    release(resources, allocations, finished);
    }
    


return 0;
}

/*returns the availabe process index.*/
int find_available(int resources[5], int requests[5][5], int finished[5]){
    for(int i=0; i<5; i++){
        if(finished[i]!=1){
        if(resources[0] >= requests[i][0] && 
        resources[1] >= requests[i][1] && 
        resources[2] >= requests[i][2] &&
        resources[3] >= requests[i][3] &&
        resources[4] >= requests[i][4])
        return i;
    }
    }
    return -1;
}

/*Puts all requests into the allocations array and removes the items of the requests array for the corresponding process.*/
void allocate(int i, int resources[5], int requests[5][5], int allocations[5][5], int finished[5]){
    finished[i] = 1; 
    for(int j=0; j<5; j++){
        resources[j] -= requests[i][j];
        allocations[i][j] += requests[i][j];
        requests[i][j] = 0;
    }
}

/*If the process is finished, removes its allocations and adds them to the resources*/
void release(int resources[5], int allocations[5][5], int finished[5]){
    for(int i=0; i<5; i++){
        if(finished[i] ==1){ 
            for(int j=0; j<5; j++){
                resources[j] += allocations[i][j];
                allocations[i][j] = 0;
            }
        }

    }
}