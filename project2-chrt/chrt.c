#include <unistd.h>
#include <lib.h>
#include <stdio.h>
#include <sys/time.h>

int chrt(deadline)
long deadline;
{	
	if(deadline == 0){
		return 0;
	}
	message m;
	struct timespec res;
	alarm((unsigned int) deadline);
	clock_gettime(CLOCK_REALTIME,&res);
	deadline=deadline+res.tv_sec;
	m.m2_l1 = deadline ;
	
	return(_syscall(PM_PROC_NR,PM_CHRT,&m));  //return(_syscall(PM_PROC_NR, PM_FORK, &m)); fork的例子

}

