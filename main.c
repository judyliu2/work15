#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define semKEY 11111
#define memKEY 19192
#include "sem.h"

/*
Will attempt to access the resources using the semaphore
  - once in, it should display the last line added to the file (use shared memory to contain the size of the last line)
  - prompt used for the next line
  - one a new line is read, write to that file, update the sahred memory and then release the semaphore
 */

int main(int argc, char* argv[]){
  
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op = -1;
  op.sem_flg = SEM_UNDO;
  int sid = semget(semKEY,1, 0644);

  if(sid == -1) printf("please run compile and run sem.c with -c\n");
  else{
  printf("Semaphore: %d before access\n", sid);  
  semop(sid, &op, 1 );  
  printf("Semaphore: %d after access\n", sid);
  
  int fd = open("story", O_RDWR | O_APPEND | 0644);  
  int shid = shmget(memKEY, 4, IPC_CREAT| IPC_EXCL | 0600);
  int * size; //size of last line written
  
  printf("last line of story:\n");
  if (shid == -1){
    //look at last line of what was written
    size = shmat(shid, 0 ,0);
  }
  else{
    size = shmat(shid, 0 ,0);
  }

  //taking user input
  printf("contribute to the story:\n");
  char input[512];
  fgets(input, 512, stdin);

  *size = strlen(input);
  shmdt(size);

  write(fd, input, strlen(input));
  close(fd);
  }
  
  
}
