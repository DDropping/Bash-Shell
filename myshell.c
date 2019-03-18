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


int
main(int* argc, char** argv)
{

	char buffer[BUFFERSIZE];
	int myargc; //number of strings encountered

	while (true) {

		char *myargv[100] = { NULL }; //collection of sub-strings

		printf("%s", PROMPT);

		//read user input into buffer, fill rest of buffer with \0
		memset(buffer, '\0', BUFFERSIZE);
		fgets(buffer, BUFFERSIZE, stdin);

		//tokenize the substrings
		char* token = strtok(buffer, " \n");
		int tokenNum = 0;
		while (token != NULL) {
			myargv[tokenNum] = token;
			token = strtok(NULL, " \n");
			tokenNum++;
		}

		//set myargc
		myargc = tokenNum;

		//break if user enters 'exit'
		if (strcmp(myargv[0], "exit") == 0) {
			return 0;
		}


        //implement cd command
        char *dir;
        if (strcmp(myargv[0], "cd") == 0) {
            if (myargc > 1)
                dir = myargv[1];
            int status = chdir(dir);
            if (status == -1)
                perror("cd failed");
            continue;
        }

        //implement pwd command
            char pwd[100];
            if (strcmp("pwd", myargv[0]) == 0) {
                if (getcwd(pwd, sizeof(pwd)) != NULL) {
                    printf("%s\n", pwd);
                }
                else {
                    printf("pwd failed");
                }
                continue;
            }


            ////////////////////////////////////////////////////////////////////////


		    bool background = false;
			int fd0, fd1, fd2, start=0;
            int pipe_fd[2];
			char input[64], output[64], append[64];
            char *left_side[64] = {NULL};
            char *right_side[64] = {NULL};
			bool in = false, out = false, app = false, isPipe = false, next = false;

            //determine if piped command
            for(int i=0; i<myargc; i++){
                if(strcmp(myargv[i], "|")==0){
                    isPipe = true;
                }
            }
                    //test: if pipe char was found
            // if(isPipe){
            //     printf("| was found\n");
            // }

            if(isPipe){
                for(int i = 0; i < myargc; i++){
                    if(strcmp(myargv[i], "|")==0){
                        next = true;
                        continue;
                    }
                    if(!next){
                        left_side[i] = myargv[i];
                    }
                    if(next){
                        right_side[start] = myargv[i];
                        start++;
                    }
                }
            }
                    //test: prints contents of left and right side
            // if(isPipe){
            //     for(int i = 0; left_side[i] != NULL; i++){
            //         printf("left %d: %s \n",i, left_side[i]);
            //     }
            //     for(int i = 0; right_side[i] != NULL; i++){
            //         printf("right %d: %s \n",i, right_side[i]);
            //     }
            // }

			//determine if should run in Background
			if (strcmp(myargv[myargc - 1], "&") == 0) {
				background = true;
				myargv[myargc - 1] = NULL;
				myargc = myargc - 1;
			}

			//determine if statement is redirection
			for (int i = 0; i < myargc; i++) {
				if (strcmp(myargv[i], "<") == 0) {
					in = true;
					myargv[i] = NULL;
					strcpy(input, myargv[i + 1]);
				}
				else if (strcmp(myargv[i], ">") == 0) {
					out = true;
					myargv[i] = NULL;
					strcpy(output, myargv[i + 1]);
				}
				else if (strcmp(myargv[i], ">>") == 0) {
					app = true;
					myargv[i] = NULL;
					strcpy(append, myargv[i + 1]);
				}

			}

			//test
				// printf(in ? "in: true\n" : "in: false\n");
				// printf(out ? "out: true\n" : "out: false\n");
				// printf(app ? "app: true\n" : "app: false\n");
				// printf(background ? "background: true\n" : "background: false\n");


        pid_t id;
		id = fork();

        //if background
		if (background && id == 0) {
            //printf("background item");
			setpgid(0, 0);
			execvp(*myargv, myargv);
			perror("error: background execvp");
			exit(1);
		}

        //child
		if (id == 0) {
            //pipe
            if(isPipe){
                pipe(pipe_fd);
                if(fork()){
                    dup2(pipe_fd[0],0);
                    close(pipe_fd[1]);
                    close(pipe_fd[0]);
                    execvp(right_side[0],right_side);
                }
                if(fork()){
                    dup2(pipe_fd[1],1);
                    close(pipe_fd[1]);
                    close(pipe_fd[0]);
                    execvp(left_side[0],left_side);
                }
                exit(0);
                
            }

		    //if input redirection(<)
			if (in) {
				int fd0;
				if ((fd0 = open(input, O_RDONLY, 0)) < 0) {
					perror("Could not open input file");
					exit(0);
				}
				dup2(fd0, 0);
				close(fd0);
			}

			//if output redirection(>)
			if (out) {
				int fd1;
				if ((fd1 = creat(output, 0644)) < 0) {
					perror("Could not open output file");
					exit(0);
				}
				dup2(fd1, STDOUT_FILENO);
				close(fd1);
			}

			//if append redirection(>>)
			if (app) {
				int fd2;
				if ((fd2 = open(append, O_WRONLY | O_APPEND)) < 0) {
					perror("Could not open file to append");
					exit(0);
				}
				dup2(fd2, STDOUT_FILENO);
				close(fd2);
			}

            if(!isPipe){
			    execvp(*myargv, myargv);
			    perror("error: execvp");
			    exit(1);
            }
			

		}
        //parent
		else if (id > 0 && !background) {
            //printf("parent item \n");
		    wait(0);
		}
		//if error
		else if ((id) < 0) {
			printf("fork() failed!\n");
			exit(1);
		}

	}
	return 0;
}