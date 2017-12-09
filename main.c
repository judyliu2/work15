#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include "sem.h"
#define semKEY 11111
#define memKEY 19192



/*
Will attempt to access the resources using the semaphore
  - once in, it should display the last line added to the file (use shared memory to contain the size of the last line)
  - prompt used for the next line
  - one a new line is read, write to that file, update the sahred memory and then release the semaphore
 */

int main(int argc, char* argv[]){
  //*Print last line of file through shared memory
  
  if (!strcmp(argv[1], "-c") && argc == 3 ){
    
  }
  else if (!strcmp(argv[1], "-v")){
    view();
  }
  else if (!strcmp(argv[1], "-r")){
    removeF();
  }
  else{
    printf("Command not recognized. Please try again\n");
  }
}
