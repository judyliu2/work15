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
 
  int fd = 0;
  union semun semaUn;
  semaUn.val = 1;
  sid = semget(semKEY, 1, IPC_CREAT| IPC_EXCL | 0777);
  shid = shmget(memKEY, 4, IPC_CREAT| IPC_EXCL | 0600);
  fd = open("story", O_CREAT | O_TRUNC | 0777);
 
  
  if (sid == -1){
      printf("Semaphore already exists.\n");
    }
    else{
      semctl(sid, 0, SETVAL, semaUn.val);
      printf("Semaphore created: %d\n", sid);
      printf("Semaphore value: %d\n", semctl(sid, 0, GETVAL, semaUn));
      printf("Shared Memory created: %d\n", shid);
      printf("File is created: %d\n", fd);
      close(fd);
    }
  
}
/*
  - Remove the shared memory, the semaphore and the story
  - Display the full contents of the story
  - This should wait until the semaphore is available
 */
void removeF(){
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op = -1;
  op.sem_flg = SEM_UNDO;
  
  int sid = semget(semKEY, 1, 0);
  int shid = shmget(memKEY, 0, 0);
  printf("WAITING FOR SEMAPHORE\n");
  semop(sid, &op,1);
  printf("DONE WAITING, BYE BYE STUFF\n");

  semctl(sid, 0, IPC_RMID);
  printf("Semaphore removed: %d\n", sid);
  shmctl(shid, IPC_RMID,0);
  printf("Shared Mem removed: %d\n", shid);
  view();
 
 
  remove("story");
}

char* readFile(){
  
  struct stat tistics;
  stat("story", &tistics);
  int fd = open("story", O_RDONLY);
  int size = tistics.st_size;
  char *s = calloc(1, size+1);
  read(fd, s, size);
  close(fd);
  return s;
  
  
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

