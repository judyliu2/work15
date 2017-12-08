#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY 11111




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
    semctl(sid, 0 , SETVAL, atoi(argv[2]));
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
