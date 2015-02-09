//main()->listfiles->writefifo()->read()
//given file name in input without .txt
#define _BSD_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "file_perms.h"
#include "tlpi_hdr.h"
#include <dirent.h>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<sstream>
#include<fcntl.h>

using namespace std;

#define SIZE 1

string dep1,file1;
char arr[SIZE];
int total = 0,fdes[2];
//string output;
//name->fifoname and path-> data to be written on fifo
// lab31.txt  database/CSE/cs101.txt
bool writefifo(char *name,char *path)
{	
	//cout<<"in writefifo "<<name <<" path "<<path<<endl;
	unlink(name);  // if file alrady exist removes hard link if we don't do file already exist error
	if(mkfifo(name,S_IRUSR|S_IWUSR|S_IWGRP)==-1) 
		printf("error in creating the fifo\n");
	int fdr = open(name,O_RDONLY|O_NONBLOCK);
	fdes[total++] = fdr; 
	if(fdr==-1)
		printf("error in opening the read end of the fifo\n");
	int fdw = open(name,O_WRONLY);
	fdes[total++] = fdw;
	if(fdw==-1)
		printf("error in openeing the write end of the fifo\n");
	int fd3 = open(path,O_RDONLY);  //here opening the file
	if(fd3==-1)	
		printf("error in opening the file\n");
	while(true)
	{
		int status = read(fd3,arr,SIZE);				
		if(status==-1)
		{
			printf("error in reading from the file\n");
			break;
		}
		if(status==0)
			break;
		write(fdw,arr,status);  // here status is the BUF size		
		//cout<<arr<<endl;
	} 
	write(fdw,"#",1);    // adding one extra special character to know all data in fifo are end while reading from fifo
	printf("done writing in fifo %s\n",name);  // otherwise we will not see EOF here because if we close
	return(true);                              // write end here then fifo will be closed and all data 
}                                              // will be destroyed!

bool read_and_sort()
{		
	int k,ok,count,out=0;	
	char ch[1];
	string name, roll, webmail,first;
	bool is_last;	
	int fdrone = fdes[0];       // read descriptors of fifos
	int fdwone = fdes[1];	    // write descriptors of fifos
	char newline[1],comma[1],tempchar,tempchar2[1],hash[1];
	newline[0] = '\n';
	comma[0] = ',';	
	hash[0] = '#';
	string tfifo = "tempfifo.txt";
	const char* tempfifo = tfifo.c_str();
	unlink(tempfifo);

	

	if(mkfifo(tempfifo,S_IRUSR|S_IWUSR|S_IWGRP)==-1) 
		printf("error in creating the fifo\n");
	int tfdr = open(tempfifo,O_RDONLY|O_NONBLOCK);
	if(tfdr==-1)
		printf("error in opening the read end of the fifo\n");
	int tfdw = open(tempfifo,O_WRONLY);	
	if(tfdw==-1)
		printf("error in openeing the write end of the fifo\n");


	
	while(true)
	{
		//cout<<"true"<<endl;
		roll = "";	name = ""; webmail = ""; first = ""; count = 0;
		int lol2 = read(fdrone,tempchar2,1);
		if(lol2==0)break;  // only # remain in fifo
		else{	
			first.clear();
			roll.clear();
			roll.append("1");			
			first = tempchar2[0];
			//cout<<"first and ch[0] are "<<first<<"   "<<tempchar2[0]<<endl;
			roll.append(first);
			//cin>>ok;
		}
		while(count<3)
		{
			//cout<<"in while"<<endl;
			int fk = read(fdrone,ch,1);
			if(fk==-1){
				//printf("error in reading from the fifo\n");	
				write(tfdw,hash,1);
				out = 1;
			}
			if(ch[0]=='#')	
				break;
			if(ch[0]==','||ch[0]=='\n')
				count++;
			if(ch[0]!=',')
			{
				//cout<<ch[0]<<endl;
				if(count<1)
					roll.append(1,ch[0]);
				else if(count<2)
					name.append(1,ch[0]);
				else if(count<3)
					webmail.append(1,ch[0]);
			}
		}
		if(out)break;
		//if(ch[0]=='#')break;
		// write again to fifo if not last line (just before #)
		//cout<<"-----------------------"<<roll<<endl;
		int lol = read(fdrone,ch,1);
		if(ch[0]=='#'){
			//put in tempfifo			
			char* croll = new char[roll.length()+1]; strcpy(croll,roll.c_str());
			char* cname = new char[name.length()+1]; strcpy(cname,name.c_str());
			char* cwebmail = new char[webmail.length()+1]; strcpy(cwebmail,webmail.c_str());
			write(tfdw,croll,roll.length());        write(tfdw,comma,1);
			write(tfdw,cname,name.length());        write(tfdw,comma,1);
			write(tfdw,cwebmail,webmail.length());  write(tfdw,newline,1);
			write(fdwone,hash,1);
			cout<<"temp fifo written "<<roll<<" "<<name<<" "<<webmail<<endl;

		}else{
			//put again in same fifo
			char* croll = new char[roll.length()+1]; strcpy(croll,roll.c_str());
			char* cname = new char[name.length()+1]; strcpy(cname,name.c_str());
			char* cwebmail = new char[webmail.length()+1]; strcpy(cwebmail,webmail.c_str());
			write(fdwone,croll,roll.length());        write(fdwone,comma,1);
			write(fdwone,cname,name.length());        write(fdwone,comma,1);
			write(fdwone,cwebmail,webmail.length());  write(fdwone,newline,1);
			//cout<<"main fifo written "<<roll<<" "<<name<<" "<<webmail<<endl;
		}

		count = 0;
		//cout<<"here"<<endl;
		//cout<<roll<<endl;	
	}
	

	//AGAIN WRITE TO MAIN FIFO

	while(true) {
		roll = "";
		name = "";
		webmail = "";
		count = 0;
		while(count<3) {
			int fk = read(tfdr,ch,1);
			if(fk==-1)
			printf("error in reading from the fifo\n");
			if(ch[0]=='#')
				break;
			if(ch[0]==','||ch[0]=='\n')
				count++;
			if(count<2&&ch[0]!=',')
			{
				if(count<1)
				roll.append(1,ch[0]);
				else if(count<2)
				name.append(1,ch[0]);
			}
		}
		if(ch[0]=='#')
		break;

		char* ccroll = new char[roll.length()+1]; strcpy(ccroll,roll.c_str());
		char* ccname = new char[name.length()+1]; strcpy(ccname,name.c_str());
		char* ccwebmail = new char[webmail.length()+1]; strcpy(ccwebmail,webmail.c_str());
		write(fdwone,ccroll,roll.length());        write(fdwone,comma,1);
		write(fdwone,ccname,name.length());        write(fdwone,comma,1);
		write(fdwone,ccwebmail,webmail.length());  write(fdwone,newline,1);
	}



	close(fdwone); close(fdrone); close(tfdr); close(tfdw);
	return(true);
}

static void listFiles(const char *dirpath,char *arr1)
{
	DIR *dirp;
	struct stat sb;
	struct dirent *dp;
	//Boolean isCurrent;  /*true if dirpath is current directory*/
	bool isCurrent = strcmp(dirpath,".") == 0;
	dirp = opendir(dirpath);
	if(dirp==NULL)
	{
		//errMsg("opendir failed on '%s'",dirpath);
		return;
	}
	for(;;)
	{
		errno = 0;
		dp = readdir(dirp);  // each call to readdir reads next dire. from the direc. stream refer to by dirp
		if(dp == NULL)
			break;
		if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..") ==0)
			continue;

		//if(!isCurrent)
		//  printf("%s/",dirpath);

		string str1(dirpath);
		string str2(dp->d_name);
		string str3 = "/";
		string str = str1 + str3 + str2;   //ie:  c = database/CSE
		char* c = new char[str.length()+1];
		c[str.length()] = '\0';
		strcpy(c,str.c_str());
		if(stat(c,&sb)==-1)
			printf("error in stat\n");
		switch(sb.st_mode & S_IFMT)    // for FIFO S_IFIFO  
		{
			case S_IFREG:
				if(strcmp(dp->d_name,arr1)==0)
				{
					//write the data to file_pipe1
					writefifo("lab31.txt",c);										
				}
				break;
			case S_IFDIR:
				listFiles(c,arr1); 
		}
	}
	if(errno!=0);
	//  errExit("readdir");

	if(closedir(dirp)==-1);
	//   errMsg("closedir");
}

int main()
{
	cout<<"enter first dept1 enter file1 (without .txt) enter"<<endl;
	cin>>dep1>>file1;
	file1.append(".txt");	

	char *arr1 = new char[file1.length()+1];
	strcpy(arr1,file1.c_str());

	listFiles("database",arr1);
	read_and_sort();
	return 0;
}
