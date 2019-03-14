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
#include <sys/wait.h>
#include <fcntl.h>

#include <stdbool.h>

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

while (true) {

    char *myargv[100] = {NULL}; //collection of sub-strings

    /*===========================================================================================
    1.Implement your shell to simply initialize your shell, display a prompt, read in user
    input and print it back to the console.
    ===========================================================================================*/

        printf("%s", PROMPT);

    //read user input into buffer, fill rest of buffer with \0
        memset(buffer, '\0',BUFFERSIZE );
        fgets(buffer, BUFFERSIZE, stdin);


    /*===========================================================================================
    2.Add functionality to your shell to parse user input setting the correct values for myargv
    and myargc. Once parsed, print myargv and myargc to the console. Note that myargvs
    need to be null terminated for the exec commands to interpret them correctly
    ===========================================================================================*/
    //note: may need to add string terminator to end of each substring in myargv...

    //tokenize the substrings
        char* token = strtok(buffer, " \n");
        int tokenNum=0;
        while(token != NULL){
            myargv[tokenNum] = token;
            token = strtok(NULL, " \n");
            tokenNum++;
        }

    //set myargc
        myargc=tokenNum;

    //prints contents of myargv and myargc
        printf("\n");
        printf("myargc: %d\n", myargc);
        int i=0;
        while(myargv[i]){
            printf("myargv[%d]: %s\n", i, myargv[i]);
            i++;
        }
        printf("\n");

    /*===========================================================================================
    3.Add functionality to your shell to execute simple shell commands. Start with commands like
    ls, then commands with options like ls -la /home.
    ===========================================================================================*/

    //create string from user input
        char str_cmd[100] = "";
        i = 0;
        while(myargv[i] != NULL){
            if(i>0)
            strcat(str_cmd, " ");
            strcat(str_cmd, myargv[i]);
            i++;
        }


    //break if user enters 'exit'
        if(strcmp(myargv[0], "exit") == 0) {
			return 0;
		}

    //adds 'ls' functionality 
        if( (strcmp(myargv[0], "ls") == 0) ) {
			system(str_cmd);
		}





    /*===========================================================================================
    4.Add functionality to shell to execute input and output redirection. It is required to
    implement >, >>, and <.
    ===========================================================================================*/



    /*===========================================================================================
    5.Add functionality to shell to execute commands in the background. For example
    commands like : ls -la &
    ===========================================================================================*/



    /*===========================================================================================
    6.Add functionality to your shell to execute the cd and pwd commands. Note these need
    to be implemented in your shell. Use the chdir() and getpwd() functions to implement
    these shell commands.
    ===========================================================================================*/



    /*===========================================================================================
    7.Add functionality to your shell to execute piped commands. These are commands that
    are connected by a shell pipe, |.
    ===========================================================================================*/
    }
return 0;
}
