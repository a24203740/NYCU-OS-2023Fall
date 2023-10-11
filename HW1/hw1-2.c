#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void getUserCommand(char* inputBuffer)
{
    printf("osh> ");
    fgets(inputBuffer, 1000, stdin);
}

int isSpace(char c)
{
    return (c == ' ') || (c == '\0') || (c == '\n'); 
}

int getArgumentCount(const char * str)
{
    int strSize = strlen(str);
    int count = 0;
    //printf("%s ", str);
    for(int i = 1; i <= strSize; i++)
    {
        if((isSpace(str[i])) && !isSpace(str[i-1]))
        {
            //printf("!");
            count++;
        }
        else
        {
            //printf(" ");
        }
    }
    //printf("\n");
    return count;
}

int locateNextArgStart(const char * str, int len, int st)
{
    for(int i = st + 1; i < len; i++)
    {
        if(!isSpace(str[i]))return i;
    }
    return -1;
}
int locateNextArgEnd(const char * str, int len, int st)
{
    for(int i = st; i <= len; i++)
    {
        if(isSpace(str[i]))return i-1;
    }
    return -1;
}

void getArgumentPos(int startPos[], int edPos[], const char * str, int n)
{
    int strSize = strlen(str);
    int st = -1, ed = -1;
    for(int i = 0; i < n; i++)
    {
        st = locateNextArgStart(str, strSize, ed);
        ed = locateNextArgEnd(str, strSize, st);
        //printf("st = %d, ed = %d \n", st, ed);
        //printf("for char, st = %c, ed = %c\n", str[st], str[ed]);

        startPos[i] = st;
        edPos[i] = ed;
    }
}
void mallocArgStringSpace(int startPos[], int edPos[], char* argList[], int n)
{
    for(int i = 0; i < n; i++)
    {
        int size = edPos[i] - startPos[i] + 1;
        argList[i] = malloc((size+1) * sizeof(char));
    }
}

void parseArgument(int startPos[], int edPos[], char* argList[], const char * str, int n)
{
    for(int i = 0; i < n; i++)
    {
        int argSize = edPos[i] - startPos[i] + 1;
        memcpy(argList[i], str+startPos[i], argSize+1);
        *(argList[i] + argSize) = '\0';
        //printf("%s\n", argList[i]);
    }
    argList[n] = NULL;
}

void freeArgSpace(char* argList[], int n)
{
    for(int i = 0; i < n; i++)
    {
        free(argList[i]);
    }
}

void runShell()
{
    while (1)
    {
        char str[1000];
        getUserCommand(str);
        int n = getArgumentCount(str);
        
        char* argList[n+1];
        int argStPos[n], argEdPos[n];
        
        getArgumentPos(argStPos, argEdPos, str, n);
        mallocArgStringSpace(argStPos, argEdPos, argList, n);
        parseArgument(argStPos, argEdPos, argList, str, n);

        if(strcmp(argList[0], "exit") == 0)
        {
            freeArgSpace(argList, n);
            printf("process end\n");
            return;
        }
        else
        {
            pid_t pid;
            pid = fork();
            if(pid == 0)
            {
                execvp(argList[0], argList);
                return;
            }
            else if(pid > 0)
            {
                wait(NULL);
                freeArgSpace(argList, n);
            }
        }
        
    }
    
}
int main()
{
    runShell();
}