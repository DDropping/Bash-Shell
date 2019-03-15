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

    // //prints contents of myargv and myargc
    //     printf("\n");
    //     printf("myargc: %d\n", myargc);
    //     int i=0;
    //     while(myargv[i]){
    //         printf("myargv[%d]: %s\n", i, myargv[i]);
    //         i++;
    //     }
    //     printf("\n");

    /*===========================================================================================
    3.Add functionality to your shell to execute simple shell commands. Start with commands like
    ls, then commands with options like ls -la /home.
    ===========================================================================================*/
    //code at bottom of program

    //break if user enters 'exit'
        if(strcmp(myargv[0], "exit") == 0) {
			return 0;
		}


    /*===========================================================================================
    4.Add functionality to shell to execute input and output redirection. It is required to
    implement >, >>, and <.
    ===========================================================================================*/
        
        
        pid_t id;
        id=fork();
        if(id == 0){
            int fd0, fd1;
            char input[64], output[64];
            bool in=false, out=false;

    //determine if statement is redirection
            for(int i=0; i< myargc; i++){
                if(strcmp(myargv[i], "<")==0){
                    in = true;
                    myargv[i] = NULL;
                    strcpy(input, myargv[i+1]);
                }
                if(strcmp(myargv[i], ">")==0){
                    out = true;
                    myargv[i] = NULL;
                    strcpy(output, myargv[i+1]);
                }
            }

    //if input redirection(<)
            if(in){
                int fd0;
                if((fd0=open(input, O_RDONLY, 0)) < 0) {
                    perror("Could not open input file");
                    exit(0);
                }
                dup2(fd0, 0);
                close(fd0);
            }

    //if output redirection(>)
            if(out){
                int fd1;
                if((fd1=creat(output, 0644)) < 0) {
                    perror("Could not open output file");
                    exit(0);
                }
                dup2(fd1, STDOUT_FILENO);
                close(fd1);
            }
            execvp(*myargv, myargv);
            perror("error: execvp");
            exit(1);

        }
    //if error
        else if((id) < 0){
            printf("fork() failed!\n");
            exit(1);
        }
        else{
            wait(0);
        }    


    /*===========================================================================================
    5.Add functionality to shell to execute commands in the background. For example
    commands like : ls -la &
    ===========================================================================================*/

    //fork code at bottom

    /*===========================================================================================
    6.Add functionality to your shell to execute the cd and pwd commands. Note these need
    to be implemented in your shell. Use the chdir() and getpwd() functions to implement
    these shell commands.
    ===========================================================================================*/

    //implement pwd command
        char pwd[100];
        if (strcmp("pwd", myargv[0]) == 0) {
            if(getcwd(pwd, sizeof(pwd)) != NULL) {
                printf("%s\n", pwd);
            } else {
                printf("pwd failed");
            }
            
        }

    //implement cd command
        char *dir;
        if (strcmp(myargv[0], "cd") == 0) {
            if(myargc > 1)
                dir = myargv[1];
            int status = chdir(dir);
            if(status == -1)
                perror("cd failed");
        }


    /*===========================================================================================
    7.Add functionality to your shell to execute piped commands. These are commands that
    are connected by a shell pipe, |.
    ===========================================================================================*/
    



    //===========================================================================================
    //BOTTOM
    //===========================================================================================
     
     
        // pid_t id;
        // id=fork();

    //parent
        if(id>0){
            wait(0);
        }
    //child
        else if(id==0){
            int result = execvp(myargv[0], myargv);
        }
    //error
        else{
            perror("fork error");
        }
    }


    //===========================================================================================
    //===========================================================================================
    //MISC.

        // //create string from user input
        // char str_cmd[100] = "";
        // i = 0;
        // while(myargv[i] != NULL){
        //     if(i>0)
        //     strcat(str_cmd, " ");
        //     strcat(str_cmd, myargv[i]);
        //     i++;
        // }
        // strcat(str_cmd, "\0");

        // //adds 'ls' functionality 
        //     if( (strcmp(myargv[0], "ls") == 0) ) {
        //         system(str_cmd);
        //     }

    //MISC
    //===========================================================================================
    //===========================================================================================

    return 0;
}
