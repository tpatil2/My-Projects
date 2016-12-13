// NAME: Tejas Patil.  006949762

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
#include "gameboard.h"
GameBoard*  mp;
const int CAPACITY=10000;

using namespace std;

mqd_t readqueue_fd; //message queue file descriptor
string mq_name="/none";
Map *ptr = NULL;
//Map *ptr = new Map;
sem_t *my_sem_ptr;
bool clean =false;

void sig_handler(int){
if(ptr!=NULL){
  //  cerr<<"got interrupt \n";
    ptr->drawMap();
  }
}


void read_message(int)
{

  //set up message queue to receive signal whenever
  //message comes in. This is being done inside of
  //the handler function because it has to be "reset"
  //each time it is used.
  struct sigevent mq_notification_event;
  mq_notification_event.sigev_notify=SIGEV_SIGNAL;
  mq_notification_event.sigev_signo=SIGUSR2;
  mq_notify(readqueue_fd, &mq_notification_event);

  //read a message
  int err;
  char msg[121];
  memset(msg, 0, 121);//set all characters to '\0'
  while((err=mq_receive(readqueue_fd, msg, 120, NULL))!=-1)
  {
  //  cout << "Message received: " << msg << endl;
    ptr->postNotice(msg);
    memset(msg, 0, 121);//set all characters to '\0'
  }
  //we exit while-loop when mq_receive returns -1
  //if errno==EAGAIN that is normal: there is no message waiting
  if(errno!=EAGAIN)
  {
    perror("mq_receive");
    exit(1);
  }
}


void clean_up(int)
{
  //cerr << "Cleaning up message queue" << endl;
/*
  mq_close(readqueue_fd);
  mq_unlink(mq_name.c_str());
  shm_unlink("map");
  sem_close(my_sem_ptr);
  sem_unlink("/player1");
  ptr->postNotice("SIGTERM, SIGINT, and SIGHUP are trapped ");
  delete ptr;
  exit(0);*/
  ptr->postNotice("SIGTERM, SIGINT, and SIGHUP are trapped ");
  clean=true;

}



int write_queue(string st)
{

const char *str = st.c_str();

  if(mq_name=="/none")
  {
    cerr << "Please change mq_name from \"" << mq_name << "\" to something unique to you" << endl;
    exit(1);
  }

  mqd_t writequeue_fd; //message queue file descriptor

  //cout<<"mqq in write is--"<<mq_name<<endl;
  if((writequeue_fd=mq_open(mq_name.c_str(), O_WRONLY|O_NONBLOCK))==-1)
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


int read_queue()
{
  //I have added this signal-handling
  //code so that if you type ctrl-c to
  //abort the long, slow loop at the
  //end of main, then your message queue
  //will be properly cleaned up.
  struct sigaction exit_handler;
  exit_handler.sa_handler=clean_up;
  sigemptyset(&exit_handler.sa_mask);
  exit_handler.sa_flags=0;
  sigaction(SIGINT, &exit_handler, NULL);
  sigaction(SIGTERM, &exit_handler, NULL);
  sigaction(SIGHUP, &exit_handler, NULL);




  //make sure we can handle the SIGUSR2
  //message when the message queue
  //notification sends the signal
  struct sigaction action_to_take;
  //handle with this function
  action_to_take.sa_handler=read_message;
  //zero out the mask (allow any signal to interrupt)
  sigemptyset(&action_to_take.sa_mask);
  action_to_take.sa_flags=0;
  //tell how to handle SIGINT
  sigaction(SIGUSR2, &action_to_take, NULL);

  struct mq_attr mq_attributes;
  mq_attributes.mq_flags=0;
  mq_attributes.mq_maxmsg=10;
  mq_attributes.mq_msgsize=120;

  if((readqueue_fd=mq_open(mq_name.c_str(), O_RDONLY|O_CREAT|O_EXCL|O_NONBLOCK,
          S_IRUSR|S_IWUSR, &mq_attributes))==-1)
  {
    perror("mq_open in read");
    exit(1);
  }
  //set up message queue to receive signal whenever message comes in
  struct sigevent mq_notification_event;
  mq_notification_event.sigev_notify=SIGEV_SIGNAL;
  mq_notification_event.sigev_signo=SIGUSR2;
  mq_notify(readqueue_fd, &mq_notification_event);
}


int main( int argc, char **argv )
{

    if(argc>1){
      client_daemon(argv[1]);
      sleep(2);
    }
  GameBoard gb;
  //sem_t *my_sem_ptr;
  int ret;
	int fd;
	char chr;

	int rows=0;
	int coln=0;
	int i;

	int v = 0;
	int curr_pos;

/***************/
  struct sigaction my_action;   //structure object
  //initilize all the values
  my_action.sa_handler=sig_handler;
  sigemptyset(&my_action.sa_mask);
  my_action.sa_flags=0;
  my_action.sa_restorer=NULL ;
  //change sigint behaviour to call the function
  sigaction(SIGUSR1, &my_action, NULL);

	my_sem_ptr= sem_open(
           "/player1",
           O_CREAT|O_EXCL,
           S_IROTH| S_IWOTH| S_IRGRP| S_IWGRP| S_IRUSR| S_IWUSR,
           1
           );


	if(my_sem_ptr==SEM_FAILED)
	{
    if(errno==EEXIST)
    {perror("semphone present inside first palyer ");}
  }


    if(my_sem_ptr!=SEM_FAILED)
    {  //sem_wait
        sem_wait(my_sem_ptr);
	       //shm_open (creates shared memory)
        ret=shm_open("map", O_CREAT|O_RDWR,  S_IRUSR | S_IWUSR);

        if (ret==-1){perror("the shared memory open problem:");}
        else{perror("Shared memory created if 1 player:");}

         //open up the map file and determine rows & columns
        fd=open("mymap.txt", O_RDONLY);
        if(fd==-1)
        {perror("Opening mymap.txt");}
        //dynamic array..
        char *arr;
        arr=new char[CAPACITY];
        int read_bytes = read(fd, arr, CAPACITY);
        if(read_bytes==-1){
        perror("Reading mymap.txt file:");
        }

         cout << arr << endl;
         //	write(1,arr,10000);

  	    for (i=2; i<read_bytes; i++){
            if (arr[i] == '\n')
              ++rows;
            }

        for (i=2; i<read_bytes; i++){
                if (arr[i] != '\n'){
                ++coln;
		            }else{break;}
              }


        cout<<"number of rows:"<<rows<<endl;
        cout<<"number of columns:"<<coln<<endl;
         //ftruncate (grows shared memory)

	        if(ftruncate(ret,rows*coln)==-1)
	         {perror("ftruncate called:");
		         exit(1);}


   //mmap so that we can treat shared memory as "normal" memory
   GameBoard*  mp=(GameBoard*) mmap (NULL,rows*coln+sizeof(GameBoard),PROT_READ|PROT_WRITE,MAP_SHARED,ret,0);
    //char*  mp=(char*) mmap (NULL,read_bytes-1,PROT_READ|PROT_WRITE,MAP_SHARED,ret,0);
    //write(mp,arr,3000);
    //read in the file again, this time
    //convert each character into a byte that is stored into
    //initilize process id

      mp->daemonID=0; // initilize daemonID

        for(int r=0;r<5;r++){
          mp->pid[r]=0;
        }


        mp->pid[0]=getpid();

        mp->row_s=rows;
        mp->col_s=coln;


 	      int j=0;
        for (i=2; i<read_bytes; i++){
          if(arr[i]==' ')
		        {
              mp->map[j]=0;
		            j++;
              }
        else if(arr[i]=='*')
		      {
		          mp->map[j]|=G_WALL;
		            j++;
		            }
   	          }




    //the shared memory
    //do stuff for first player


    srand (time(NULL));

	   while(mp->map[v]==G_WALL){//real
	      v = rand() % (rows*coln-1) + 1;
      } mp->map[v]|=G_GOLD;



	     int k=0;
	      v=0;
	       while(k<5){//fool hardcoded
           v = rand() % (rows*coln-1) + 1;
	          if(mp->map[v]==G_WALL || mp->map[v]==G_GOLD || mp->map[v]==G_FOOL)
            {
	             continue;
             } mp->map[v]|=G_FOOL;
	            k++;
	           }



	            while(mp->map[v]==G_WALL || mp->map[v]==G_GOLD || mp->map[v]==G_FOOL){
                v = rand() % (rows*coln-1) + 1;
              }{ mp->map[v]|=G_PLR0; curr_pos=v;}


              for(int m=0;m<5;m++){
                if(mp->pid[m]!=0){
                  kill(mp->pid[m],SIGUSR1);
                }}
    mq_name="/tpatil1";
      read_queue();
    //mp->players=G_PLR0;

    sem_post(my_sem_ptr);// release sempohore



    if(mp->daemonID==0)
    {
      std::cerr << "process ID 1 is :"<<getpid()<< std::endl;
      create_daemon();

      std::cerr << "mp->DaemonID  is :"<<mp->daemonID<< std::endl;
    }
//     kill(mp->daemonID,SIGHUP);

	   Map goldMine(mp->map,rows,coln);
     int a=0;
     ptr = &goldMine;
   // goldMine.postNotice("This is a notice");
   sleep(2);
   kill(mp->daemonID,SIGHUP);
	  char key;
	   bool flag = false;

	    while((key=goldMine.getKey())!='Q'&& clean==false){
        if(key=='k'&& curr_pos-coln>=0){
		      if(mp->map[curr_pos-coln]!=G_WALL){
        //move up
        mp->map[curr_pos] &= ~G_PLR0;
        mp->map[curr_pos-coln]|=G_PLR0;
        curr_pos=curr_pos-coln;
        goldMine.drawMap();
            }
            if( curr_pos-coln<=0 && flag == true){
            break;
            }
            for(int m=0;m<5;m++){
              if(mp->pid[m]!=0){
            kill(mp->pid[m],SIGUSR1);
          }}
        }
        else if(key=='h'){
        //move left
        if(mp->map[curr_pos-1]!=G_WALL){
        mp->map[curr_pos] &= ~G_PLR0;
        mp->map[curr_pos-1]|=G_PLR0;
        curr_pos=curr_pos-1;;
        goldMine.drawMap();
            }
            if(curr_pos%coln == 0 && flag == true){
            break;
            }for(int m=0;m<5;m++){
              if(mp->pid[m]!=0){
            kill(mp->pid[m],SIGUSR1);
          }}
        }
        else if(key=='j' && curr_pos+coln<=rows*coln){
        //move down
        if(mp->map[curr_pos+coln]!=G_WALL){
        mp->map[curr_pos] &= ~G_PLR0;
        mp->map[curr_pos+coln]|=G_PLR0;
        curr_pos=curr_pos+coln;
        goldMine.drawMap();

        }
        if( curr_pos+coln >= rows*coln && flag == true){
            break;
            }for(int m=0;m<5;m++){
              if(mp->pid[m]!=0){
            kill(mp->pid[m],SIGUSR1);
          }}
        }
        else if(key=='l'){
        //move right
        if(mp->map[curr_pos+1]!=G_WALL && (curr_pos+1%coln)!=0){
        mp->map[curr_pos]&= ~G_PLR0;
        mp->map[curr_pos+1]|= G_PLR0;
        curr_pos=curr_pos+1;
        goldMine.drawMap();
            }
            if((curr_pos+1)%coln==0 && flag == true){
            break;
            }for(int m=0;m<5;m++){
              if(mp->pid[m]!=0){
            kill(mp->pid[m],SIGUSR1);
          }}
        }
        else if(key=='m'){
        	unsigned int plr = 0;
				for(int j=0;j<5;j++){
					if(mp->pid[j] != 0){
						if(j==1){plr |= G_PLR1;}
						if(j==2){plr |= G_PLR2;}
						if(j==3){plr |= G_PLR3;}
						if(j==4){plr |= G_PLR4;}
					}
				}

			unsigned	int ip=0;
        ip = goldMine.getPlayer(plr);

        if(ip!=0){
        if(ip==G_PLR0){mq_name="/tpatil1";}
        else if(ip==G_PLR1){ mq_name="/tpatil2";}
        else if(ip==G_PLR2){mq_name="/tpatil3";}
        else if(ip==G_PLR3){mq_name="/tpatil4";}
        else if(ip==G_PLR4){mq_name="/tpatil5";}
        //cout<<"value of mq_queuw is is ---->"<<mq_name<<endl;

        string strt=goldMine.getMessage();
        strt.insert(0,"Player 1 Says:");
        write_queue(strt);
        strt.clear();

      }

   	 }else if(key=='b'){

       string strt=goldMine.getMessage();
       strt.insert(0,"Player 1 Says:");
       //brodcast_msg(strt);
       for(int j=0;j<5;j++){
         if(mp->pid[j] != 0){
           if(j==1){mq_name="/tpatil2";}
           else if(j==2){mq_name="/tpatil3";}
           else if(j==3){mq_name="/tpatil4";}
           else if(j==4){mq_name="/tpatil5";}
           if (j!=0) {

            write_queue(strt);

           }
          }
        }
     }



         if(mp->map[curr_pos] & G_GOLD){
            goldMine.postNotice("you win..found REAL GOLD Please escape from game");
            mp->map[curr_pos] &= ~G_GOLD;
            flag =true;
            } else  if(mp->map[curr_pos] & G_FOOL){
            goldMine.postNotice("Sorry...you found FOOL's GOLD");
            }


              kill(mp->daemonID,SIGUSR1);
                  /*
              kill(mp->daemonID,SIGUSR2);
              kill(mp->daemonID,SIGHUP);
              */
	}



  if (flag==true) {

    for(int j=0;j<5;j++){
      if(mp->pid[j] != 0){
        if(j==1){mq_name="/tpatil2";}
        else if(j==2){mq_name="/tpatil3";}
        else if(j==3){mq_name="/tpatil4";}
        else if(j==4){mq_name="/tpatil5";}
        if (j!=0) {
          string temp="Player 1 won";
          //temp=strt;
          //std::stringstream ss;
          //ss << play << 1<<" won ";
          //play=ss.str();
          //temp.insert(0,play);

         write_queue(temp);
         //play="Player ";
        }
       }
     }
    /* code */
  }





  for(int m=0;m<5;m++){
    if(mp->pid[m]!=0){
  kill(mp->pid[m],SIGUSR1);
  }}


    mp->pid[0]=0;


    mp->map[curr_pos] &= ~G_PLR0;

    sem_wait(my_sem_ptr);
    //mp->players &= ~G_PLR0;
    sem_post(my_sem_ptr);
    //while(a=goldMine.getKey()!='Q');

    mq_close(readqueue_fd);
    mq_name="/tpatil1";
    mq_unlink(mq_name.c_str());

    kill(mp->daemonID,SIGHUP);


	//shm_unlink("map");
  bool ulink1=true;
  for(int c=0;c<5;c++){
    if(mp->pid[c]!=0)
      ulink1=false;
  }

    /*if(ulink1){
        shm_unlink("map");
		    sem_close(my_sem_ptr);
        sem_unlink("/player1");
      }*/



	 }
       else// fro subsquent players
       {

        my_sem_ptr = sem_open("/player1",O_RDWR,S_IROTH| S_IWOTH| S_IRGRP| S_IWGRP | S_IRUSR| S_IWUSR, 1);

        int v=0;
        //do stuff for subsequent player
        //cout<< "u rrr in secind plaeerrr"<< endl;

        sem_wait(my_sem_ptr);
        /*if(p==-1){
            perror("sem wait:");
            exit(1);
        }*/

        //shm_open (creates shared memory)

        cout<< "u rrr in seccond player"<< endl;

        int fd1;
        fd1=shm_open("map", O_RDWR,  S_IRUSR | S_IWUSR);

        if(fd==-1){
        perror("shared memory open else if:");
        }else{
        perror("shared memory open else:");
        }

    int s_rows;
	int s_cols;
	unsigned char current_player;

	read(fd1, &s_rows, sizeof(int));
	read(fd1, &s_cols, sizeof(int));



	 GameBoard*  mp=(GameBoard*) mmap (NULL,s_rows*s_cols+sizeof(GameBoard),PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0);

  std::cerr << "mp->DaemonID ELSE is :"<<mp->daemonID<< std::endl;

   int pl;
   // if(mp->players==G_PLR0){
    if((mp->pid[0] == 0)){
        current_player=G_PLR0;
        pl=0;
      mq_name="/tpatil1";}
    else if((mp->pid[1] == 0)){
        current_player=G_PLR1;
          pl=1;
        mq_name="/tpatil2";}
    else if((mp->pid[2] == 0)){
        current_player=G_PLR2;
          pl=2;
        mq_name="/tpatil3";}
    else if((mp->pid[3] == 0)){
        current_player=G_PLR3;
          pl=3;
        mq_name="/tpatil4";}
    else if((mp->pid[4] == 0)){
        current_player=G_PLR4;
          pl=4;
          mq_name="/tpatil5";}
    else {//cout <<"players are full"<<endl;

        cout<< "----------------------------------"<<"\n"<<
                "|*******Players are full*********|"<<"\n"<<
                "----------------------------------"<<endl;
                sem_post(my_sem_ptr);
            return 0;
        }

          read_queue();

        cout<<"number of else part rows"<<s_rows<<endl;
        cout<<"number of else part columns "<<s_cols<<endl;

        mp->pid[pl]=getpid();
        kill(mp->daemonID,SIGHUP);


        while(mp->map[v]!=0){
        v = rand() % (s_rows*s_cols-1) + 1;
        }{ mp->map[v]|=current_player; curr_pos=v;}

         sem_post(my_sem_ptr);

         for(int m=0;m<5;m++){
           if(mp->pid[m]!=0){
         kill(mp->pid[m],SIGUSR1);
         }}

        sem_wait(my_sem_ptr);
        //mp->players|=current_player;
        sem_post(my_sem_ptr);

        Map goldMine(mp->map,s_rows,s_cols);
         int a=0;
         ptr = &goldMine;
       // goldMine.postNotice("This is a notice");
       // while(a=goldMine.getKey()!='Q');
       char key;
       bool flag = false;
       std:string play="Player ";


        while((key=goldMine.getKey())!='Q'&& clean==false){
        if(key=='k' && curr_pos-s_cols>=0){
		if(mp->map[curr_pos-s_cols]!=G_WALL){
        //move up
        mp->map[curr_pos] &= ~current_player;
        mp->map[curr_pos-s_cols]|= current_player;//mp->players;
        curr_pos=curr_pos-s_cols;
        goldMine.drawMap();
          }
          if(curr_pos-s_cols<=0 && flag==true){
          break;
          }for(int m=0;m<5;m++){
          if(mp->pid[m]!=0){
          kill(mp->pid[m],SIGUSR1);
          }}
        }
        else if(key=='h'){
        //move left
        if(mp->map[curr_pos-1]!=G_WALL){
        mp->map[curr_pos] &= ~current_player;
        mp->map[curr_pos-1]|= current_player;  //mp->players;
        curr_pos=curr_pos-1;;
        goldMine.drawMap();
            }
            if(curr_pos%s_cols==0 && flag==true){
            break;
            }for(int m=0;m<5;m++){
            if(mp->pid[m]!=0){
            kill(mp->pid[m],SIGUSR1);
          }}
        }
        else if(key=='j' && curr_pos+s_cols<=s_rows*s_cols){
        //move down
        if(mp->map[curr_pos+s_cols]!=G_WALL){
        mp->map[curr_pos] &= ~current_player;
        mp->map[curr_pos+s_cols]|= current_player;//mp->players;
        curr_pos=curr_pos+s_cols;
        goldMine.drawMap();
        }
        if((curr_pos+s_cols)>=s_rows*s_cols && flag==true){
            break;
            }for(int m=0;m<5;m++){
              if(mp->pid[m]!=0){
            kill(mp->pid[m],SIGUSR1);
          }}
        }
        else if(key=='l'){
        //move right
        if(mp->map[curr_pos+1]!=G_WALL && (curr_pos+1%s_cols)!=0){
        mp->map[curr_pos] &= ~current_player;
        mp->map[curr_pos+1]|= current_player; //mp->players;
        curr_pos=curr_pos+1;
        goldMine.drawMap();
            }
            if((curr_pos+1)%s_cols==0 && flag==true){
            break;
            }for(int m=0;m<5;m++){
              if(mp->pid[m]!=0){
            kill(mp->pid[m],SIGUSR1);
          }}
        }else if(key=='m'){
        	unsigned int plr = 0;int p;
				for(int j=0;j<5;j++){
					if(mp->pid[j] != 0){
            if(j==0){plr |= G_PLR0;}
						if(j==1){plr |= G_PLR1;}
						if(j==2){plr |= G_PLR2;}
						if(j==3){plr |= G_PLR3;}
						if(j==4){plr |= G_PLR4;}
					}
				}
        plr &= ~current_player;
        int ip2=0;
				ip2 = goldMine.getPlayer(plr);

          if(ip2!=0){
          //    cout<<"value of ip is ---->"<<ip<<endl;
        if(ip2==G_PLR0){mq_name="/tpatil1";}
        else if(ip2==G_PLR1){ mq_name="/tpatil2";}
        else if(ip2==G_PLR2){ mq_name="/tpatil3";}
        else if(ip2==G_PLR3){ mq_name="/tpatil4";}
        else if(ip2==G_PLR4){ mq_name="/tpatil5";}
      //  cout<<"value of mq_queuw is is ---->"<<mq_name<<endl;
        string strt=goldMine.getMessage();
        std::stringstream ss;
        ss << play << (pl+1)<<" says:";
        play=ss.str();
        strt.insert(0,play);
        write_queue(strt);
        play="Player";
      }
    }else if(key=='b'){

      string strt=goldMine.getMessage();
      //brodcast_msg(strt);
      for(int j=0;j<5;j++){
        if(mp->pid[j] != 0){
          if(j==0){mq_name="/tpatil1";}
          else if(j==1){mq_name="/tpatil2";}
          else if(j==2){mq_name="/tpatil3";}
          else if(j==3){mq_name="/tpatil4";}
          else if(j==4){mq_name="/tpatil5";}
          if (j!=pl) {
            string temp;
            temp=strt;
            std::stringstream ss;
            ss << play << (pl+1)<<" says:";
            play=ss.str();
            temp.insert(0,play);

           write_queue(temp);
           play="Player";
          }
         }
       }
    }

         if(mp->map[curr_pos] & G_GOLD){
            goldMine.postNotice("you win..found REAL GOLD Please escape from game");
                mp->map[curr_pos] &= ~G_GOLD;
                flag = true;
            } else  if(mp->map[curr_pos] & G_FOOL){
            goldMine.postNotice("Sorry..You found FOOL's GOLD");
            }



          }

          if (flag==true) {

            for(int j=0;j<5;j++){
              if(mp->pid[j] != 0){
                if(j==0){mq_name="/tpatil1";}
                else if(j==1){mq_name="/tpatil2";}
                else if(j==2){mq_name="/tpatil3";}
                else if(j==3){mq_name="/tpatil4";}
                else if(j==4){mq_name="/tpatil5";}
                if (j!=pl) {
                //  string temp;
                  //temp=strt;
                  std::stringstream ss;
                  ss << play << (pl+1)<<" won";
                  play=ss.str();
                  //temp.insert(0,play);

                 write_queue(play);
                 play="Player ";
                }
               }
             }
            /* code */
          }


          if(current_player==G_PLR0){
            mq_name="/tpatil1";
          }else if(current_player==G_PLR1){
            mq_name="/tpatil2";
          }else if(current_player==G_PLR2){
            mq_name="/tpatil3";
          }else if(current_player==G_PLR3){
            mq_name="/tpatil4";
          }else if(current_player==G_PLR4){
            mq_name="/tpatil5";
          }

          mq_close(readqueue_fd);
          mq_unlink(mq_name.c_str());

        for(int m=0;m<5;m++){
          if(mp->pid[m]!=0){
            kill(mp->pid[m],SIGUSR1);
          }}


        mp->map[curr_pos] &= ~current_player;//turn off bitd when player quits..
        mp->pid[pl]=0;

        sem_wait(my_sem_ptr);
      //  mp->players &= ~current_player;
        sem_post(my_sem_ptr);


        mq_close(readqueue_fd);
        mq_unlink(mq_name.c_str());
        kill(mp->daemonID,SIGHUP);
        bool ulink=true;
        for(int c=0;c<5;c++){
          if(mp->pid[c]!=0)
            ulink=false;
        }

      /*  if(ulink){
      //  cout<<"closing semaphone inside else"<<endl;
        shm_unlink("map");
		    sem_close(my_sem_ptr);
        sem_unlink("/player1");

      }*/

	}

}
