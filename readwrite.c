#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t rcountacc;
sem_t dbcountacc;
int rcount=0;

void *Reader(void *arg);
void *Writer(void *arg);

void *Writer(void *arg)
{
 
 sleep(1); 
 int temp=(int)arg;
 printf("\nWriter %d is trying to enter into database for modifying the data",temp);
 sem_wait(&dbcountacc);
 printf("\nWriter %d is writting into the database",temp); 
 printf("\nWriter %d is leaving the database\n",temp); 
 sem_post(&dbcountacc);
}

void *Reader(void *arg)
{ 
 sleep(1); 
 int temp=(int)arg;
 printf("\nReader %d is trying to enter into the Database for reading the data",temp);
 sem_wait(&rcountacc);
 rcount++;
 if(rcount==1)
 {
  sem_wait(&dbcountacc);
  printf("\nReader %d is reading the database",temp); 
 }
 sem_post(&rcountacc);
 sem_wait(&rcountacc);
 rcount--;
 if(rcount==0)
 {
  printf("\nReader %d is leaving the database\n",temp);  
  sem_post(&dbcountacc);
 }
 sem_post(&rcountacc);
}
int main(int argc, char *argv[])
{
 int i=0;
 sem_init(&rcountacc,0,1);
 sem_init(&dbcountacc,0,1);
 
 pthread_t Readers_thr[100],Writer_thr[100];

int NoRT=atoi(argv[1]);
int NoWT=atoi(argv[2]);

 for(i=0;i<NoRT;i++)
 {
  pthread_create(&Readers_thr[i],NULL,Reader,(void *)i);
 }
 for(i=0;i<NoWT;i++)
 { 
  pthread_create(&Writer_thr[i],NULL,Writer,(void *)i);
 }
 for(i=0;i<NoWT;i++)
 {
  pthread_join(Writer_thr[i],NULL);
 }

 for(i=0;i<NoRT;i++)
 {
  pthread_join(Readers_thr[i],NULL); 
 }
 sem_destroy(&dbcountacc);
 sem_destroy(&rcountacc); 
 return 0;
}
