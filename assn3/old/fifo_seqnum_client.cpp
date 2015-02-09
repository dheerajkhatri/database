#include "fifo_seqnum.h"
#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h>
#include "get_num.h"
using namespace std;

static char clientFifo[CLIENT_FIFO_NAME_LEN];

static void removeFIfo(void){
	unlink(clientFifo);
}

int main(int argc,char*argv[]){

	int serverFd,clientFd;
	struct request req;
	struct response resp;
	
	if(argc>1 && strcmp(argv[1],"--help") == 0){
		cout<<"show help from man page"<<endl;
		return 0;
	}

	umask(0);

	snprintf(clientFifo , CLIENT_FIFO_NAME_LEN , CLIENT_FIFO_TEMPLATE , (long)getpid());

	if(mkfifo(clientFifo,S_IRUSR | S_IWUSR | S_IWGRP )==-1){
		cout<<"error in making client fifo"<<endl;
	}

	if(atexit(removeFIfo) != 0 ){
		cout<<"error at exit"<<endl;
		return 0;
	}

	req.pid = getpid();
	req.seqLen = (argc>1)? getInt(argv[1],GN_GT_0,"seq-len"):1;

	serverFd = open(SERVER_FIFO,O_WRONLY);

	if(serverFd == -1){
		cout<<"error in opening server fifo"<<endl;
		return 0;
	}

	if(write(serverFd,&req,sizeof(struct request)) != sizeof(struct request)){
		cout<<"error in writing to server fifo"<<endl;
		return 0;
	}

	clientFd = open(clientFifo,O_RDONLY);
	if(clientFd == -1){
		cout<<"error in opening client fifo"<<endl;
		return 0;
	}

	if(read(clientFd,&resp,sizeof(struct response))!=sizeof(struct response)){
		cout<<"error in reading from client fifo"<<endl;
	}

	cout<<resp.seqNum<<endl;
	
	exit(EXIT_SUCCESS);

}