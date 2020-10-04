#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>


#define DAEMON_NAME "nishad-daemon"

// Parent Process(P) => Child Process(C) => Grandchild Process(G)
// New Process session is created with setsid(), for below reasons, we cant call
// setsid() from P.
// when you run a binary in shell terminal, shell executes that binary and make 
// the process a process group leader.
// manpage of setsid() says, it will fail, if the calling process is the process
// group leader.
// setsid() creates new session and make the calling process, session leader.
// We dont want our daemon to run as session leader(just to make sure, there isnt 
// any possibility for our Daemon to get hold of any controlling terminal), 
// hence we call another fork() from Process C and finaly Process G run as our 
// daemon process

void daemonize(void){

	pid_t pid;
	int fd0,fd1,fd2;

	// We are in Process (P), Process (P) is also Process Group Leader
    
	// Set file creation mask to 000 => file permissions = 777
	umask(0);

	// create a new process
	if((pid=fork()) < 0){ // Fork Failed
		exit(1);
	
	}else if(pid > 0){  // Process (P)
		exit(0);
	}
	
	// Now in Process (C)

	// creates a new session and Process (C) is the session leader
	setsid();

	if((pid=fork()) < 0){ // Fork Failed
		exit(1);
	}else if(pid > 0){  // Process (C)
		exit(0);
	}

	// Now in Process (G), we are part of session created by Process (C),
	// which is now an orphaned session due Process (C) exit,
	// Also we(Process (G)) are not the session leader of this session

	// changing working directory to root directory, 
	// Parent Process might have been running on any mounted filesystem location,
	// We, being a daemon is gonna run throghout, so making sure we running from a 
	// directory which will never be unmounted.
	if(chdir("/")<0){
		_exit(1);
	}

	for(int i=0;i<=3;i++){
	    close(i);
	}


	fd0=open("/dev/null",O_RDWR);
	fd1=dup(fd0);
	fd2=dup(fd0);

	openlog(DAEMON_NAME,LOG_CONS,LOG_DAEMON);
	syslog(LOG_INFO,"Started, pid=%d",getpid());

	do{
	}while(1);
	closelog();


}


int main(void){

    daemonize();

    return 0;
}
