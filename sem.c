#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define semKEY 1111
#define memKEY 1919
#include "sem.h"

/*
  - Should make the shared memory, semaphore and file (open the file with the truncate flag).
  - Set any values that are needed 
 */

void create(){
  int sid = 0;
  int shid = 0;
  int * shm_ptr;
  int fd = 0;
  union semun semaUn;
  semaUn.val = 1;
  sid = semget(semKEY, 1, IPC_CREAT |IPC_EXCL| 0600);
  shid = shmget( memKEY, 4, IPC_CREAT | IPC_EXCL | 0600);
  fd = open("story", O_CREAT | O_TRUNC | 0644);
 
  
  if (sid == -1){
      printf("Semaphore already exists.\n");
    }
    else{
      
      printf("Semaphore created: %d\n", sid);
      printf("Semaphore value: %d\n", semctl(sid, 0, GETVAL));
      printf("Shared Memory created: %d\n", shid);
      printf("File is created: %d\n", fd);
    }
  semctl(sid, 0, SETVAL, semaUn.val);
  close(fd);
  
  shm_ptr = shmat(shid, 0, 0); //shared mem = size of new last line written
  write(fd, shm_ptr , sizeof(shm_ptr));

}
/*
  - Remove the shared memory, the semaphore and the story
  - Display the full contents of the story
  - This should wait until the semaphore is available
 */
void removeF(){
  int sid = 0;
  int shid = 0;
  int * shm_ptr;
  
  
  sid = semget(semKEY, 1, 0);
  printf("Semaphore removed: %d\n", semctl(sid, 0, IPC_RMID));
  view();
  shid = shmget(memKEY, 4, IPC_RMID); 
  shmdt(shm_ptr);
  remove("story");
}

char* readFile(){
  
  int fd = open("story", O_RDONLY);
  struct stat sb;
  stat("story", &sb);
  int size = sb.st_size;
 
  char * newbuff = calloc(1, size+1);
  read(fd, newbuff, sizeof(newbuff));
  
  return newbuff;

}
/*
  - Output the contents of the story file
  - This mode does not need to interact with the semaphore
*/
void view(){
  char *story = readFile();
  printf("Story contents:\n");
  printf("%s\n", story);
  free(story);
    
}

int main(int argc, char* argv[]){
     
  if (!strcmp(argv[1], "-c") && argc == 2 ){
   
    create();
  }
  else if (!strcmp(argv[1], "-v")){
    view();
  }
  else if (!strcmp(argv[1], "-r")){
    removeF();
    
  }
  else{
    printf("command is not recognized\n");
  }

  return 0;
}

