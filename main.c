#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sem.h" 

#define KEY 11111

int main(int arc, char* argv[]){

  if (!strcmp(argv[1], "-c") && argc == 3 ){
    


