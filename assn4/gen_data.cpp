#include <bits/stdc++.h>
#include <time.h>
using namespace std;

#define MAX 100000

int main(int argc, char* argv[]){
	
	if(argc==1)	{
		cout<<"give file name in argument"<<endl;
		return 0;
	}

	ofstream ofs(argv[1],ofstream::out);

	srand (time(NULL));

	for(int i=0;i<1000;i++){
		ofs<<rand()%MAX<<endl;		
	}
	
	return 0;
}