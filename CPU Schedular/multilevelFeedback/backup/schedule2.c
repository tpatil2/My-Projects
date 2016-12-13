/**
Name: Tejas Patil
Csu Id: 006949762
*/


#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>

struct Node{
int data;
int prio;
int ageC;
struct Node* next;
struct Node* prev;
};

struct Node* Head1;
struct Node* Tail1;

struct Node* Head2;
struct Node* Tail2;

struct Node* Head3;
struct Node* Tail3;



struct Node* tempnext4;
struct Node* tempnext3;
struct Node* tempnext2;
struct Node* tempnext1;
struct Node* tempn;
int prio;


//int present;

int counter;
/**
 * Function to initialize any global variables for the scheduler.
 */
void init(){

Head1 = NULL;
Tail1 = NULL;

Head2 = NULL;
Tail2 = NULL;

Head3 = NULL;
Tail3 = NULL;



counter = 4;
tempn=NULL;

}

/**
 * Function called every simulated time period to provide a mechanism
 * to age the scheduled processes and trigger feedback if needed.
 */
void age(){

int id;
struct Node* agetemp2=Head2;
struct Node* agetemp3=Head3;
if(Head2 != NULL){
while(agetemp2 != Tail2){
    agetemp2->ageC++;
   // printf("in age head 1 %d ",agetemp2->ageC );
    agetemp2=agetemp2->next;
}agetemp2->ageC++;
}
if(Head3 != NULL){
while(agetemp3 != Tail3){
    agetemp3->ageC++;
   //  printf("in age head 1 %d ",agetemp3->ageC );
    agetemp3=agetemp3->next;
}agetemp3->ageC++;
}

struct Node* agecounter=Head2;
struct Node* agecounter2=Head3;

if(Head2 != NULL ){




    prio=2;

//printf("in age counter agec1 %d\n ",agecounter->ageC);


    if(agecounter->ageC==1000){
  //  printf("in agecounter:----age: %d\n",agecounter->ageC);
   // printf("in agecounter:----process data: %d\n",agecounter->data );
   // printf("in agecounter:----age: %d\n",agecounter->next->ageC);
   // printf("in agecounter:----process data: %d\n",agecounter->next->data );

//printf("in age head 1\n ");
    id=agecounter->data;
    prio=2;

    //exit(0);
    removeProcess(id);

    addProcess(id,0);
    }




}


if(Head3!=NULL){
int i=0;

for(i=0;i<3;i++){

//printf("in agecounter:----age: %d\n",agecounter2->ageC);
//printf("in agecounter:----process data: %d\n",agecounter2->data );

 if(agecounter2->ageC==1000){
     // printf("in age head 2 ");
    id=agecounter2->data;
    prio=3;

    agecounter2=agecounter2->next;

    removeProcess(id);

    addProcess(id,1);
}

}
}
/*
if(Head3 != NULL){



while(agecounter2 != Tail3){
    prio=3;

//printf("in age counter agec2 %d\n ",agecounter2->ageC);
    if(agecounter2->ageC==1000){
      printf("in age head 2 ");
    id=agecounter2->data;
    prio=3;
    removeProcess(id);

    addProcess(id,1);

    }
    agecounter2=agecounter2->next;
    }
    if(agecounter2->ageC==1000){

    id=agecounter2->data;
    prio=3;
    removeProcess(id);

    addProcess(id,1);

    }
}*/

}

/**
 * Function to add a process to the scheduler
 * @Param pid - the ID for the process/thread to be added to the
 *      scheduler queue
 * @Param priority - priority of the process being added
 * @return true/false response for if the addition was successful
 */
int addProcess(int pid, int priority){


struct Node* currNode = (struct Node*)malloc(sizeof(struct Node));

    if(priority == 0){
    struct Node* temp = Head1;
    if(Head1 == NULL)
    {
    currNode->data = pid;
    currNode->ageC = 0;
    currNode->prio = priority;
    currNode->next = currNode;
    currNode->prev = currNode;

    Head1 = currNode;
    Tail1 = currNode;

    return 1;
    }

    else{
    temp = Tail1;
    currNode->data = pid;
    currNode->ageC = 0;
    currNode->prio = priority;
    temp->next = currNode;
    currNode->prev = temp;
    currNode->next = Head1;
    Head1->prev = Tail1;
    Tail1 = currNode;
    if(pid < 10){
    tempnext4 = Head1;
    }
    return 1;
    }
    }
    else if(priority == 1){

    struct Node* temp = Head2;
     if(Head2 == NULL)
    {

    currNode->data = pid;
    currNode->ageC = 0;
    currNode->prio = priority;
    currNode->next = currNode;
    currNode->prev = currNode;
    Head2 = currNode;
    Tail2 = currNode;

    return 1;
    }

    else{
    temp = Tail2;
    currNode->data = pid;
    currNode->ageC = 0;
    currNode->prio = priority;
    temp->next = currNode;
    currNode->prev = temp;
    currNode->next = Head2;

    Head2->prev = Tail2;
    Tail2 = currNode;
    if(pid < 10){
     tempnext3 = Head2;
    }
    return 1;
    }
    }
    else if(priority == 2){
    struct Node* temp = Head3;
     if(Head3 == NULL)
    {

    currNode->data = pid;
     currNode->ageC = 0;
    currNode->prio = priority;
    currNode->next = currNode;
    currNode->prev = currNode;

    Head3 = currNode;
    Tail3 = currNode;

    return 1;
    }

    else{
    temp = Tail3;
    currNode->data = pid;
     currNode->ageC = 0;
    currNode->prio = priority;
    temp->next = currNode;
    currNode->prev = temp;
    currNode->next = Head3;
    Head3->prev = Tail3;
    Tail3 = currNode;

    if(pid<10){
    tempnext2 = Head3;
    }
    return 1;
    }
    }

	return 0;
}

/**
 * Function to remove a process from the scheduler queue
 * @Param pid - the ID for the process/thread to be removed from the
 *      scheduler queue
 * @Return true/false response for if the removal was successful
 */
int removeProcess(int pid){


int present=0;

struct Node* temp1 = Head1;
struct Node* temp2 = Head2;
struct Node* temp3 = Head3;

if(prio==1)
{
while(temp1->data != pid && present == 0 ){
    temp1 = temp1->next;
    if(temp1->data == pid){
    present = 1;
    }
}
}


if(prio==2)
{
while(temp2->data != pid && present == 0 ){
    temp2 = temp2->next;
    if(temp2->data == pid){
    present = 2;
    }
}
}


if(prio==3)
{
while(temp3->data != pid && present == 0 ){
    temp3 = temp3->next;
    if(temp3->data == pid){
    present = 3;
    }
    }
}



if(prio==1)
{
    if(temp1->data == pid){
    if(temp1 == Head1 && Head1->next == Head1){
    Head1=NULL;
    free(temp1);
    return 1;
 }
 else if(temp1 == Tail1){
    Tail1 = Tail1->prev;
    Tail1->next = Head1;
    Head1->prev = Tail1;
    free(temp1);
 return 1;
 }
 else if(temp1 == Head1 && Head1->next != Head1){
    Head1 = temp1->next;
    Head1->prev = Tail1;
    Tail1->next = Head1;
    free(temp1);
    return 1;
 }

 else{
    temp1->prev->next = temp1->next;
    temp1->next->prev = temp1->prev;
    free(temp1);
 return 1;
        }
    }
 }

if(prio==2)
{
if (temp2->data == pid )
{
if(temp2 == Head2 && Head2->next == Head2){
    Head2=NULL;
    free(temp2);
    return 1;
 }
 else if(temp2 == Tail2){
    Tail2 = temp2->prev;
    Tail2->next = Head2;
    Head2->prev = Tail2;
    free(temp2);

 return 1;
 }
 else if(temp2 == Head2 && Head2->next != Head2){
 Head2 = temp2->next;
 Head2->prev = Tail2;
 Tail2->next = Head2;
 free(temp2);
 return 1;
 }
 else{
    temp2->prev->next = temp2->next;
    temp2->next->prev = temp2->prev;
    free(temp2);
 return 1;
        }
    }
}

if(prio==3)
{
if (temp3->data == pid){
if(temp3 == Head3 && Head3->next == Head3){

    Head3=NULL;
    free(temp3);
    return 1;
 }
 else if(temp3 == Tail3){
    Tail3 = temp3->prev;
    Tail3->next = Head3;
    Head3->prev = Tail3;
    free(temp3);
 return 1;
 }
 else if(temp3 == Head3 && Head3->next != Head3){
 Head3 = temp3->next;
 Head3->prev = Tail3;
 Tail3->next = Head3;
 free(temp3);
 return 1;
 }
 else{
    temp3->prev->next = temp3->next;
    temp3->next->prev = temp3->prev;
    free(temp3);
 return 1;
 }
}
}
  return 0;
  }
/**
 * Function to get the next process from the scheduler
 * @Param time - pass by pointer variable to store the quanta of time
 * 		the scheduled process should run for, returns -1 if should run to completion.
 * @Return returns the thread id of the next process that should be
 *      executed, returns -1 if there are no processes
 */
int nextProcess(int *time){

if(Head1!=NULL){
*time =0;
int pid;
pid = Head1->data;
prio=1;
removeProcess(pid);
return pid;

}

else if( Head2 != NULL){
*time = 4;
int pid;

    pid = Head2->data;
    Head2->ageC=-4;
    tempn = Head2;
    Head2=Head2->next;
    Tail2=tempn;
    Tail2->next=Head2;
    Head2->prev=Tail2;

//printf("in next loop\n");
prio=2;

return pid;

}
else if( Head3 != NULL){
*time = 1;
int pid;

    pid = Head3->data;
    Head3->ageC=-1;
    tempn = Head3;
    Head3=Head3->next;
    Tail3=tempn;
    Tail3->next=Head3;
    Head3->prev=Tail3;

//printf("in next loop\n");
prio=3;
return pid;

}

return -1;
}

/**
 * Function that returns a boolean 1 True/0 False based on if there are any
 * processes still scheduled
 * @Return 1 if there are processes still scheduled 0 if there are no more
 *		scheduled processes
 */
int hasProcess(){

if((Head1 == NULL) && (Head2 == NULL) && (Head3 == NULL)){
    return 0;
    }
else
	return 1;
}
