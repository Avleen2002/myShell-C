#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>

//STRUCTURES----------------------------------------------------------//

typedef struct {
char *name;
struct tm time;
int retValue;
} Command;

typedef struct {
    char *name;
    char *value;
} EnvVar;

//ARRAYS-------------------------------------------------------------//
typedef struct {
    EnvVar *array;
    size_t used;
    size_t size;
} Array;

typedef struct {
    Command *array;
    size_t used;
    size_t size;
} logArray;

//FUNCTIONS-----------------------------------------------------------//

//EnvVar Functions
void initArray(Array *a, size_t initialSize);
char* searchArray(Array *a, char* elementName);
void insertArray(Array *a, char* elementName, char* elementValue);
void freeArray(Array *a);
void envVar(char* command);
void printEnvVar(char *str);

//Log Functions
void initlogArray(logArray *a, size_t initialSize);
char* printlogArray(logArray *a);
void insertlogArray(logArray *a, char* commandName, int ret);
void freelogArray(logArray *a);

//Other Built-in Commands
void parseCommand(char* command);
void exitFunc();
void printFunc(char* command);
void themeFunc(char* command);
void waitDescision(char* command);

//Non Built-in Commands
void libCommands(char* command);


//GLOBAL VAR----------------------------------------------------------//

Array eV;
logArray la;
int waitVar = 1;
int tempVar;

//INT MAIN-----------------------------------------------------------//

int main(int argc, char* argv[])
{

    char input[1024];
    initArray(&eV, 10);//initialise the array of envVar.
    initlogArray(&la, 10);//initialise the array of command logs.

    if(argc == 2) //script mode
    {
        FILE* fptr;
        fptr = fopen(argv[1], "r");

        while(fgets(input,1024,fptr) != NULL)
        {
            strtok(input, "\n");
            parseCommand(input);
        }

        fclose(fptr);
    }
    else if(argc == 1) //interactive mode
    {
        int flag = 1;
        while(flag != 0)
        {
            printf("cshell$ ");
            fgets(input,1024,stdin);  
            strtok(input, "\n");
            parseCommand(input);
        }
    }

    freeArray(&eV);
    freelogArray(&la);
    return 0;
}

//FUNCTION DEFINITIONS-------------------------------------------------------//

//Description: Parses the input command and calls the appropriate function for 
//             the command, fork() and exec() happen here.
//Return Value: Void.
void parseCommand(char* command)
{

    waitDescision(command);
    int fc = 0; //fork();
    //if the command is EnvVar change

    if (fc == 0)
    {
        if(command[0] == '$')
        {   
            //DONE: printf("envVar block entered\n");
            envVar(command); 
        }
        else{
            //strtok(command, "\n");
            char* portion = strtok(command, " ");

            if(strcmp(portion, "exit") == 0)
            {
                //DONE: printf("exit");
                exitFunc();
            }
            else if(strcmp(portion, "print") == 0) //if the command is built-in: print
            {
                //DONE; printf("print");
                printFunc(command);

            }
            else if(strcmp(portion, "log") == 0) //if the command is built-in: log 
            {
                //DONE: printf("log");
                printlogArray(&la);
            }
            else if(strcmp(portion, "theme") == 0) //if the command is built-in: theme
            { 
                //DONE: printf("theme\n");
                themeFunc(command);
            }
            else //if the command is not built-in
            {
                // char* argList = strtok(NULL, " ");
                //DONE: printf("non-built");
                libCommands(command);

            }
        }
    }
    else{
        if(waitVar)
            wait(NULL);  //Makes the parent wait.
    }
}

void exitFunc()
{
    printf("Bye!\n");
    exit(0);
}

void printFunc(char* command)
{
    tempVar = 0;
    command = strtok(NULL, " ");
    if(command[0] == '$')
    {   
        //printf("Print var");
        printEnvVar(command);
    }
    else{
        while(command != NULL)
        {
            printf("%s ", command);
            command = strtok(NULL, " ");
        } 
    }

    printf("\n"); 
    insertlogArray(&la, "print", tempVar);
}

void printEnvVar(char *str)
{
    char* val = searchArray(&eV, str);
    if(val != NULL)
    {
        printf("%s", val);
    }else{
        tempVar = 1;
        printf("variable does not exist\n");
    }  
    return;
}

void themeFunc(char* command)
{
    command = strtok(NULL, " ");
    int flag = 0;
    if(strcmp(command, "red") == 0)
        printf("\x1B[31m");
    else if(strcmp(command, "green") == 0)
        printf("\x1B[32m");
    else if(strcmp(command, "blue") == 0)
        printf("\x1B[34m");
    else 
    {
        printf("unsupported theme\n");
        flag = 1;
    }
    
    insertlogArray(&la, "theme", flag);
}

void libCommands(char* command)
{
    //printf("%s\n", command);
    char* argumentList = (command, NULL);
    if(fork() == 0)
    {
        //Newly spawned child Process. This will be taken over by "ls -l"
        int status_code = execvp(command, argumentList);
        insertlogArray(&la, command , status_code);
        if (status_code == -1) 
        {
            printf("missing, keyword or command, or permission problem\n");
            return 1;
        } 
    }
    else
    {
        if(waitVar)
            wait();
        insertlogArray(&la, command , 0);
    }    
    return;
}

void waitDescision(char* command)
{
    // int len = strlen(command);

    // if(command[len-1] == '&')
    //     waitVar = 0;
    // else
    //     waitVar = 1;

}

//EnvVar Functions--------------------------------------------//

void initArray(Array *a, size_t initialSize) {
    a->size = initialSize;
    a->array = malloc(initialSize * sizeof(EnvVar));
    a->used = 0;
}

char* searchArray(Array *a, char* elementName){
    //printf("Entered Search\n");
    for(int i = 0; i < a->used ; i++)
    {
        if(strcmp(a->array[i].name, elementName) == 0)
        {
            char* temp = a->array[i].value;
            return temp;
        }
    }
    //printf("Exiting Search\n");
    return NULL;
}

void envVar(char* command)
{
    tempVar = 0;
    char* varName = strtok(command, "=");
    char* varVal = strtok(NULL, "=");

    //To insert in array
    insertArray(&eV, varName, varVal);

    insertlogArray(&la, command, tempVar);
}

void insertArray(Array *a, char* elementName, char* elementValue) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size 
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(EnvVar));
    }

    //printf("components: %s %s\n", elementName, elementValue);

    char* temp = searchArray(a, elementName);
    int flag = -1;
    if(temp != NULL)
    {
        strcpy(temp, elementValue);
        //printf("Updated value: %s", temp);
        flag = 0;

    }
    else{
        a->array[a->used].name = malloc(100);
        a->array[a->used].value = malloc(100);

        strcpy(a->array[a->used].name, elementName);
        strcpy(a->array[a->used].value, elementValue);
        a->used++;
        //printf("Element entered\n");
        flag = 0;
    }

    if(flag != 0)
        tempVar = 1;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

//Log Functions------------------------------------------------------//
void initlogArray(logArray *a, size_t initialSize) {
    a->size = initialSize;
    a->array = malloc(initialSize * sizeof(Command));
    a->used = 0;
}

char* printlogArray(logArray *a){
    //printf("Entered Print\n");
    int i;
    for(i = 0; i < a->used ; i++)
    {
        struct tm* tmptr = &a->array[i].time;
        printf("%s %s %d\n", asctime(tmptr), a->array[i].name, a->array[i].retValue);
    }
    tempVar = 0;

    if(i < a->used)
        tempVar = 1;

    insertlogArray(&la, "log", tempVar);
    //printf("Exiting Print\n");
    return NULL;
}

void insertlogArray(logArray *a, char* commandName, int ret) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size 
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(Command));
    }

    time_t rawtime;
    time( &rawtime );
    struct tm* tmptr = localtime( &rawtime );
    a->array[a->used].time =  *tmptr; //Current Time stored in Log
    a->array[a->used].name = malloc(100);
    strcpy(a->array[a->used].name, commandName);
    a->array[a->used].retValue = ret;
    a->used++;  
}

void freelogArray(logArray *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}