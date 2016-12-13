#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>




struct Node{
int data;
int prio;
struct Node* next;
struct Node* prev;
};

struct Node* Head1;
struct Node* Tail1;

struct Node* Head2;
struct Node* Tail2;

struct Node* Head3;
struct Node* Tail3;

struct Node* Head4;
struct Node* Tail4;

struct Node* tempnext4;
struct Node* tempnext3;
struct Node* tempnext2;
struct Node* tempnext1;
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

Head4 = NULL;
Tail4 = NULL;

counter = 4;
//present = 0;




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

    if(priority == 1){

    struct Node* temp = Head1;

    if(Head1 == NULL)
    {

    currNode->data = pid;
    currNode->prio = priority;
    currNode->next = currNode;
    currNode->prev = currNode;

    Head1 = currNode;
    Tail1 = currNode;

    return 1;
    }

    else{

  //  while(temp->next != NULL) temp = temp->next;

    temp = Tail1;

    currNode->data = pid;

    currNode->prio = priority;

    temp->next = currNode;

    currNode->prev = temp;

    currNode->next = Head1;

    Head1->prev = Tail1;

    Tail1 = currNode;
    tempnext4 = Head1;

    return 1;
    }



    }
    else if(priority == 2){

    struct Node* temp = Head2;
     if(Head2 == NULL)
    {

    currNode->data = pid;
    currNode->prio = priority;
    currNode->next = currNode;
    currNode->prev = currNode;

    Head2 = currNode;
    Tail2 = currNode;

    return 1;
    }

    else{

  //  while(temp->next != NULL) temp = temp->next;

    temp = Tail2;

    currNode->data = pid;

    currNode->prio = priority;

    temp->next = currNode;

    currNode->prev = temp;

    currNode->next = Head2;

    Head2->prev = Tail2;

    Tail2 = currNode;
    tempnext3 = Head2;

    return 1;
    }


    }
    else if(priority == 3){
    struct Node* temp = Head3;
     if(Head3 == NULL)
    {

    currNode->data = pid;
    currNode->prio = priority;
    currNode->next = currNode;
    currNode->prev = currNode;

    Head3 = currNode;
    Tail3 = currNode;

    return 1;
    }

    else{

  //  while(temp->next != NULL) temp = temp->next;

    temp = Tail3;

    currNode->data = pid;

    currNode->prio = priority;

    temp->next = currNode;

    currNode->prev = temp;

    currNode->next = Head3;

    Head3->prev = Tail3;

    Tail3 = currNode;
    tempnext2 = Head3;

    return 1;
    }
    }
    else if(priority == 4){
    struct Node* temp = Head4;
     if(Head4 == NULL)
    {

    currNode->data = pid;
    currNode->prio = priority;
    currNode->next = currNode;
    currNode->prev = currNode;

    Head4 = currNode;
    Tail4 = currNode;

    return 1;
    }

    else{

  //  while(temp->next != NULL) temp = temp->next;

    temp = Tail4;

    currNode->data = pid;

    currNode->prio = priority;

    temp->next = currNode;

    currNode->prev = temp;

    currNode->next = Head4;

    Head4->prev = Tail4;

    Tail4 = currNode;
    tempnext1 = Head4;

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


//int i=0,j=0,k=0,l=0;
/*
struct Node* temp1 = Head1;
struct Node* temp2 = Head2;
struct Node* temp3 = Head3;
struct Node* temp4 = Head4;
*/

if(Head1!=NULL){
if(Head1->data == pid){

struct Node* t = Head1;

if(Head1->next==Head1){
Head1->next = NULL;
Head1->prev = NULL;
Head1 =NULL;
free(Head1);

return 1;
}
else{
Head1 = Head1->next;
Tail1->next=Head1;
Head1->prev=Tail1;
free(t);
return 1;
}
}
}

if(Head2!=NULL){
if(Head2->data == pid){

struct Node* t = Head2;

if(Head2->next==Head2){
//Head2 =NULL;
Head2->next = NULL;
Head2->prev = NULL;
Head2 = NULL;
free(Head2);
return 1;
}
else{
Head2 = Head2->next;
Tail2->next=Head2;
Head2->prev=Tail2;

free(t);
return 1;
}
}
}

if(Head3!=NULL){
if(Head3->data == pid){

struct Node* t = Head3;
if(Head3->next==Head3){
//Head3 =NULL;
Head3->next = NULL;
Head3->prev = NULL;
Head3 = NULL;
free(Head3);
return 1;
}
else{
Head3 = Head3->next;
Tail3->next=Head3;
Head3->prev=Tail3;

free(t);
return 1;
}
}
}

if(Head4!=NULL){
if(Head4->data == pid){

struct Node* t = Head4;
if(Head4->next==Head4){
//Head4 =NULL;
Head4->next = NULL;
Head4->prev = NULL;
Head4 = NULL;
free(Head4);

return 1;
}
else{
Head4 = Head4->next;
Tail4->next=Head4;
Head4->prev=Tail4;

free(t);
return 1;
}
}
}

  return 0;

}
/**
 * Function to get the next process from the scheduler
 * @Param time - pass by pointer variable to store the quanta of time
 * 		the scheduled process should run for
 * @Return returns the thread id of the next process that should be
 *      executed, returns -1 if there are no processes
 */
int nextProcess(int *time){

if(counter == 4){
    int pid;
    *time = 4;

    if(Head1 != NULL){
    pid = tempnext4->data;
    tempnext4 = tempnext4->next;

    counter=3;

    return pid;
    }
    counter = 3;

}


if(counter == 3){
    int pid;
     *time = 3;
    if(Head2 != NULL){
    pid = tempnext3->data;
    tempnext3 = tempnext3->next;

    counter = 2;

return pid;
    }
    counter = 2;
}

if(counter == 2){
    int pid;
     *time = 2;
    if(Head3 != NULL){
    pid = tempnext2->data;
    tempnext2 = tempnext2->next;

    counter = 1;

return pid;
    }
    counter = 1;
}


if(counter == 1){
    int pid;
     *time = 1;
    if(Head4 != NULL){
    pid = tempnext1->data;
    tempnext1 = tempnext1->next;

    counter = 4;

return pid;
    }
    counter = 4;
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


if((Head1 == NULL) && (Head2 == NULL) && (Head3 == NULL) && (Head4 == NULL)){

    return 0;
}
else
    return 1;
}
