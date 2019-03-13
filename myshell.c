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


    printf("shell: ");
    //memset(starting value of mem, value to be filled, number of bytes to be filled)
    memset(buffer, '\0',BUFFERSIZE );
    fgets(buffer, BUFFERSIZE, stdin);

//    print buffer
//    for ( int i = 0; i < BUFFERSIZE; i++ ){
//        putc( isprint(buffer[i]) ? buffer[i] : '.' , stdout );
//    }


return 0;
}
