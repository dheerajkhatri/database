#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;

#define MAXLINE 100

static void displayCmdFmt(void)
{
	printf("\nFormat: cmd lock start length [whence]\n\n");
	printf("'cmd' is 'g' (GETLK), 's' (SETLK), or 'w' (SETLKW)\n");
	printf("'lock' is 'r' (READ), 'w' (WRITE), or 'u' (UNLOCK)\n");
	printf("'start' and 'length' specify byte range to lock\n");
	printf("'whence' is 's' (SEEK_SET, default), 'c' (SEEK_CUR), ""or 'e' (SEEK_END)\n\n");
}


int main(int argc,char*argv[]){
	int fd,numRead,cmd,status;
	char lock,cmdCh,whence,line[MAXLINE];
	struct flock fl;
	long long len, st;

	if(argc!=2){
		cout<<"error in arguments"<<endl;
		return 0;
	}

	fd = open(argv[1],O_RDWR);
	if(fd==-1){
		cout<<"error openinng fd"<<endl;
		return 0;
	}

	cout<<"enter ? for hewlp"<<endl;

	for(;;){
		cout<<getpid()<<endl;
		fflush(stdout);
		
		if(fgets(line,MAXLINE,stdin)==NULL){
			cout<<"cmds not given properly"<<endl;
		}

		line[strlen(line)-1] = '\0'; //remove \n at the end
		
		if(*line == '\0')
			continue;

		if(line[0]=='?'){
			displayCmdFmt();
			continue;
		}

		whence = 's';
		numRead = sscanf(line, "%c %c %lld %lld %c", &cmdCh, &lock,&st, &len, &whence);
		fl.l_start = st;
		fl.l_len = len;

		//strchr Returns a pointer to the first occurrence of character in the C string str.
		if (numRead < 4 || strchr("gsw", cmdCh) == NULL ||strchr("rwu", lock) == NULL || strchr("sce", whence) == NULL) {
			printf("Invalid command!\n");
			continue;
		}

		cmd = (cmdCh == 'g') ? F_GETLK : (cmdCh == 's') ? F_SETLK : F_SETLKW;
		fl.l_type = (lock == 'r') ? F_RDLCK : (lock == 'w') ? F_WRLCK : F_UNLCK;
		fl.l_whence = (whence == 'c') ? SEEK_CUR :
		(whence == 'e') ? SEEK_END : SEEK_SET;
		status = fcntl(fd, cmd, &fl); /* Perform request... */


		if (cmd == F_GETLK) {  	 /* ... and see what happened */
			if (status == -1) {
				cout<<"fcntl - F_GETLK"<<endl;
				return 0;			
			} else {
				if (fl.l_type == F_UNLCK)
					printf("[PID=%ld] Lock can be placed\n", (long) getpid());
				else             /* Locked out by someone else */
					printf("[PID=%ld] Denied by %s lock on %lld:%lld ""(held by PID %ld)\n", (long) getpid(),(fl.l_type == F_RDLCK) ? "READ" : "WRITE",(long long) fl.l_start,(long long) fl.l_len, (long) fl.l_pid);
			}
		} else {      /* F_SETLK, F_SETLKW */		 
			if (status == 0)
				printf("[PID=%ld] %s\n", (long) getpid(),(lock == 'u') ? "unlocked" : "got lock");
			else if (errno == EAGAIN || errno == EACCES)  /* F_SETLK */		 
				printf("[PID=%ld] failed (incompatible lock)\n",(long) getpid());
			else if (errno == EDEADLK)   /* F_SETLKW */		 
					printf("[PID=%ld] failed (deadlock)\n", (long) getpid());
			else{
				cout<<"fcntl - F_SETLK(W)"<<endl;
				return 0;
			}
		}

	}


	return 0;
}