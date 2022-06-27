
/*    compilation: gcc -o project2action1and4.1 project2action1and4.1.c -pthread
      running: ./project2action1and4.1 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

pthread_t tids [4];

void sig_handler(int signum) {
  
  printf("Signal %d has been handled\n", signum);
  signal(signum, sig_handler);

}


void *action(void *vptr) {
  
  signal(SIGILL, sig_handler);
  signal(SIGABRT, sig_handler);
  signal(SIGSEGV, sig_handler);

  signal(SIGINT, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);
  signal(SIGFPE, SIG_IGN);
  signal(SIGHUP, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);


  return(NULL);

}

int main(int argc, char **argv) {
  
  int pid = (int) getpid();
  printf("Process ID: %d\n", pid);

  pthread_t tidA, tidB, tidC, tidD;
  pthread_create(&tidA, NULL, &action, NULL);
  printf("Created thread %d\n", (int)tidA);
  tids[0] = tidA; 

  pthread_create(&tidB, NULL, &action, NULL);
  printf("Created thread %d\n", (int)tidB);
  tids[1] = tidB; 

  pthread_create(&tidC, NULL, &action, NULL);
  printf("Created thread %d\n", (int)tidC);
  tids[2] = tidC; 

  pthread_create(&tidD, NULL, &action, NULL);
  printf("Created thread %d\n", (int)tidD);
  tids[3] = tidD; 

  kill(pid, SIGHUP);
  pthread_kill(tids[1], SIGINT); 

  /* wait for all threads to terminate */
  pthread_join(tidA, NULL);
  printf("Thread %u terminated\n", (int)tidA);
  pthread_join(tidB, NULL);
  printf("Thread %u terminated\n", (int)tidB);
  pthread_join(tidC, NULL);
  printf("Thread %u terminated\n", (int)tidC);
  pthread_join(tidD, NULL);
  printf("Thread %u terminated\n", (int)tidD);

  exit(0);

}