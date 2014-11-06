#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>


pthread_t philosopher[5];
pthread_mutex_t forks[5];


 void *funmutex(int n);

 void *funmutex(int n) {

   printf("\nPhilosopher %d is thinking \n",n);
   pthread_mutex_lock(&forks[n]);
   pthread_mutex_lock(&forks[(n+1)%5]);
   printf("\nPhilosopher %d is eating \n",n);
   sleep(3);
   pthread_mutex_unlock(&forks[n]);
   pthread_mutex_unlock(&forks[(n+1)%5]);
   printf("\nPhilosopher %d has finished eating \n",n);
 }
 


int main()    {

  int i,k;
  void *msg;
  for(i=1;i<=5;i++)    {

    k=pthread_mutex_init(&forks[i],NULL);
    if(k==-1)     {

      printf("Mutex initialization failed\n");
      exit(1);
    }
  }
 
  for(i=1;i<=5;i++)   {

    k=pthread_create(&philosopher[i],NULL,(void *)funmutex,(int *)i);

    if(k!=0)    {

      printf("Thread creation error");
      exit(1);
    }
  }
  
  for(i=1;i<=5;i++)    {

    k=pthread_join(philosopher[i],&msg);

    if(k!=0)    {

    printf("Thread join failure");

    exit(1);
    }
  }

  for(i=1;i<=5;i++)    {

    k=pthread_mutex_destroy(&forks[i]);

    if(k!=0)    {

    printf("Mutex Destroyed\n");
    exit(1);
    }
  }

  return 0;
 }

