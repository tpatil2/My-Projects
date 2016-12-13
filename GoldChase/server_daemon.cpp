#ifndef server_daemon_CPP
#define server_daemon_CPP
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
#include "goldchase.h"
#include "read_write_mq.cpp"

GameBoard*  mptr;

int debugFD;
bool gotsighup=false;
int new_sockfd=0;
unsigned char* my_local_copy;
int mapsize;
unsigned char SockPlayer=G_SOCKPLR;
unsigned char* temp;

void SIGUSR1_server(int){

write(debugFD, "GOT SIG USER 1!\n", sizeof("GOT SIG USER1!\n"));

vector< pair<short,unsigned char> > pvec;
  for(short i=0; i<mapsize; ++i)
  {
    write(debugFD, "inside map size!\n", 20);
    if(temp[i]!= my_local_copy[i])
    {
      pair<short,unsigned char> aPair;
      aPair.first=i;
      aPair.second=temp[i];
      pvec.push_back(aPair);
      my_local_copy[i]=temp[i];
      write(debugFD, "changed square!\n", 20);
    }
  }

  //here iterate through pvec, writing out to socket
  //testing we will print it:
  unsigned char byt = 0;
  if(pvec.size()>0){
    write(debugFD, "vectore greater !\n", 15);

      write(new_sockfd,&byt,1);
      short szc=pvec.size();
      write(new_sockfd,&szc,sizeof(short));

      for(short i=0; i<pvec.size(); ++i)
      {
        write(debugFD, "forloop start!\n", 15);

        cerr << "offset=" << pvec[i].first;
        cerr << ", new value=" << pvec[i].second << endl;

        write(new_sockfd,&pvec[i].first,sizeof(short));
        write(new_sockfd,&pvec[i].second, sizeof(unsigned char));
        write(debugFD, "forloop end!\n", 15);

      }
  }

}


void SIGUSR2_server(int){

write(debugFD, "GOT SIG USER 2!\n", sizeof("GOT SIG USER1!\n"));
}



void SIGHUP_server(int){

  write(debugFD, "GOT SIG HUP!\n", sizeof("GOT SIG HUP!\n"));
  SockPlayer=G_SOCKPLR;
  for(int i=0; i<5; ++i)
      if(mptr->pid[i]!=0)
      switch(i)
      {
      case 0:
        SockPlayer|=G_PLR0;
        break;
      case 1:
        SockPlayer|=G_PLR1;
        break;
      case 2:
          SockPlayer|=G_PLR2;
          break;
      case 3:
            SockPlayer|=G_PLR3;
            break;
      case 4:
            SockPlayer|=G_PLR4;
            break;
      }

      WRITE(new_sockfd,&SockPlayer,sizeof(unsigned char));
}



void create_daemon()
{
    int s_rw = 0;
    int s_cl = 0;



  if(fork()>0){
    sleep(1);
    return; //I'm the parent, leave the function
  }

  if(fork()>0)
    exit(0);
  if(setsid()==-1)
    exit(1);

  for(int i=0; i< sysconf(_SC_OPEN_MAX); ++i)
    close(i);

  open("/dev/null", O_RDWR); //fd 0
  open("/dev/null", O_RDWR); //fd 1
  open("/dev/null", O_RDWR); //fd 2
  umask(0);
  chdir("/");

  debugFD=open("/home/csci-611/Desktop/project3/src/mypipe", O_RDWR);

  struct sigaction Siguer1;
  Siguer1.sa_handler=SIGUSR1_server;
  sigemptyset(&Siguer1.sa_mask);
  Siguer1.sa_flags=0;
  Siguer1.sa_restorer=NULL ;
  sigaction(SIGUSR1, &Siguer1, NULL);

  struct sigaction sigusr2;
  sigusr2.sa_handler=SIGUSR2_server;
  sigemptyset(&sigusr2.sa_mask);
  sigusr2.sa_flags=0;
  sigusr2.sa_restorer=NULL ;
  sigaction(SIGUSR2, &sigusr2, NULL);

  struct sigaction sighup;
  sighup.sa_handler=SIGHUP_server;
  sigemptyset(&sighup.sa_mask);
  sighup.sa_flags=0;
  sighup.sa_restorer=NULL ;
  sigaction(SIGHUP, &sighup, NULL);


  //open shared memory in daemon
  int shmFD;
  shmFD = shm_open("map", O_RDWR,  S_IRUSR | S_IWUSR);
  if(shmFD == -1){
    perror("shared memory open daemon if:");
  }else{
    perror("shared memory open daemon else:");
  }

  read(shmFD, &s_rw, sizeof(int));
  read(shmFD, &s_cl, sizeof(int));

  mptr=(GameBoard*) mmap (NULL,s_rw*s_cl+sizeof(GameBoard),PROT_READ|PROT_WRITE,MAP_SHARED,shmFD,0);

  mptr->daemonID=getpid();
  mapsize = s_rw*s_cl;
  temp=mptr->map;
  my_local_copy = new unsigned char [mapsize];
  //my_local_copy = temp; //COPY MAP TO LOCAL COPY

for(int i=0 ;i<mapsize;++i){
  my_local_copy[i] = temp[i];
}

  char c='*';
  char c2='1';
  char c3=' ';
  //copying map to local map
  for(short i=0;i<mapsize;i++){
      if(my_local_copy[i]& G_WALL){
        write(debugFD,&c,sizeof(char));
      }else if(my_local_copy[i]& G_PLR0){
        write(debugFD,&c2,sizeof(char));
      }else if(my_local_copy[i] == 0){
        write(debugFD,&c3,sizeof(char));
      }
    }

/*===========================server statrtup===========================*/

  int sockfd; //file descriptor for the socket
  int status; //for error checking

  write(debugFD,"server is going to start\n",sizeof("server is going to start\n"));

  //change this # between 2000-65k before using
  const char* portno="42431";
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints)); //zero out everything in structure
  hints.ai_family = AF_UNSPEC; //don't care. Either IPv4 or IPv6
  hints.ai_socktype=SOCK_STREAM; // TCP stream sockets
  hints.ai_flags=AI_PASSIVE; //file in the IP of the server for me

  write(debugFD,"server addrinfo\n",sizeof("server is addrinfo\n"));

  struct addrinfo *servinfo;

  if((status=getaddrinfo(NULL, portno, &hints, &servinfo))==-1){
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      write(debugFD, "ERROR getaddrinfo error:!\n", sizeof("ERROR getaddrinfo error:!\n"));
      exit(1);
    }

  write(debugFD,"server status is done\n",sizeof("server is status is done\n"));

  sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  /*avoid "Address already in use" error*/
  int yes=1;

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1){
      perror("setsockopt");
      write(debugFD, "ERROR setsockopt !\n", sizeof("ERROR setsockopt!\n"));
      exit(1);
    }

  write(debugFD,"server setsockopt Done\n",sizeof("server is setsockopt Done\n"));

    //We need to "bind" the socket to the port number so that the kernel
    //can match an incoming packet on a port to the proper process
  if((status=bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen))==-1){
        perror("bind");
        write(debugFD, "ERROR bind !\n", sizeof("ERROR bind!\n"));
        exit(1);
        }

    write(debugFD,"server status bind done \n",sizeof("server is status bind done \n"));

    //when done, release dynamically allocated memory
    freeaddrinfo(servinfo);

  if(listen(sockfd,1)==-1){
          perror("listen");
          write(debugFD,"server listen error\n",sizeof("server is lister error\n"));
          exit(1);
          }


  write(debugFD,"server Blocking, waiting for client to connect\n",sizeof("server Blocking, waiting for client to connect\n"));
  printf("Blocking, waiting for client to connect\n");

  struct sockaddr_in client_addr;
  socklen_t clientSize = sizeof(client_addr);
  //int new_sockfd;


dooer:  if((new_sockfd=accept(sockfd, (struct sockaddr*) &client_addr, &clientSize))==-1){
    perror("accept");
    write(debugFD, "ERROR accept !\n", sizeof("ERROR accept!\n"));
    goto dooer;
  //  exit(1);
    }//blocking

    //read & write to the socket

    WRITE(new_sockfd,&s_rw,sizeof(int));
    WRITE(new_sockfd,&s_cl,sizeof(int));
    //std::cerr << "rows are " << s_rw<< std::endl;

    //Send the local copy to client
    for(int j=0;j<mapsize;j++)
    {
      WRITE(new_sockfd,&my_local_copy[j],sizeof(unsigned char));
    }

		//write(new_sockfd, &SockPlayer, 1);
      WRITE(new_sockfd,&SockPlayer,sizeof(unsigned char));

  while(1){

  write(debugFD, "inside while !\n", 10);
  sem_t *my_sem_ptr;
  unsigned char byte;
  write(debugFD, "befor read !\n", 10);
  READ(new_sockfd, &byte, 1);
  write(debugFD, "after read !\n", 10);

  if(byte & G_SOCKPLR){
  unsigned char player_bit[5]={G_PLR0, G_PLR1, G_PLR2, G_PLR3, G_PLR4};
  write(debugFD, "SOCKPLR1 !\n", 10);
  for(int i=0; i<5; ++i) //loop through the player bits
  {  write(debugFD, "SOCKPLR2 !\n", 10);

  // If player bit is on and shared memory ID is zero,
  // a player (from other computer) has joined:
  if(byte & player_bit[i] && mptr->pid[i]==0){
    //do 1.1 and 1.2
    mptr->pid[i]=mptr->daemonID;
    write(debugFD, "SOCKPLR3 !\n", 10);
    //mq_open 0 1 2 3 4 5
    if(i==0){mq_name1="/tpatil1";}
    if(i==1){mq_name1="/tpatil2";}
    if(i==2){mq_name1="/tpatil3";}
    if(i==3){mq_name1="/tpatil4";}
    if(i==4){mq_name1="/tpatil5";}
    read_Dqueue(i);
    //

  //If player bit is off and shared memory ID is not zero,
  //remote player has quit:
  }
  else if(!(byte & player_bit[i]) && mptr->pid[i]!=0)
  {  //do 2.1 and 2.2
    mptr->pid[i]=0;
    if(i==0){mq_name1="/tpatil1";}
    if(i==1){mq_name1="/tpatil2";}
    if(i==2){mq_name1="/tpatil3";}
    if(i==3){mq_name1="/tpatil4";}
    if(i==4){mq_name1="/tpatil5";}
    mq_close(Dreadqueue_fd[i]);
    mq_unlink(mq_name1.c_str());
    //mq_close  mq_unlink
    write(debugFD, "SOCKPLR1 NULL !\n", 13);
  }
}
  if(byte==G_SOCKPLR){
    shm_unlink("map");
    sem_close(my_sem_ptr);
    sem_unlink("/player1");
    write(debugFD, "SOCKPLR1 Cleaning !\n", 15);
    unsigned char bye=G_SOCKPLR;
    WRITE(new_sockfd,&bye,sizeof(unsigned char));
    exit(0);

  }
  //no players are left in the game.  Close and unlink the shared memory.
  //Close and unlink the semaphore.  Then exit the program.
}

if(byte==0){
		write(debugFD, "map chenge !\n",20);

		short v_size;
		short offset;
		unsigned char new_byte;

		READ(new_sockfd, &v_size, sizeof(short));
	   write(debugFD, "map chenge after read !\n",25);
			for(short i=0; i<v_size; ++i)
      {
				write(debugFD, "map chenge inside for!\n",25);
        READ(new_sockfd,&offset,sizeof(short));
        READ(new_sockfd,&new_byte, sizeof(unsigned char));
				mptr->map[offset]=new_byte;
				my_local_copy[offset]=new_byte;

      }
				write(debugFD, "map chenge drawMap !\n",25);
				for(int m=0;m<5;m++){
          if(mptr->pid[m]!=0){
            kill(mptr->pid[m],SIGUSR1);
          }}


	}


/*if(gotsighup){
  WRITE(new_sockfd,&SockPlayer,sizeof(unsigned char));
  gotsighup=false;
}*/

}
  close(new_sockfd);


/*=======================server end=========================*/

  while(1)
  {
  //std::cerr << "cerr Daemon is running!" << std::endl;
  write(debugFD, "Daemon is running!\n", sizeof("Daemon is running!\n"));
  std::cerr << "Daemon ID at  :"<<getpid()<< std::endl;
  sleep(2);
  }


  //Now, write the signal handlers. This is most of the code, right here.
}

#endif
