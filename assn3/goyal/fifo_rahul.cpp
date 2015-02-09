#include <bits/stdc++.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#define BUF_SIZE 1
using namespace std;
int main()
{
	string a,b;
	cin>>a>>b;
	//cout<<a<<"\n"<<b<<endl;
	ifstream f1,f2;
	f1.open((char*)a.c_str());
	f2.open((char*)b.c_str());
	string s;
	char buf[BUF_SIZE],pub[BUF_SIZE];
	ssize_t num1,num2;
	char*p;
	int fifo1,fifo2;
	
	umask(0);
	if(mknod("fifo_1", S_IFIFO|0666, 0)==-1)
	{
		cout<<"error creating client fifo\n";
		return 0;
	}
	umask(0);
	if(mknod("fifo_2", S_IFIFO|0666, 0)==-1)
	{
		cout<<"error creating client fifo\n";
		return 0;
	}
	
	int Fd1=open("fifo_1",O_RDONLY|O_NONBLOCK);
	int Fd2=open("fifo_2",O_RDONLY|O_NONBLOCK);
	//cout<<a<<"hello\n";
	fifo1=open("fifo_1",O_WRONLY);
//	cout<<"hello\n";
	while(getline(f1,s))
	{
		if(s!=""){
		p=(char*)s.c_str();
		//cout<<p<<endl;
		write(fifo1,p,strlen(p));
		write(fifo1,"#",1);
		}
	}
	write(fifo1,"@",1);
	f1.close();
//	cout<<"hello\n";	
	fifo2=open("fifo_2",O_WRONLY);
	while(getline(f2,s))
	{
		if(s!=""){
		p=(char*)s.c_str();
		write(fifo2,p,strlen(p));
		write(fifo2,"#",1);
		}
	}
	write(fifo2,"@",1);
	f2.close();

	ofstream f3,f4,f5;
	f3.open("btech.txt");
	f4.open("mtech.txt");
	f5.open("phd.txt");
	string file_1="",file_2="";
	char*file1_roll,*file1_name,*file2_roll,*file2_name;
	char ck[1];
	
	while(1)
	{
		file_1="";
		num1=read(Fd1,ck,1);
		while(ck[0]!='@' && ck[0]!='#')
		{
			file_1.push_back(ck[0]);
			num1=read(Fd1,ck,1);
		}
		if(ck[0]=='@')
		{
			cout<<"No more same data\n";
			close(fifo1);
			close(fifo2);		
			close(Fd1);
			close(Fd2);
			unlink("fifo_1");
			unlink("fifo_2");
			return 0;
		}
		file1_roll=strtok((char*)file_1.c_str(),",");
		file1_name=strtok(NULL,",");
		while(1)
		{
			file_2="";
			num2=read(Fd2,ck,1);
			while(ck[0]!='#' && ck[0]!='@')
			{
				file_2.push_back(ck[0]);
				num2=read(Fd2,ck,1);
			}
			if(ck[0]=='#')
			{
				p=(char*)file_2.c_str();
				write(fifo2,p,strlen(p));
				write(fifo2,"#",1);
			}
			if(ck[0]=='@')
			{
				write(fifo2,"@",1);
				break;
			}
			file2_roll=strtok((char*)file_2.c_str(),",");
			file2_name=strtok(NULL,",");
			if(strcmp(file1_roll,file2_roll)==0)
			{
				if(file1_roll[2]=='0')
					f3<<file1_roll<<" \t"<<file1_name<<endl;
				if(file1_roll[2]=='4' && file1_roll[3]=='1')
					f4<<file1_roll<<" \t"<<file1_name<<endl;
				if(file1_roll[2]=='6')
					f5<<file1_roll<<" \t"<<file1_name<<endl;
			}
		}
	}
}