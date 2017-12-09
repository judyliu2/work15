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

int sid = 0;
int shid = 0;
int * shm_ptr;
int fd = 0;



/*
  - Should make the shared memory, semaphore and file (open the file with the truncate flag).
  - Set any values that are needed 
 */

void create( union semun N ){

  sid = semget(semKEY, 4, IPC_CREAT |IPC_EXCL| 0600);
  shid = shmget( memKEY, 4, IPC_CREAT | IPC_EXCL | 0600);
  fd = open("story", O_CREAT | O_TRUNC | 0640);
  if (sid == -1){
      printf("Semaphore already exists.\n");
    }
    else{
      
      printf("Semaphore created: %d\n", sid);
      printf("Semaphore value: %d\n", semctl(sid, 0, GETVAL));
      printf("Shared Memory created: %d\n", shid);
      printf("File is created: %d\n", fd);
    }
  semctl(sid, 0, SETVAL, N); 
  
  shm_ptr = shmat(shid, 0, 0); //shared mem = size of new last line written
  write(fd, shm_ptr , sizeof(shm_ptr));

}


/*
  - Remove the shared memory, the semaphore and the story
  - Display the full contents of the story
  - This should wait until the semaphore is available
 */
void removeF(){
  sid = semget(memKEY, 4, 0600);
  printf("Semaphore removed: %d\n", semctl(sid, 0, IPC_RMID));
  printf("Story contents:\n");
  readFile();
  shid = shmget(memKEY, 4, IPC_RMID); 
  shmdt(shm_ptr);
}

void readFile(){
  char * newbuff; 
  read(fd, newbuff, sizeof(newbuff));
  printf("%s\n", newbuff);
}
/*
  - Output the contents of the story file
  - This mode does not need to interact with the semaphore
 */
void view(){
    printf("Story contents:\n");
    readFile();
}


/*

int main(int argc, char* argv[]){
 
  int sid;
  
  if (!strcmp(argv[1], "-c") && argc == 3 ){
    printf("testing out -c N :\n");
    sid = semget(KEY, 4, IPC_CREAT |IPC_EXCL| 0600);
    if (sid == -1){
      printf("Semaphore already exists.\n");
    }
    else{
      
      printf("Semaphore created: %d\n", sid);
      printf("Semaphore value: %d\n", semctl(sid, 0, GETVAL));
      
    }
    
  }
  else if (!strcmp(argv[1], "-v")){
    printf("testing out -v :\n");
    sid = semget(KEY, 4, 0600);
    printf("Semaphore value: %d\n", semctl(sid, 0, GETVAL));
  }
  else if (!strcmp(argv[1], "-r")){
    printf("testing out -r:\n");
    sid = semget(KEY, 4, 0600);
    printf("Semaphore removed: %d\n", semctl(sid, 0, IPC_RMID));
    
  }
  else{
    printf("command is not recognized\n");
  }

  return 0;
}
*/
