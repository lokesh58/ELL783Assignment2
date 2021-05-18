#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

void breakpoint(){
  static int i=1;
  static char in[5];
  printf(1, "Breakpoint %d: Press any key to continue\n", i);
  gets(in, 5);
  i += 1;
}

int main(int argc, char *argv[]){
  #if defined(FIFO)
  printf(1, "FIFO\n");
  #elif defined(SCFIFO)
  printf(1, "SCFIFO\n");
  #elif defined(NFU)
  printf(1, "NFU\n");
  #endif
  printf(1, "Parent pid=%d\n", getpid());
  char *str[20];
  for(int i=0;i<15;++i){
    str[i] = sbrk(PGSIZE);
    //used sbrk instead of malloc because malloc was allocating extra memory
    //sbrk gives better control
  }
  printf(1, "Printing to not get variable unused warning str=0x%x\n", str);
  breakpoint();
  str[15] = sbrk(PGSIZE);
  breakpoint();
  str[16] = sbrk(PGSIZE);
  breakpoint();
  str[2][0] = 'a'; //use page 2
  breakpoint();
  str[17] = sbrk(PGSIZE);
  breakpoint();
  str[18] = sbrk(PGSIZE);
  breakpoint();
  str[2][0] = 'b'; //use page 2
  breakpoint();
  int pid = fork();
  if(pid == 0){
    printf(1, "Child pid=%d\n", getpid());
    sbrk(-PGSIZE); //delete page 18, free space in memory
    str[0][0] = 'a'; //use 0
    breakpoint();
    exit();
  }else if(pid > 0){
    wait();
  } else {
    printf(1, "fork error\n");
  }
  exit();
}
