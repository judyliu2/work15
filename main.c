#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define semKEY 1111
#define memKEY 1919
#include "sem.h"

/*
Will attempt to access the resources using the semaphore
  - once in, it should display the last line added to the file (use shared memory to contain the size of the last line)
  - prompt used for the next line
  - one a new line is read, write to that file, update the shared memory and then release the semaphore
 */

int main(int argc, char* argv[]){
  union semun semaUn;
  printf("It's ya boi, semaphone by Jasper and Judy\n\n\n");
  //DOWN
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op = -1;
  op.sem_flg = SEM_UNDO; 
  int sid = semget(semKEY,1, 0777);
  // printf("%d\n", sid);
  
   
  if(sid == -1){ printf("please compile sem.c and run ./a.out with -c\n");}
  else{
    

   printf("Semaphore Value: %d before access\n", semctl(sid, 0, GETVAL, semaUn));
   printf("WAITING...\n");
  semop(sid, &op, 1 );  
  printf("Semaphore Value: %d after access\n",semctl(sid, 0, GETVAL, semaUn));
 
  int fd = open("story", O_RDWR | O_APPEND | 0644);
  printf("last line of story:\n");
  int shid = shmget(memKEY,0,0644);
 
  int * size = shmat(shid, 0,0); //size of last line written
  //printf("%d\n", *size);
  //read correct size bytes from last line
  
 
  lseek(fd, -1 * (* size), SEEK_END);
  char * line = (char*) calloc(*size, sizeof(char));
  read(fd, line, *size);
  printf("%s\n",line);
  free( line);
  
  
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
