#include <sys/mman.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"
#include <bits/stdc++.h>
#include<fcntl.h>

using namespace std;

int main(int argc, char* argv[]){
	char* addr;
	int fd;
	struct stat sb;
	if(argc!=2 || strcmp(argv[1], "--help")==0){
		cout<<"error in arguments"<<endl;
		return 0;
	}

	fd = open(argv[1],0_RDONLY);

	if(fd==-1){
		cout<<"error opening in data file"<<endl;
		return 0;
	}

	if(fstat(fd,&sb) == -1){
		cout<<"error in fstat"<<endl;
	}

	addr = mmap(NULL,sb.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

	if(addr == MAP_FAILED){
		cout<<"error in mem mapping"<<endl;
		return 0;
	}

	if(write(STDOUT_FILENO,addr,sb.st_size)!=sb.st_size){
		cout<<"partial read write "<<endl;
		return 0;
	}

	exit(EXIT_SUCCESS);
	return 0;

}


