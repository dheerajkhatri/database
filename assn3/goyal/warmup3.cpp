//main()->listfiles->writefifo()->read()

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

#define SIZE 10

string dep1,dep2,file1,file2;
char arr[SIZE];
int total = 0,fdes[4];
//name->fifoname and path-> data to be written on fifo
bool writefifo(char *name,char *path)
{	
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
	} 
	write(fdw,"#",1);    // adding one extra special character to know all data in fifo are end while reading from fifo
	printf("done writing in fifo %s\n",name);  // otherwise we will not see EOF here because if we close
	return(true);                              // write end here then fifo will be closed and all data 
}                                              // will be destroyed!

bool read()
{
	int k;
	FILE *fp1 = fopen("btech.txt","w");
	FILE *fp2 = fopen("mtech.txt","w");
	FILE *fp3 = fopen("phd.txt","w");
	int fdb = fileno(fp1);  //fileno will give the file desciptor corresponding given file name
	int fdm = fileno(fp2);
	int fdp = fileno(fp3);
	int count;
	char ch[1];
	string name, roll1,roll2;
	bool match;
	int fdrone = fdes[0];       // file descriptors of fifos
	int fdrtwo = fdes[2];
	int fdwone = fdes[1];
	int fdwtwo = fdes[3];
	while(true)
	{
		roll1 = "";
		name = "";
		count = 0;
		while(count<3)
		{
			int fk = read(fdrone,ch,1);
			if(fk==-1)
				printf("error in reading from the fifo\n");
			if(ch[0]=='#')
				break;
			if(ch[0]==','||ch[0]=='\n')
				count++;
			if(count<2&&ch[0]!=',')
			{
				if(count<1)
					roll1.append(1,ch[0]);
				else if(count<2)
					name.append(1,ch[0]);
			}
		}
		if(ch[0]=='#')
			break;
		roll2 = "";
		count = 0;
		match = false;
		while(true)
		{
			int st = read(fdrtwo,ch,1);
			if(st==-1)
				printf("error in reading from the fifo\n");
			write(fdwtwo,ch,1);
			if(ch[0]=='#')
				break;
			if(ch[0]==','||ch[0]=='\n')
				count++;
			if(count<1&&ch[0]!=',')
					roll2.append(1,ch[0]);
			if(count==3)
			{
				if(roll1==roll2)
				{
					match = true;
					break;
				}
				roll2 = "";
				count=0;
			}
		}
		if(match==true)
		{
			if(roll1[2]=='0'&&roll1[3]=='1') k = fdb;
			if(roll1[2]=='4'&&roll1[3]=='1') k = fdm;
			if(roll1[2]=='6'&&roll1[3]=='1') k = fdp;
			write(k,name.c_str(),name.length());
			write(k," ",1);
			write(k,roll1.c_str(),roll1.length());
			write(k,"\n",1);
		}
	}
	close(fdwone); close(fdwtwo); close(fdrone); close(fdrtwo); fclose(fp1); fclose(fp2); fclose(fp3);
	return(true);
}

static void listFiles(const char *dirpath,char *arr1,char *arr2)
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
				if(strcmp(dp->d_name,arr2)==0)
				{
					//write the data to file_pipe2
					writefifo("lab32.txt",c);
				} 
				break;
			case S_IFDIR:
				listFiles(c,arr1,arr2); 
		}
	}
	if(errno!=0);
	//  errExit("readdir");

	if(closedir(dirp)==-1);
	//   errMsg("closedir");
}

int main()
{
	cout<<"enter first dept1 enter file1 enter dept2 enter file2"<<endl;
	cin>>dep1>>file1>>dep2>>file2;

	file1.append(".txt");
	file2.append(".txt");
	char *arr1 = new char[file1.length()+1];
	strcpy(arr1,file1.c_str());
	char *arr2 = new char[file2.length()+1];
	strcpy(arr2,file2.c_str());
	listFiles("database",arr1,arr2);
	read();
	return 0;
}
