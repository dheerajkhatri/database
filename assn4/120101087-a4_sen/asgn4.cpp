#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
using namespace std;
map<string,string> cname;   //course
map<string,vector<string> > stu;
map<string,int> dpcoden;
vector<string> paths;
static int getType(const struct stat *sb)   //return 0 if regular 1 if directory
{
    switch(sb->st_mode & S_IFMT){
    case S_IFREG: return 0;
    case S_IFDIR: return 1;
    default: printf("unknown file type?\n"); break;
    }
}
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

static void listFiles2(const char *dirpath)
{
    char str[255] ;
    char roll[20];
    char core[10];
    int len;
DIR *dirp;
    struct dirent *dp;
    bool isCurrent;  /*true if dirpath is current directory*/
    isCurrent = strcmp(dirpath,".") == 0;
    dirp = opendir(dirpath);
    if(dirp==NULL)
    {
        //errMsg("opendir failed on '%s'",dirpath);
        return;
    }

    for(;;)
    {
        strcpy(str,dirpath);
        len = strlen(str);
        str[len] = '/';
        str[len+1] = '\0';
        errno = 0;
        dp = readdir(dirp);
        if(dp == NULL)
            break;
        if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..") ==0)
            continue;

        int i;
        for( i = 0 ; i<12 ; i++)
        {
            if((dp->d_name)[i]=='.')
            break;
            else
            {
                core[i] = dp->d_name[i];
            }
        }
        core[i] = '\0';
        strcat(str,dp->d_name);
        string course_name(str);    //changed
      //  cout<<str<<" "<<course_name<<endl;

        if(str[strlen(str)-1]!='t')
            continue;
        else
            paths.push_back(course_name);
    }
        if(errno!=0);
      //  errExit("readdir");

    if(closedir(dirp)==-1);
}

static void listDir2(const char *dirpath)
{

    DIR *dirp;
    struct dirent *dp;
    string str = "database/";
    bool isCurrent;  /*true if dirpath is current directory*/
    isCurrent = strcmp(dirpath,".") == 0;
    dirp = opendir(dirpath);
    struct stat sb;
    if(dirp==NULL)
    {
        //errMsg("opendir failed on '%s'",dirpath);
        return;
    }
    for(;;)
    {
        char str[100] = "database/";
        errno = 0;
        dp = readdir(dirp);
        if(dp == NULL)
            break;
        if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..") ==0)
            continue;
        strcat(str,dp->d_name);
        //cout<<(str)<<endl;

        if(stat(str,&sb)==-1)
        {
            printf("error in stat\n");
            return ;
        }
        else
        {
            if(getType(&sb)==1) //directory then traverse
            {
                //cout<<"It is directory"<<endl;
                listFiles2(str);
            }
            else
            {
               // cout<<"It is file"<<endl;
            }
        }
//dp->d_name
    }
    if(errno!=0);
      //  errExit("readdir");

    if(closedir(dirp)==-1);
     //   errMsg("closedir");
}



static void listFiles(const char *dirpath)
{
    char str[255] ;
    char roll[20];
    char core[10];
    int len;
    vector<string> x;
    string line;
    ifstream myfile;
    DIR *dirp;
    struct dirent *dp;
    bool isCurrent;  /*true if dirpath is current directory*/
    isCurrent = strcmp(dirpath,".") == 0;
    dirp = opendir(dirpath);
    if(dirp==NULL)
    {
        //errMsg("opendir failed on '%s'",dirpath);
        return;
    }
    map<string,vector<string> >::iterator it;
    for(;;)
    {
        strcpy(str,dirpath);
        len = strlen(str);
        str[len] = '/';
        str[len+1] = '\0';
        errno = 0;
        dp = readdir(dirp);
        if(dp == NULL)
            break;
        if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..") ==0)
            continue;

        int i;
        for( i = 0 ; i<12 ; i++)
        {
            if((dp->d_name)[i]=='.')
            break;
            else
            {
                core[i] = dp->d_name[i];
            }
        }
        core[i] = '\0';
        strcat(str,dp->d_name);
        string course_name(core);
      //  cout<<str<<" "<<course_name<<endl;

        if(str[strlen(str)-1]!='t')
            continue;

        ifstream myfile (str);
    if (myfile.is_open())
      {
        while ( getline (myfile,line) )
        {
          int i;
          for( i = 0 ; i< line.length() ;i++)
          {
            if(line[i]!=',')
            {
                roll[i] = line[i];
            }
            else
            break;
          }
          roll[i] = '\0';
          if(strlen(roll)>6 && roll[0]!=' ' )                    ///CHECK
          {
            string ro(roll);
          //  cout<<ro<<endl;
            it = stu.find(ro);
            if(it==stu.end())
            {
                vector<string> v;

                v.push_back(course_name);
                stu.insert(make_pair(ro,v));
            }
            else
            {
                (it->second).push_back(course_name);
            }
          }
          else
          {
            //cout<<roll;
            //cin>>i;
          }

        }
        myfile.close();
      }
      else
      {
        cout << "Unable to open file";
      }
    }
    if(errno!=0);
      //  errExit("readdir");

    if(closedir(dirp)==-1);
     //   errMsg("closedir");
}

static void listDir(const char *dirpath)
{

    DIR *dirp;
    struct dirent *dp;
    string str = "database/";
    bool isCurrent;  /*true if dirpath is current directory*/
    isCurrent = strcmp(dirpath,".") == 0;
    dirp = opendir(dirpath);
    struct stat sb;
    if(dirp==NULL)
    {
        //errMsg("opendir failed on '%s'",dirpath);
        return;
    }
    for(;;)
    {
        char str[100] = "database/";
        errno = 0;
        dp = readdir(dirp);
        if(dp == NULL)
            break;
        if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..") ==0)
            continue;
        strcat(str,dp->d_name);
        //cout<<(str)<<endl;

        if(stat(str,&sb)==-1)
        {
            printf("error in stat\n");
            return ;
        }
        else
        {
            if(getType(&sb)==1) //directory then traverse
            {
                //cout<<"It is directory"<<endl;
                listFiles(str);
            }
            else
            {
               // cout<<"It is file"<<endl;
            }
        }
//dp->d_name
    }
    if(errno!=0);
      //  errExit("readdir");

    if(closedir(dirp)==-1);
     //   errMsg("closedir");
}
void initialize()
{
    dpcoden["01"] = 1;
    dpcoden["02"] = 2;
    dpcoden["03"] = 3;
    dpcoden["04"] = 4;
    dpcoden["05"] = 5;
    dpcoden["06"] = 6;
    dpcoden["07"] = 7;
    dpcoden["08"] = 8;
    dpcoden["21"] = 21;
    dpcoden["22"] = 22;
    dpcoden["23"] = 23;
    dpcoden["41"] = 41;
    dpcoden["10"] = 90;
    dpcoden["52"] = 90;
    dpcoden["53"] = 90;
}















bool writebyte(char* &addr,char c,int &byte,int sizeoffile)
{
    if(byte >= sizeoffile-1)
    {
        return false;
    }
    *addr = c;
    cout<<c;
    addr++;
    byte++;
    return true;
}


int main(int argc,char *argv[])
{
    string ini_str = "Roll Number    Course Number    Couse Name\n\n" ;
    initialize();
    vector<string> x;
    string line;
    ifstream myfile ("./database/index.txt");
      if (myfile.is_open())
      {
            while ( getline (myfile,line) )
            {
                x = split(line, ',');
                cname[x[0]] = x[1];
            }
        myfile.close();
      }
  else
  {
    cout << "Unable to open file";
  }
    if(argc==1)
        listDir("database");
    else{
        for(argv++;*argv;argv++)
        {
            listDir(*argv);
        }
    }

    string course;


    int k;
    map<string,string> stud;
    for(map<string,vector<string> > ::iterator it = stu.begin() ; it!= stu.end() ; it++)
    {
            cout<<it->first<<" ";
            course = " ";
            for(vector<string>::iterator itt = (it->second).begin() ;itt!= (it->second).end() ; itt++)
            {
                course+=(*itt);
                course+=",";
                cout<<(*itt);       ///check
            }
            cout<<endl;
            course+="\n";
            stud[it->first] = course;
    }
//for(map<string,string > ::iterator it = stud.begin() ; it!= stud.end() ; it++)
//{
//    cout<<it->first<<it->second<<endl;
//}
cout<<"as";
listDir2("database");
for(int i = 0 ; i< paths.size(); i++) //paths.size()
{
    cout<<"Doing for file: "<<paths[i]<<endl;
    char path[100];
    int k = 0;
    for(; k<paths[i].size() ; k++)
    {
        path[k] = paths[i][k];
    }
    path[k] = '\0';
   // ifstream myfile (paths[i]);
    ifstream myfile (path);
    vector<string> rollinfile;
    string line;
    char roll[20];
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
              int i;
              for( i = 0 ; i< line.length() ;i++)
              {
                if(line[i]!=',')
                {
                    roll[i] = line[i];
                }
                else
                break;
              }
              roll[i] = '\0';
              if(strlen(roll)>6 && roll[0]!=' ' )                    ///CHECK
              {
                string ro(roll);
                rollinfile.push_back(ro);   //make array of roll numbers;
              }
        }

        myfile.close();
    }
    else
    {
        cout<<"file open error for rollinfile\n";
    }



//    for(int i = 0 ; i< rollinfile.size(); i++)
//    {
//        cout<<rollinfile[i]<<endl;
//    }

///mmap section

    char xyz;
    char *addr;
    int fd;
    struct stat sb;
    fd = open(path,O_RDWR);
    if(fd == -1)
    {
        printf("file open failed\n");
        return 0;
    }
    if(fstat(fd,&sb)==-1)
    {
        printf("fstat error\n");
        return 0;
    }
    addr = (char *)mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0  );
    close(fd);
    if(addr == MAP_FAILED)
    {
        printf("file map failed\n");
        return 0;
    }
    char *start = addr;
    int bytes_written = 0;
    bool notdone = true;

   // cin>>xyz;
    switch(fork())
    {
        case -1:
            cout<<"Fork failed!\n";
            return 0;
        case 0:
           // cout<<"Iamchild";
            for(int everyroll = 0 ; everyroll < rollinfile.size() ;everyroll++)
            {
                if(!notdone)
                    break;
                for(int eachchar = 0 ; eachchar<rollinfile[everyroll].size() ; eachchar++)
                {
                    if(!notdone)
                        break;
                    notdone = writebyte(addr,rollinfile[everyroll][eachchar],bytes_written,sb.st_size);  //(char*& addr,char c,int &byte,int sizeoffile)
                }
                    if(!notdone)
                    break;
                for(int eachchar = 0 ; eachchar<stud[rollinfile[everyroll]].size() ; eachchar++)
                {
                    if(!notdone)
                        break;
                    notdone = writebyte(addr,stud[rollinfile[everyroll]][eachchar],bytes_written,sb.st_size);  //(char*& addr,char c,int &byte,int sizeoffile)
                }
            }

             _exit(EXIT_SUCCESS);
        default:
           // cout<<"imparetn";
            wait(NULL); //wait for child to over
             //   exit(EXIT_SUCCESS);
            if (munmap(start, sb.st_size) == -1)
            {
                printf("file unmap fialed\n");
                return 0;
            }
    }
}


///mmap section


    return 0;
}
