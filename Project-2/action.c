#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#define NLOOP 500
/*
Two threads: each thread will increase the global counter NLOOP times, and print it out with the thread ID.
*/
void sig_handler(int signum) {
printf("sig_handler: Signal: %d /n",signum);
sleep(20);
}

void *doit(void *vptr) {

	int status;sigset_t newmask, oldmask, waiting_mask, look_for_these_child, look_for_these_parent, hblock;
	siginfo_t extra;
	struct sigaction new_action, new_action1, old_action, old_action1;
	memset(&new_action, 0, sizeof(new_action));
	new_action.sa_handler =  sig_handler;
	
	while(1){
	sigaction(SIGILL, &new_action, NULL);
	sigaction(SIGABRT, &new_action, NULL);
	sigaction(SIGSEGV, &new_action, NULL);
	}
	signal(SIGILL, sig_handler);
	signal(SIGABRT, sig_handler);
	signal(SIGSEGV, sig_handler);
	
  
  return(NULL);
}

int main(int argc, char **argv) {

	int status;sigset_t newmask, oldmask, waiting_mask, look_for_these_child, look_for_these_parent, hblock;
	siginfo_t extra;
    /*Maybe get rid of code below
    */
	struct sigaction new_action, new_action1, old_action, old_action1;
	struct sigaction sa;
    sa.sa_handler = &sig_handler;
    sa.sa_flags = SA_RESTART;  
    sigaction(SIGTSTP, &sa, NULL);
	
	new_action.sa_handler =  sig_handler;

  pthread_t tidA, tidB, tidC, tidD;
  pthread_create(&tidA, NULL, &doit, NULL);
  printf("Created thread %d\n", (int)tidA);
  pthread_create(&tidB, NULL, &doit, NULL);
  printf("Created thread %d\n", (int)tidB);
  pthread_create(&tidC, NULL, &doit, NULL);
  printf("Created thread %d\n", (int)tidC);
  pthread_create(&tidD, NULL, &doit, NULL);
  printf("Created thread %d\n", (int)tidD);
  /*wait for both threads to terminate */
  pthread_join(tidA, NULL);
  printf("Thread %d is terminated.\n", (int)tidA);
  pthread_join(tidB, NULL);
  printf("Thread %d is terminated.\n", (int)tidB);
  pthread_join(tidC, NULL);
  printf("Thread %d is terminated.\n", (int)tidC);
  pthread_join(tidD, NULL);
  printf("Thread %d is terminated.\n", (int)tidD);
  
  	memset(&new_action, 0, sizeof(new_action));
  
  	signal(SIGILL, sig_handler);
	signal(SIGABRT, sig_handler);
	signal(SIGSEGV, sig_handler);
  
  
  exit(0);
}
