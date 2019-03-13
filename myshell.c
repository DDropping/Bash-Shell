/****************************************************************
 * Name        :                                                *
 * Class       :  CSC 415                                       *
 * Date        :                                                *
 * Description :  Writting a simple bash shell program          *
 *                that will execute simple commands. The main   *
 *                goal of the assignment is working with        *
 *                fork, pipes and exec system calls.            *
 ****************************************************************/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/wait.h>
#include <fcntl.h>

/* CANNOT BE CHANGED */
#define BUFFERSIZE 256
/* --------------------*/
#define PROMPT "myShell >> "
#define PROMPTSIZE sizeof(PROMPT)

//ctrl+shift+c to comment
//ctrl+shift+x to uncomment

int
main(int* argc, char** argv)
{

    char buffer[BUFFERSIZE];
    int myargc; //number of strings encountered
    char *myargv[100]; //collection of sub-strings



    printf("%s", PROMPT);
//read user input into buffer, fill rest of buffer with \0
    memset(buffer, '\0',BUFFERSIZE );
    fgets(buffer, BUFFERSIZE, stdin);

//print contents of buffer to console
    printf("\n");
    for ( int i = 0; i < BUFFERSIZE; i++ ){
        putc( isprint(buffer[i]) ? buffer[i] : '.' , stdout );
    }
    printf("\n");

//tokenize the substrings
    char* token = strtok(buffer, " ");
    int tokenNum=0;
    while(token != NULL){
        myargv[tokenNum] = token;
        token = strtok(NULL, " ");
        tokenNum++;
    }

//set myargc
    myargc=tokenNum;

//prints contents of myargv and myargc
    printf("\n");
    printf("myargc: %d\n", myargc);
    for(int i=0; i< 100; i++){
            printf("myargv[%d]: %s\n",i, myargv[i]);
    }
    printf("myargc: %d", myargc);
    printf("\n");


return 0;
}
