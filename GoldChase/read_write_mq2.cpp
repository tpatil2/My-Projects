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
//#include "read_write_mq2.h"


void read_Dmessage2(int pnum);
int write_Dqueue2(string st);
int read_Dqueue2(int pnum);
mqd_t Dreadqueue_fd2[5];
string mq_name2="/none";



void read_Dmessage2(int pnum)
{

  //set up message queue to receive signal whenever
  //message comes in. This is being done inside of
  //the handler function because it has to be "reset"
  //each time it is used.
  struct sigevent mq_notification_event;
  mq_notification_event.sigev_notify=SIGEV_SIGNAL;
  mq_notification_event.sigev_signo=SIGUSR2;
  mq_notify(Dreadqueue_fd2[pnum], &mq_notification_event);

  //read a message
  int err;
  char msg[121];
  memset(msg, 0, 121);//set all characters to '\0'
  while((err=mq_receive(Dreadqueue_fd2[pnum], msg, 120, NULL))!=-1)
  {
  //  cout << "Message received: " << msg << endl;
  //  ptr->postNotice(msg);
  //  memset(msg, 0, 121);//set all characters to '\0'
  }
  //we exit while-loop when mq_receive returns -1
  //if errno==EAGAIN that is normal: there is no message waiting
  if(errno!=EAGAIN)
  {
    perror("mq_receive");
    exit(1);
  }
}



int write_Dqueue2(string st)
{
const char *str = st.c_str();

  if(mq_name2=="/none")
  {
    cerr << "Please change mq_name2 from \"" << mq_name2 << "\" to something unique to you" << endl;
    exit(1);
  }

  mqd_t writequeue_fd; //message queue file descriptor

  //cout<<"mqq in write is--"<<mq_name2<<endl;
  if((writequeue_fd=mq_open(mq_name2.c_str(), O_WRONLY|O_NONBLOCK))==-1)
  {
    perror("mq_open");
    exit(1);
  }
//  cerr << "fd=" << writequeue_fd << endl;
  char message_text[121];
  memset(message_text, 0, 121);
  strncpy(message_text, str, 120);
  if(mq_send(writequeue_fd, message_text, strlen(message_text), 0)==-1)
  {
    perror("mq_send");
    exit(1);
  }
  mq_close(writequeue_fd);
}


int read_Dqueue2(int pnum)
{
  //I have added this signal-handling
  //code so that if you type ctrl-c to
  //abort the long, slow loop at the
  //end of main, then your message queue
  //will be properly cleaned up.
/*  struct sigaction exit_handler;
  exit_handler.sa_handler=clean_up;
  sigemptyset(&exit_handler.sa_mask);
  exit_handler.sa_flags=0;
  sigaction(SIGINT, &exit_handler, NULL);
  sigaction(SIGTERM, &exit_handler, NULL);
  sigaction(SIGHUP, &exit_handler, NULL);

*/


  //make sure we can handle the SIGUSR2
  //message when the message queue
  //notification sends the signal
  //struct sigaction action_to_take;
  //handle with this function
//  action_to_take.sa_handler=read_Dmessage;
  //zero out the mask (allow any signal to interrupt)
  //sigemptyset(&action_to_take.sa_mask);
  //action_to_take.sa_flags=0;
  //tell how to handle SIGINT
  //sigaction(SIGUSR2, &action_to_take, NULL);

  struct mq_attr mq_attributes;
  mq_attributes.mq_flags=0;
  mq_attributes.mq_maxmsg=10;
  mq_attributes.mq_msgsize=120;

  if((Dreadqueue_fd2[pnum]=mq_open(mq_name2.c_str(), O_RDONLY|O_CREAT|O_EXCL|O_NONBLOCK,
          S_IRUSR|S_IWUSR, &mq_attributes))==-1)
  {
    perror("mq_open in read");
    exit(1);
  }
  //set up message queue to receive signal whenever message comes in
  struct sigevent mq_notification_event;
  mq_notification_event.sigev_notify=SIGEV_SIGNAL;
  mq_notification_event.sigev_signo=SIGUSR2;
  mq_notify(Dreadqueue_fd2[pnum], &mq_notification_event);
}
