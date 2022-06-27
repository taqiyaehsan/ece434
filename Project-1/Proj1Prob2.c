#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SLEEP 5

int main() {

	int status, res;
	pid_t a_pid, b_pid, c_pid, d_pid;
	int a_ret, b_ret, c_ret, d_ret;
	int a, b, c, d;
	int fd[2], pd[2];
	char cmd[200];

	int original = (int)getpid();

	pipe(fd); pipe(pd);

	// Process A
	a_pid = fork();

	if (a_pid == 0) {
		a = (int)getpid();
		write(fd[1], &a, sizeof(a));
		printf("I'm process A (%d), the root of the tree\n",a);

		// Process C
		c_pid = fork();

		if (c_pid == 0) {
			c = (int)getpid();
			write(fd[1], &c, sizeof(c));
			printf("I'm process C (%d), leaf of process A (%d)\n",(int)getpid(),(int)getppid());

			sleep(SLEEP);
			exit(6);
		}

		else {
			// Process B
			b_pid = fork();

			if (b_pid == 0) {
				b = (int)getpid();
				write(fd[1], &b, sizeof(b));
				printf("I'm process B (%d), leaf of process A (%d)\n",(int)getpid(),(int)getppid());

				// Process D
				d_pid = fork();

				if (d_pid == 0) {

					d = (int)getpid();
					printf("I'm process D (%d), leaf of process B (%d)\n",(int)getpid(),(int)getppid());
					write(fd[1], &d, sizeof(d));

					sprintf(cmd, "pstree -np -C age %d", a);
					system(cmd);
					if (res<0) fprintf(stdout,"The pstree command did not execute\n");

					sleep(SLEEP);
					exit(10);
				}

				else {
					sleep(SLEEP);
					waitpid(d_pid, &status, 0); // wait for D to terminate
					d_ret = WEXITSTATUS(status);

					write(pd[1], &d_ret, sizeof(d_ret));
					printf("Process D (%d), child of Process B (%d) has terminated with return code %d\n", (int)d_pid, (int)getppid(), d_ret);
				
				}
				exit(4);
			}

			else {
				sleep(SLEEP);
				waitpid(b_pid, &status, 0); // wait for B to terminate
				b_ret = WEXITSTATUS(status);
				write(pd[1], &b_ret, sizeof(b_ret));

				printf("Process B (%d), child of Process A (%d), has terminated with return code %d\n", (int)b_pid, (int)getppid(), b_ret);

				waitpid(c_pid, &status, 0); // wait for C to terminate
				c_ret = WEXITSTATUS(status);
				write(pd[1], &c_ret, sizeof(c_ret));

				printf("Process C (%d), child of Process A (%d), has terminated with return code %d\n", (int)c_pid, (int)getppid(), c_ret);
			
			}
		}

		exit(2);
	}

	else {

		waitpid(a_pid, &status, 0); // wait for A to terminate
		a_ret = WEXITSTATUS(status);
		write(pd[1], &a_ret, sizeof(a_ret));

		printf("Process A (%d) has terminated with return code %d\n", (int)a_pid, a_ret);
	}
	
	read(fd[0], &a, sizeof(a)); read(pd[0], &a_ret, sizeof(a_ret));		
	read(fd[0], &b, sizeof(b)); read(pd[0], &b_ret, sizeof(b_ret));
	read(fd[0], &c, sizeof(c)); read(pd[0], &c_ret, sizeof(c_ret));
	read(fd[0], &d, sizeof(d)); read(pd[0], &d_ret, sizeof(d_ret));

	// printf("\n\nA(%d, return value %d) -> C(%d, return value %d)\n|-> B(%d, return value %d) -> D(%d, return value %d)\n", a, a_ret, c, c_ret, b, b_ret, d, d_ret);

	return (0);
} 
