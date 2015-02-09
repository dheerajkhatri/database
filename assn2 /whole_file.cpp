#include <sys/wait.h>
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                            plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <bits/stdc++.h>
#include <dirent.h>
#include <iostream>

#define file1 "./database/CSE/cs561.txt"
#define file2 "./database/CL/cl613.txt"
using namespace std;

#define S_SIZE 100000

map<string,string> btech;
map<string,string> mtech;
map<string,string>phd;


void genFiles(){
		ofstream fp1,fp2,fp3;	
		fp1.open("btech.txt");
		fp2.open("mtech.txt");
		fp3.open("phd.txt");

		for(map<string,string>::iterator b = btech.begin();b!=btech.end();++b){
			fp1<<(b->first)<<" => "<<(b->second)<<endl;
		}
		//cout<<"-------------------------------------------------------------------------------------"<<endl;
		for(map<string,string>::iterator m = mtech.begin();m!=mtech.end();m++){
			fp2<<(m->first)<<" => "<<(m->second)<<endl;
		}
		//cout<<"-------------------------------------------------------------------------------------"<<endl;
		for(map<string,string>::iterator p = phd.begin();p!=phd.end();p++){
			fp3<<(p->first)<<" => "<<(p->second)<<endl;
		}
		//cout<<"-------------------------------------------------------------------------------------"<<endl;



}

int main(int argc, char* argv[]){

	string line;
	char store[S_SIZE];
	int i,j,pfd[2],cCounter,pCounter;
	ssize_t numRead,numWrite;
	

	if(pipe(pfd)==-1){
		cout<<"error in creating pipe\n";
	}

	switch(fork()){
		case -1:
			cout<<"error in fork()\n";
		return 0;

		case 0:
			if(close(pfd[1])==-1){
				cout<<"can't close child write end"<<endl;
			}

			numRead = read(pfd[0],store,S_SIZE);
				
			if(numRead == -1){
				cout<<"error while reading\n";
				return 0;
			}

			if(numRead==0)
				break;                                //cout<<"End of file occured\n";

			else{

				for(j=0;j<strlen(store);j++){   
					
					int start;
					start = j;					
					while(store[j]!=',' || j>=strlen(store))j++;					
					char tempstr[100];
					strncpy(tempstr,store+start,j-start);
					string rollno(tempstr);
					//cout<<"rollno start and len are "<<start<<" "<<j-start<<endl;
					

					j++;
					start = j;
					while(store[j]!=',' || j>=strlen(store))j++;
					char tempstr2[100];
					strncpy(tempstr2,store+start,j-start);
					string name(tempstr2);
					//cout<<"name start and len are "<<start<<" "<<j-start<<endl;
					j++;

					//cout<<rollno<<" => "<<name<<endl;
					//cout<<rollno[3]<<rollno[4]<<endl;
					if(rollno[2]=='0' && rollno[3]=='1'){
						btech[rollno] = name;
						//cout<<"btech\n";
						//cout<<btech[rollno]<<endl;
					}else if(rollno[2]=='4' && rollno[3]=='1'){
						mtech[rollno] = name;
						//cout<<"mtech\n";
						//cout<<mtech[rollno]<<endl;
					}else if(rollno[2]=='6' && rollno[3]=='1'){
						phd[rollno] = name;
						//cout<<"ptech\n";
						//cout<<phd[rollno]<<endl;
					}


					while(store[j]!='\n')j++;
					

				}
			}

			genFiles();

			if(close(pfd[0])==-1){
				cout<<"can't close child read end"<<endl;
			}

			wait(NULL);
			_exit(EXIT_SUCCESS);

		break;

		default:
		
			if(close(pfd[0])==-1){
				cout<<"can't close parent read end"<<endl;
				return 0;
			}

			ifstream t1(file1);
			ifstream t2(file2);

			std::stringstream buffer1;
			std::stringstream buffer2;
			buffer1 << t1.rdbuf();
			buffer2 << t2.rdbuf();
			string content1,content2;
			content1 = buffer1.str();
			content2 = buffer2.str();

			/*for(int a=0;a<content.length();a++){
				if(content[a]=='\n')
					continue;
				//cout<<content[a]<<endl;
			}*/

			numWrite = write(pfd[1],content1.c_str(),content1.length());
			numWrite = write(pfd[1],content2.c_str(),content2.length());
			if(numWrite == -1){
				cout<<"error while writing to pipe"<<endl;
				return 0;
			}

			if(numWrite==0)
				break;                                //cout<<"End of file occured\n";
				wait(NULL);	

			if(close(pfd[1])==-1){
				cout<<"error closing child write end\n";
				return 0;
			}
			exit(EXIT_SUCCESS);
		}


		cout<<"done\n";



	return 0;
}