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

char dep1[15],dep2[15],crs1[15],crs2[15];
char arr[SIZE];
int total = 0,fdes[4];

bool writefifo(char *name,char *path)
{	
	unlink(name);
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
	int fd3 = open(path,O_RDONLY);
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
		write(fdw,arr,status);
	}
	write(fdw,"#",1);
	printf("done writing in fifo %s\n",name);
	return(true);
}

bool read()
{
	int k;
	FILE *fp1 = fopen("btech.txt","w");
	FILE *fp2 = fopen("mtech.txt","w");
	FILE *fp3 = fopen("phd.txt","w");
	int fdb = fileno(fp1);
	int fdm = fileno(fp2);
	int fdp = fileno(fp3);
	int count;
	char ch[1];
	string name, roll1,roll2;
	bool match;
	int fdrone = fdes[0];
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
	count = 0;
	roll1 = "";
	name = "";
	while(true)
	{
		int fk = read(fdrtwo,ch,1);
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
		if(count==3)
		{
			if(roll1[2]=='0'&&roll1[3]=='1') k = fdb;
			if(roll1[2]=='4'&&roll1[3]=='1') k = fdm;
			if(roll1[2]=='6'&&roll1[3]=='1') k = fdp;
			write(k,name.c_str(),name.length());
			write(k," ",1);
			write(k,roll1.c_str(),roll1.length());
			write(k,"\n",1);
			roll1 = "";
			name = "";
			count = 0;
		}
	
	}
	close(fdwone);
	close(fdwtwo);
	close(fdrone);
	close(fdrtwo);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
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
		dp = readdir(dirp);
		if(dp == NULL)
			break;
		if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..") ==0)
			continue;

		//if(!isCurrent)
		//  printf("%s/",dirpath);

		string str1(dirpath);
		string str2(dp->d_name);
		string str3 = "/";
		string str = str1 + str3 + str2;
		char* c = new char[str.length()+1];
		c[str.length()] = '\0';
		strcpy(c,str.c_str());
		if(stat(c,&sb)==-1)
			printf("error in stat\n");
		switch(sb.st_mode & S_IFMT)
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
	scanf("%s%s%s%s",dep1,crs1,dep2,crs2);
	crs1[strlen(crs1)] = '.';
	crs1[strlen(crs1)] = 't';
	crs1[strlen(crs1)] = 'x';
	crs1[strlen(crs1)] = 't';
	crs2[strlen(crs2)] = '.';
	crs2[strlen(crs2)] = 't';
	crs2[strlen(crs2)] = 'x';
	crs2[strlen(crs2)] = 't';
	char *arr1 = crs1;
	char *arr2 = crs2;
	listFiles("database",arr1,arr2);
	read();
	return 0;
}