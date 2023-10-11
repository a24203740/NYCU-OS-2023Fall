#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void show(int fork_id) {
    printf("Fork %d. I'm the child %d, my parent is %d.\n", fork_id, getpid(), getppid());
}

int createFork(int forkID)
{
    pid_t childPid;
    childPid = fork(); // Fork 1
    if (childPid == 0) 
    {
        show(forkID);
    } 
    else if (childPid > 0) 
    {
        wait(NULL);
    }
    return childPid;
}

int main()
{
    printf("Main Process ID: %d\n\n", getpid());
    
    pid_t childPid;

    childPid = createFork(1);
    childPid = createFork(2);

    // terminate fork 2 child immediately
    if(childPid == 0)return 0;

    childPid = createFork(3);
    childPid = createFork(4);
    

    return 0;
}