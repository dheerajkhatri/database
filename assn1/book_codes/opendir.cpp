#include <dirent.h>
#include <iostream>
using namespace std;
int main()
{
DIR * d;
dirent * p;
d=opendir("/Home/Academic");

while(1)
{
	p=readdir(d );
	if(p==NULL)
	{
		break;
	}
	else

	{
		cout<<p->d_ino;
	}
}
}