nclude <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <grp.h>
#define RESET_COLOR "\e[m" 
#define MAKE_GREEN "\e[32m" 
#define MAKE_BLUE "\e[36m"

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"
#define LINELEN 256
 void copy_file(FILE *fin, FILE* fout, int writeLineNumbers);



void filedate(struct stat filestat)
{
  char dm[100];
  struct tm *dt;
  dt=localtime(&filestat.st_mtime);
  strftime(dm,100,"%b %d %H:%M",dt);
  printf("%s\t",dm);
}
void nocolor(struct dirent *selfile)
{
    printf("%s", (char*)selfile->d_name);
}
void clrpr(struct dirent *selfile,struct stat filestat)
{
  (S_ISDIR(filestat.st_mode)? printf(MAKE_BLUE"%s   "RESET_COLOR,(char*)selfile->d_name) : (filestat.st_mode & S_IXOTH )? printf(MAKE_GREEN"%s   "RESET_COLOR,(char*)selfile->d_name): printf("%s   ",(char*)selfile->d_name) );
}

void filesize(struct stat filestat)
{
  printf("%d\t",(int)filestat.st_size);
}

void fileusrgrp(struct stat filestat)
{
  struct passwd *pwd;
  struct group *grp;
  pwd=getpwuid(filestat.st_uid);
  grp=getgrgid(filestat.st_gid);
  printf("%s\t",pwd->pw_name);
  printf("%s\t  ",grp->gr_name);
  
}
void filelink(struct stat filestat)
{
  printf(" %d\t",(int)filestat.st_nlink);  
}

void getpermission(struct stat filestat)
{
    printf( (S_ISDIR(filestat.st_mode)) ? "d" : "-");
    printf( (filestat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (filestat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (filestat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (filestat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (filestat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (filestat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (filestat.st_mode & S_IROTH) ? "r" : "-");
    printf( (filestat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (filestat.st_mode & S_IXOTH) ? "x" : "-");
  
}

int lsfunction(int argcount,char* arg[MAX_LENGTH]) {

  DIR *mydir;
  struct dirent *selfile;
  struct stat filestat;
  char* dir=(char*)get_current_dir_name();
  int f=0;
  char* name;
  argcount--;
  printf("%s",arg[0]);
  if(argcount<=2)
  { 
    if(!strcmp(arg[1],"-l"))
      f=1;
    else if(!strcmp(arg[1],"-a"))
      f=2;
    else if(!strcmp(arg[1],"-la"))
      f=3;
    else if(!strcmp(arg[1],"-d"))
      f=4;
    else if(!strcmp(arg[1],"-ld"))
      f=5;
    
  }  
  printf("%d\n",argcount);
  mydir = opendir(dir);
  while((selfile = readdir(mydir)) != NULL)
  {
    stat(selfile->d_name, &filestat);
    name=(char*)selfile->d_name;
    if(f==1)
    {
      if(*name!='.')
      {
      
        getpermission(filestat);
        filelink(filestat);
        fileusrgrp(filestat);
        filesize(filestat);
        filedate(filestat);
        clrpr(selfile,filestat); 
        printf("\n");
 
      }
    }
    else if(f==3)
    {
     
      getpermission(filestat);
      filelink(filestat);
      fileusrgrp(filestat);
      filesize(filestat);
      filedate(filestat);
      clrpr(selfile,filestat);
      printf("\n");

    }
    else if(f==2)
    {
     
      clrpr(selfile,filestat);
    }
    else if(f==4)
    {
      if(!strcmp(selfile->d_name,"."))
        clrpr(selfile,filestat);
    }
    else if(f==5)
    {
      if(!strcmp(selfile->d_name,"."))
      {
       
        getpermission(filestat);
        filelink(filestat);
        fileusrgrp(filestat);
        filesize(filestat);
        filedate(filestat);
        clrpr(selfile,filestat); 
        printf("\n");
      }
    }
    else
    {
      if(*name!='.')
      {
      
        clrpr(selfile,filestat);
      }
    }  
  }
  printf("\n");
  closedir(mydir);

}

/*code for ps*/

int check_if_number (char *str)
{
  int i;
  for (i=0; str[i] != '\0'; i++)
  {
    if (!isdigit (str[i]))
    {
      return 0;
    }
  }
  return 1;
}
 
#define MAX_BUF 1024
#define INT_SIZE_BUF 6
#define PID_LIST_BLOCK 32
#define UP_TIME_SIZE 10
const char *getUserName(int uid)
{
  struct passwd *pw = getpwuid(uid);
  if (pw)
  {
    return pw->pw_name;
  }

  return "";
}
 
void pidaux ()
{
  DIR *dirp;
  FILE *fp;
  struct dirent *entry;
  char path[MAX_BUF], read_buf[MAX_BUF],temp_buf[MAX_BUF];
  char uid_int_str[INT_SIZE_BUF]={0},*line;
  char uptime_str[UP_TIME_SIZE];
  char *user,*command;
  size_t len=0;
  dirp = opendir ("/proc/");
  if (dirp == NULL)
  {
    perror ("Fail");
    exit(0);
  } 
  strcpy(path,"/proc/");
  strcat(path,"uptime");
 
  fp=fopen(path,"r");
  if(fp!=NULL)
  {
	getline(&line,&len,fp);
	sscanf(line,"%s ",uptime_str);
  }
  long uptime=atof(uptime_str);
  long Hertz=sysconf(_SC_CLK_TCK); 
  strcpy(path,"/proc/");
  strcat(path,"meminfo");

  fp=fopen(path,"r");
  unsigned long long total_memory;
  if(fp!=NULL)
  {
	getline(&line,&len,fp);
	sscanf(line,"MemTotal:        %llu kB",&total_memory);
  }	
  
  while ((entry = readdir (dirp)) != NULL)
  {
    if (check_if_number (entry->d_name))
    {
	strcpy(path,"/proc/");
	strcat(path,entry->d_name);
	strcat(path,"/status");
	unsigned long long memory_rss;
	fp=fopen(path,"r");
	unsigned long long vmsize;
	if(fp!=NULL)
	{
		vmsize=0;
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		sscanf(line,"Uid:    %s ",uid_int_str);
		getline(&line,&len,fp);
                getline(&line,&len,fp);
                getline(&line,&len,fp);
		getline(&line,&len,fp);
                getline(&line,&len,fp);
		sscanf(line,"VmSize:    %llu kB",&vmsize);
                getline(&line,&len,fp);
                getline(&line,&len,fp);
                getline(&line,&len,fp);
		getline(&line,&len,fp);
		sscanf(line,"VmRSS:     %llu kB",&memory_rss);
		
	}
	else
	{
		fprintf(stdout,"FP is NULL\n");
	}
	float memory_usage=100*memory_rss/total_memory;
	strcpy(path,"/proc/");
	strcat(path,entry->d_name);
	strcat(path,"/stat");
	fp=fopen(path,"r");
	getline(&line,&len,fp);
	char comm[10],state;
	unsigned int flags;
	int pid,ppid,pgrp,session,tty_nr,tpgid;
	unsigned long minflt,cminflt,majflt,cmajflt,utime,stime;
	unsigned long long starttime;
	long cutime,cstime,priority,nice,num_threads,itreavalue;
	sscanf(line,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld  %ld %llu",&pid,comm,&state,&ppid,&pgrp,&session,&tty_nr,&tpgid,&flags,&minflt,&cminflt,&majflt,&cmajflt,&utime,&stime,&cutime,&cstime,&priority,&nice,&num_threads,&itreavalue,&starttime);
	unsigned long total_time=utime+stime;
	total_time=total_time+(unsigned long)cutime+(unsigned long)cstime;
	float seconds=uptime-(starttime/Hertz);
	float cpu_usage=100*((total_time/Hertz)/seconds);
	if(isnan(cpu_usage))
	{
		cpu_usage=0.0;
	}
	if(isnan(memory_usage))
	{
		memory_usage=0.0;
	}
	strcpy (path, "/proc/");
	strcat (path, entry->d_name);
	strcat (path, "/comm");
	 
      	
      	fp = fopen (path, "r");
      	if (fp != NULL)
      	{
        	fscanf (fp, "%s", read_buf);
		fclose(fp);
      	}
      	char *userName=getUserName(atoi(uid_int_str));
     	if(strlen(userName)<9)
      	{
		user=userName;	
      	}
      	else
      	{
		user=uid_int_str;
      	}
      	fprintf(stdout,"%s %s %0.1f %0.1f %llu %llu %c %s\n",user,entry->d_name,cpu_usage,memory_usage,vmsize,memory_rss,state,read_buf);
     	 
    }
  } 
  closedir (dirp);
}
 

/*-------------------*/
/*code for cat*/
int catcommand(int cnt,char *args[MAX_LENGTH]) {

     char cpr[]= "-n";
    FILE *fp;FILE *dest="rrr.txt";
//printf("%d%s%s%s",cnt,args[0],args[1],args[2]);
    if(cnt >1)
    if(strcmp(cpr,args[0])!=0)
    {
          if(((fp= fopen(args[0], "r"))!=NULL)) {
             copy_file(fp, stdout, 0);
             fclose(fp);
             exit(1);
          }
	  
/*	 
	   if(((fp= fopen(args[0], "r"))!=NULL) && strcmp(args[1],">")==0) {
		dest=args[1];
           copy_file(fp, dest, 1);
           fclose(fp);
           exit(1);
          
	 }*/
          else
          {
              perror("could not open the file.");
              exit(1);
          }
    }
    if(cnt==1)
    {
        copy_file(stdin, stdout, 0);
        exit(1);
    }
    else
    {
        if(cnt==2)
        {

               copy_file(stdin, stdout, 1);
               exit(1);
        }
        else
        {
             if((fp= fopen(args[1], "r"))!=NULL)  {
                 copy_file(fp, stdout, 1);
                 fclose(fp);
                 exit(1);
             }
             else
             {
                perror("could not open the file.");
                exit(1);
             }
        }

    }

   return 0;
}

void copy_file(FILE *fin, FILE* fout, int writeLineNumbers)
{
    char line[LINELEN];
    int lineno = 1;

    while (fgets(line, LINELEN, fin))
    {
        if (writeLineNumbers)
            printf("%d ", lineno);

        if (fputs(line, fout) == EOF)
        {
            perror("Write to stdout failed.");
            return;
        }

        ++lineno;
    }
}
               

/*...................*/

void clearScreen()
{
  const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO,CLEAR_SCREE_ANSI,12);
}
int main(int argc, char *argv[]) {
  char *cmd;
  char line[MAX_LENGTH];char *sec;char *param[MAX_LENGTH];char *tri;
int ip=0;int count,k=0,cl=0;
  while (1) {
    printf("Rohit:~$ ");
    if (!fgets(line, MAX_LENGTH, stdin)) break;

   
    if ((cmd = strtok(line, DELIMS))) {
     
      errno = 0;

	if (strcmp(cmd, "cd") == 0) {
        char *arg = strtok(0, DELIMS);

        if (!arg) fprintf(stderr, "cd missing argument.\n");
        else chdir(arg);

      } else if (strcmp(cmd, "exit") == 0) {
        break;

      } else 

	if(strcmp(cmd,"ls") == 0){
 
	       param[0]="ls";
	  if((sec=strtok(0, DELIMS))){
         
         param[1]=sec;}else{param[1]="0";}
		size_t len=strlen(param);
        k=(int)len;
        lsfunction(k,param);

         }
	if(strcmp(cmd,"ps")==0){
		
	pidaux();	
	}
	if(strcmp(cmd,"cat")==0){
			if((sec=strtok(0, DELIMS))){
			
			param[0]=sec;
		}
		if((tri=strtok(NULL, DELIMS))){
			param[1]=tri;
		}
	size_t ln=strlen(param);cl=(int)ln;
	cl=cl/2;
	catcommand(cl,param);
	}
	ip=strcmp(line,"clear");

	if(ip==0){clearScreen();

	}

      if (errno) perror("Command failed");
    }
  }

  return 0;exit(0);
}
