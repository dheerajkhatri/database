#include <signal.h>
#include "fifo_seqnum.h"
#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc,char*argv[]){

	int serverFd,clientFd,dummyFd;
	char clientFifo[CLIENT_FIFO_NAME_LEN];
	struct request req;
	struct response resp;
	int seqNum = 0;

	umask(0);

	if(mkfifo(SERVER_FIFO,S_IRUSR | S_IWUSR | S_IWGRP )==-1){
		cout<<"error in making server fifo"<<endl;
	}

	serverFd = open(SERVER_FIFO,O_RDONLY);

	if(serverFd==-1){
		cout<<"error opening server fifo"<<endl;
	}

	dummyFd = open(SERVER_FIFO,O_WRONLY);

	if(dummyFd == -1){
		cout<<"error opening dummy fifo "<<endl;
	}

	if(signal(SIGPIPE,SIG_IGN) == SIG_ERR){
		cout<<"error signal"<<endl;
	}

	for(;;){
		if(read(serverFd , &req , sizeof(request))!=sizeof(struct request)){
			cout<<"error in reading from server fifo"<<endl;
			continue;
		}

		snprintf(clientFifo , CLIENT_FIFO_NAME_LEN , CLIENT_FIFO_TEMPLATE , (long)req.pid);
		
		clientFd = open(clientFifo,O_WRONLY);
		
		if(clientFd==-1){
			cout<<"error in opening client fifo"<<endl;
			continue;
		}

		resp.seqNum = seqNum;
		if(write(clientFd,&resp,sizeof(struct response))!=sizeof(struct response)){
			cout<<"error in writing to client fifo"<<endl;
		}

		if(close(clientFd)==-1){
			cout<<"error in closing client fd"<<endl;
		}

		seqNum += req.seqLen;

	}


}