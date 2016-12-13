#ifndef client_daemon_CPP_
#define client_daemon_CPP_
#include<unistd.h>
#include<stdlib.h>

#include<sys/stat.h>
#include<cstring>
#include<vector>
#include<utility>
#include <sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h> //for read/write
#include<string.h> //for memset
#include<stdio.h> //for fprintf, stderr, etc.
#include<stdlib.h> //for exit
#include <fcntl.h>
#include<iostream>
#include<sstream>      /* For O_* constants */
#include<string.h>
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "goldchase.h"
#include "Map.h"
#include <mqueue.h>
#include<cstring>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <signal.h>

#include "fancyRW.h"
#include "gameboard.h"
#include "read_write_mq2.cpp"


GameBoard*  mptr1;
unsigned char SockPlayer1=G_SOCKPLR;
unsigned char* temp1;

using namespace std;

int debugfd;

bool gotsighupclnt=false;
unsigned char* client_local_copy;
int area;
int sockfd;


void SIGUSR1_client(int){

	write(debugfd, "GOT SIG USER 1!\n", sizeof("GOT SIG USER1!\n"));

	vector< pair<short,unsigned char> > pvec;
	  for(short i=0; i<area; ++i)
	  {
	    write(debugfd, "inside map size!\n", 20);
	    if(temp1[i]!= client_local_copy[i])
	    {
	      pair<short,unsigned char> aPair;
	      aPair.first=i;
	      aPair.second=temp1[i];
	      pvec.push_back(aPair);
	      client_local_copy[i]=temp1[i];
	      write(debugfd, "changed square!\n", 20);
	    }
	  }

	  //here iterate through pvec, writing out to socket
	  //testing we will print it:
	  unsigned char byt = 0;
	  if(pvec.size()>0){
	    write(debugfd, "vectore greater !\n", 15);

	      WRITE(sockfd,&byt,1);
	      short szc=pvec.size();
	      WRITE(sockfd,&szc,sizeof(short));

	      for(short i=0; i<pvec.size(); ++i)
	      {
	        write(debugfd, "forloop start!\n", 15);

	        cerr << "offset=" << pvec[i].first;
	        cerr << ", new value=" << pvec[i].second << endl;

	        WRITE(sockfd,&pvec[i].first,sizeof(short));
	        WRITE(sockfd,&pvec[i].second, sizeof(unsigned char));
	        write(debugfd, "forloop end!\n", 15);

	      }
	  }

	}

void SIGUSR2_client(int){
	write(debugfd, "GOT SIG USER 2!\n", sizeof("GOT SIG USER1!\n"));
	}

void SIGHUP_client(int){

	write(debugfd, "GOT SIG HUP!\n", sizeof("GOT SIG USER1!\n"));
	SockPlayer1=G_SOCKPLR;
	for(int i=0; i<5; ++i)
	      if(mptr1->pid[i]!=0)
	      switch(i)
	      {
	      case 0:
	        SockPlayer1|=G_PLR0;
	        break;
	      case 1:
	        SockPlayer1|=G_PLR1;
	        break;
	      case 2:
	        SockPlayer1|=G_PLR2;
	        break;
	      case 3:
	        SockPlayer1|=G_PLR3;
	        break;
	      case 4:
	        SockPlayer1|=G_PLR4;
	        break;
	      }

				gotsighupclnt=true;

				write(sockfd, &SockPlayer1, 1);
	}


void client_daemon(char* addr)
{
	sem_t *my_sem_clnt;
	int pipefd[2];
	pipe(pipefd);
	int c_rws=0;
	int c_cls=0;


	if(fork()>0)//I'm the parent, leave the function
	{
	/*	close(pipefd[1]); //close write, parent only needs read
		int val=1;
		while(1){
			write(debugfd, "paernt while !\n", sizeof("parent while !\n"));
			read(pipefd[0], &val, sizeof(val));
			if(val==0){
			return;
			}
		}
		*/
    sleep(5);
    return;
	}

	if(fork()>0)
		exit(0);
	if(setsid()==-1)
		exit(1);

	for(int i=0; i< sysconf(_SC_OPEN_MAX); ++i) close(i);

	open("/dev/null", O_RDWR); //fd 0
	open("/dev/null", O_RDWR); //fd 1
	open("/dev/null", O_RDWR); //fd 2
	umask(0);
	chdir("/");

	debugfd=open("/home/tejas/Desktop/project3/src/mypipe", O_RDWR);

		struct sigaction Siguer1;
	  Siguer1.sa_handler=SIGUSR1_client;
	  sigemptyset(&Siguer1.sa_mask);
	  Siguer1.sa_flags=0;
	  Siguer1.sa_restorer=NULL ;
	  sigaction(SIGUSR1, &Siguer1, NULL);

	  struct sigaction sigusr2;
	  sigusr2.sa_handler=SIGUSR2_client;
	  sigemptyset(&sigusr2.sa_mask);
	  sigusr2.sa_flags=0;
	  sigusr2.sa_restorer=NULL ;
	  sigaction(SIGUSR2, &sigusr2, NULL);

	  struct sigaction sighup;
	  sighup.sa_handler=SIGHUP_client;
	  sigemptyset(&sighup.sa_mask);
	  sighup.sa_flags=0;
	  sighup.sa_restorer=NULL ;
	  sigaction(SIGHUP, &sighup, NULL);

	//===================Client Startup==============================
//	int sockfd; //file descriptor for the socket
	int status; //for error checking

	write(debugfd, "Client is going to start: !\n", sizeof("Client is going to start:!\n"));

	//change this # between 2000-65k before using
	const char* portno="42431";

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints)); //zero out everything in structure
	hints.ai_family = AF_UNSPEC; //don't care. Either IPv4 or IPv6
	hints.ai_socktype=SOCK_STREAM; // TCP stream sockets

	write(debugfd, "client addrinfo Done !\n", sizeof("Client addrinfo Done !\n"));

	struct addrinfo *servinfo;
	//instead of "localhost", it could by any domain name
	if((status=getaddrinfo(addr, portno, &hints, &servinfo))==-1){
			fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
			write(debugfd, "ERROR getaddrinfo error: !\n", sizeof("ERROR getaddrinfo error:!\n"));
	  	exit(1);
		}

	write(debugfd, "Client getaddrinfo Done!\n", sizeof("Client getaddrinfo Done !\n"));

	sockfd=socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	write(debugfd, "Client next step !\n", sizeof("Client next step !\n"));

	if((status=connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen))==-1)
	{
		perror("connect");
		write(debugfd, "ERROR connect: !\n", sizeof("ERROR connect:!\n"));
	//	exit(1);
	}

	write(debugfd, "Client after connect Done !\n", sizeof("Client after connect Done !\n"));

	//release the information allocated by getaddrinfo()
	freeaddrinfo(servinfo);
	write(debugfd, "Client freeaddrinfo Done\n", sizeof("Client freeaddrinfo Done\n"));

	int err=READ(sockfd,&c_rws,sizeof(int));
	READ(sockfd,&c_cls,sizeof(int));
	if(err==-1){
		write(debugfd, "ERROR read: !\n", sizeof("ERROR read:!\n"));
	}
	write(debugfd,&c_rws, sizeof(c_rws));





	write(debugfd, "Now semaphore will be created: !\n", sizeof("Now semaphore will be created:!\n"));

	my_sem_clnt = sem_open(
			"/player1",
			O_CREAT|O_EXCL,
			S_IROTH| S_IWOTH| S_IRGRP| S_IWGRP| S_IRUSR| S_IWUSR,
			1
			);

	if(my_sem_clnt==SEM_FAILED)
	{
		if(errno==EEXIST)
		{perror("semphone present inside first palyer ");}
	}

	write(debugfd, "semaphore created in client daemon !\n", sizeof("semaphore created in client daemon!\n"));


	int ret=shm_open("map", O_CREAT|O_RDWR,  S_IRUSR | S_IWUSR);

	if (ret==-1){perror("the shared memory open client daemon problem:");}
	else{perror("Shared memory created if 1 client daemon player:");}


	if(ftruncate(ret,c_rws*c_cls)==-1)
	{perror("ftruncate called:");
		exit(1);
	}

	mptr1=(GameBoard*) mmap (NULL,c_rws*c_cls+sizeof(GameBoard),PROT_READ|PROT_WRITE,MAP_SHARED,ret,0);

	mptr1->daemonID=getpid();
 	area=c_rws*c_cls;

	client_local_copy = new unsigned char [area];


	mptr1->row_s=c_rws;
	mptr1->col_s=c_cls;
	unsigned char copy;

	for(int i=0;i<area;i++)
	{
		READ(sockfd,&copy,sizeof(unsigned char));
		mptr1->map[i]=copy;
		client_local_copy[i]=copy;
	}

 temp1=mptr1->map;

	while(1){

		write(debugfd, "indside while\n",20);
	  sem_t *my_sem_ptr;
	  unsigned char byte;
		write(debugfd, "befor read !\n",20);
  	READ(sockfd, &byte, 1);
		write(debugfd, "after read !\n",sizeof("after read !\n"));

	  if(byte & G_SOCKPLR){
			write(debugfd, "SOCKPLR 1 !\n",20);

	  unsigned char player_bit[5]={G_PLR0, G_PLR1, G_PLR2, G_PLR3, G_PLR4};

	  for(int i=0; i<5; ++i) //loop through the player bits
	  {
			write(debugfd, "SOCKPLR 2 !\n",20);

	  // If player bit is on and shared memory ID is zero,
	  // a player (from other computer) has joined:
	  if(byte & player_bit[i] && mptr1->pid[i]==0){
	    //do 1.1 and 1.2
			write(debugfd, "SOCKPLR 3 !\n",20);
	    mptr1->pid[i]=mptr1->daemonID;
			if(i==0){mq_name2="/tpatil1";}
	    if(i==1){mq_name2="/tpatil2";}
	    if(i==2){mq_name2="/tpatil3";}
	    if(i==3){mq_name2="/tpatil4";}
	    if(i==4){mq_name2="/tpatil5";}
	    read_Dqueue2(i);
	  //If player bit is off and shared memory ID is not zero,
	  //remote player has quit:
	  }
	  else if(!(byte & player_bit[i]) && mptr1->pid[i]!=0)
	  {  //do 2.1 and 2.2
			write(debugfd, "SOCKPLR NULL !\n",20);
	    mptr1->pid[i]=0;
			if(i==0){mq_name2="/tpatil1";}
	    if(i==1){mq_name2="/tpatil2";}
	    if(i==2){mq_name2="/tpatil3";}
	    if(i==3){mq_name2="/tpatil4";}
	    if(i==4){mq_name2="/tpatil5";}
	    mq_close(Dreadqueue_fd2[i]);
	    mq_unlink(mq_name2.c_str());
	  }
	}
	  if(byte==G_SOCKPLR){
	    shm_unlink("map");
	    sem_close(my_sem_ptr);
	    sem_unlink("/player1");
			write(debugfd, "SOCKPLR cleaning !\n",20);
			unsigned char bye=G_SOCKPLR;
    	WRITE(sockfd,&bye,sizeof(unsigned char));
    	exit(0);

	  }
	  //no players are left in the game.  Close and unlink the shared memory.
	  //Close and unlink the semaphore.  Then exit the program.
	}
	if(byte==0){
		write(debugfd, "map chenge !\n",20);

		short v_size;
		short offset;
		unsigned char new_byte;

		READ(sockfd, &v_size, sizeof(short));
	write(debugfd, "map chenge after read !\n",25);
			for(short i=0; i<v_size; ++i)
      {
				write(debugfd, "map chenge inside for!\n",25);
        READ(sockfd,&offset,sizeof(short));
        READ(sockfd,&new_byte, sizeof(unsigned char));
				mptr1->map[offset]=new_byte;
				client_local_copy[offset]=new_byte;

      }
				write(debugfd, "map chenge drawMap !\n",25);
				for(int m=0;m<5;m++){
          if(mptr1->pid[m]!=0){
            kill(mptr1->pid[m],SIGUSR1);
          }}


	}

	/*if(gotsighup){
	  WRITE(new_sockfd,&SockPlayer1,sizeof(unsigned char));
	  gotsighup=false;
	}*/

	}


write(debugfd, " after read SOCKPLR: !\n", sizeof(" before read SOCKPLR: !\n"));

	//int val=0;
	//write(pipefd[1], &val, sizeof(val));

	close(sockfd);


	//============================Client end====================>

	//Now, write the signal handlers. This is most of the code, right here.
}


#endif
