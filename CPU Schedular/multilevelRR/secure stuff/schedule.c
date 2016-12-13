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
struct Node* tempn;
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
tempn=NULL;
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
    if(pid < 10){
    tempnext4 = Head1;
    }
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

    if(pid < 10){
     tempnext3 = Head2;
    }



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

    if(pid<10){
    tempnext2 = Head3;
    }
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

    if(pid<10){
    tempnext1 = Head4;
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
//
//
////int i=0,j=0,k=0,l=0;
///*
//struct Node* temp1 = Head1;
//struct Node* temp2 = Head2;
//struct Node* temp3 = Head3;
//struct Node* temp4 = Head4;
//*/
//
//if(Head1!=NULL){
//
//
//struct Node* t = Head1;
//
//if(Head1->data == pid){
//
//struct Node* t = Head1;
//
//if(Head1->next==Head1){
//Head1->next = NULL;
//Head1->prev = NULL;
//Head1 =NULL;
//free(Head1);
//
//return 1;
//}
//else{
//Head1 = Head1->next;
//Tail1->next=Head1;
//Head1->prev=Tail1;
//free(t);
//return 1;
//}
//}else{
//
//if(Tail1->data == pid){
//t=Tail1;
//if(Head1->next==Tail1)
//{
// Head1->next=Head1;
// Head1->prev=Head1;
// Tail1=Head1;
//}
//else
//{
//Tail1=Tail1->prev;
//Tail1->next=Head1;
//}
//free(t);
//return 1;
//}
//}
//
//
//}
//
//if(Head2!=NULL){
//struct Node* t = Head2;
//if(Head2->data == pid){
//
//
//
//if(Head2->next==Head2){
////Head2 =NULL;
//Head2->next = NULL;
//Head2->prev = NULL;
//Head2 = NULL;
//free(Head2);
//return 1;
//}
//else{
//Head2 = Head2->next;
//Tail2->next=Head2;
//Head2->prev=Tail2;
//
//free(t);
//return 1;
//}
//}
//else{
//
//if(Tail2->data == pid){
//t=Tail2;
//if(Head2->next==Tail2)
//{
// Head2->next=Head2;
// Head2->prev=Head2;
// Tail2=Head2;
//}
//else
//{
//Tail2=Tail2->prev;
//Tail2->next=Head2;
//}
//free(t);
//return 1;
//}
//}
//
//
//}
//
//if(Head3!=NULL){
//struct Node* t = Head3;
//if(Head3->data == pid){
//
//
//if(Head3->next==Head3){
////Head3 =NULL;
//Head3->next = NULL;
//Head3->prev = NULL;
//Head3 = NULL;
//free(Head3);
//return 1;
//}
//else{
//Head3 = Head3->next;
//Tail3->next=Head3;
//Head3->prev=Tail3;
//
//free(t);
//return 1;
//}
//}else{
//
//if(Tail3->data == pid){
//t=Tail3;
//if(Head3->next==Tail3)
//{
// Head3->next=Head3;
// Head3->prev=Head3;
// Tail3=Head3;
//}
//else
//{
//Tail3=Tail3->prev;
//Tail3->next=Head3;
//}
//free(t);
//return 1;
//}
//}
//}
//
//if(Head4!=NULL){
//struct Node* t = Head4;
//if(Head4->data == pid){
//
//
//if(Head4->next==Head4){
////Head4 =NULL;
//Head4->next = NULL;
//Head4->prev = NULL;
//Head4 = NULL;
//free(Head4);
//
//return 1;
//}
//else{
//Head4 = Head4->next;
//Tail4->next=Head4;
//Head4->prev=Tail4;
//
//free(t);
//return 1;
//}
//}else{
//
//if(Tail4->data == pid){
//t=Tail4;
//if(Head4->next==Tail4)
//{
// Head4->next=Head4;
// Head4->prev=Head4;
// Tail4=Head4;
//}
//else
//{
//Tail4=Tail4->prev;
//Tail4->next=Head4;
//}
//free(t);
//}
//}
//}
//
//  return 0;




//int i=0,j=0,k=0,l=0;
int present=0;

struct Node* temp1 = Head1;
struct Node* temp2 = Head2;
struct Node* temp3 = Head3;
struct Node* temp4 = Head4;

//printf("Scheduled Pid: %d\n", pid);
//printf("Head Pid: %d\n", temp1->data);
if(prio==1)
{
while(temp1->data != pid && present == 0 ){
//printf("Scheduled Pid in 1st while: %d\n", temp1->data);

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
if(prio==4)
{
while(temp4->data != pid &&  present == 0){

    temp4 = temp4->next;
    if(temp4->data == pid){
    present = 4;
    }

}
}
if(prio==1)
{
if(temp1->data == pid){// && present == 1){
//printf("Inside temp1->data %d\n", temp1->data);

if(temp1 == Head1 && Head1->next == Head1){
//printf("in condition 1\n");
    Head1=NULL;
    free(temp1);
    return 1;
 }

 else if(temp1 == Tail1){

//printf("in condition 2\n");
    Tail1 = Tail1->prev;

    Tail1->next = Head1;

    Head1->prev = Tail1;

    free(temp1);

 return 1;
 //exit(0);
 }

 else if(temp1 == Head1 && Head1->next != Head1){
//printf("in condition 3\n");
 Head1 = temp1->next;

 Head1->prev = Tail1;

 Tail1->next = Head1;

 free(temp1);


 return 1;
 }

 else{
//printf("in condition 4\n");
    temp1->prev->next = temp1->next;
    temp1->next->prev = temp1->prev;

    free(temp1);

 return 1;
 }
 }
 }
if(prio==2)
{
if (temp2->data == pid )//&& Head2!=NULL){// && present == 2){
{
//printf("Inside temp2->data %d\n", temp2->data);

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
if (temp3->data == pid){//{ && Head3!=NULL){//} && present == 3){
//printf("Inside temp3->data %d\n", temp3->data);

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
if(prio==4)
{
 if(temp4->data == pid){// && Head4!=NULL){//} && present == 4){
//printf("Inside temp4->data %d\n", temp4->data);

if(temp4 == Head4 && Head4->next == Head4){

    Head4=NULL;
    free(temp4);
    return 1;
 }

 else if(temp4 == Tail4){

    Tail4 = temp4->prev;

    Tail4->next = Head4;

    Head4->prev = Tail4;

    free(temp4);

 return 1;
 }

 else if(temp4 == Head4 && Head4->next != Head4){

 Head4 = temp4->next;

 Head4->prev = Tail4;

 Tail4->next = Head4;

 free(temp4);


 return 1;
 }

 else{

    temp4->prev->next = temp4->next;
    temp4->next->prev = temp4->prev;

    free(temp4);

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

  //  pid = tempnext4->data;
    //tempnext4 = tempnext4->next;
    pid = Head1->data;
    tempn = Head1;
    Head1=Head1->next;
    Tail1=tempn;
     Tail1->next=Head1;
     Head1->prev=Tail1;
    counter=3;
    prio=1;
    return pid;
    }
    counter = 3;

}


if(counter == 3){
    int pid;
     *time = 3;
    if(Head2 != NULL){
   // pid = tempnext3->data;
    //tempnext3 = tempnext3->next;

    pid = Head2->data;
    tempn = Head2;
    Head2=Head2->next;
    Tail2=tempn;
Tail2->next=Head2;
     Head2->prev=Tail2;
    counter = 2;
   prio=2;
return pid;
    }
    counter = 2;
}

if(counter == 2){
    int pid;
     *time = 2;
    if(Head3 != NULL){
   // pid = tempnext2->data;
    //tempnext2 = tempnext2->next;
    pid = Head3->data;
    tempn = Head3;
    Head3=Head3->next;
    Tail3=tempn;
    Tail3->next=Head3;
     Head3->prev=Tail3;
    counter = 1;
    prio=3;
return pid;
    }
    counter = 1;
}


if(counter == 1){
    int pid;
     *time = 1;
    if(Head4 != NULL){
   // pid = tempnext1->data;
   // tempnext1 = tempnext1->next;

    pid = Head4->data;
    tempn = Head4;
    Head4=Head4->next;
    Tail4=tempn;
    Tail4->next=Head4;
     Head4->prev=Tail4;
    counter = 4;
 prio=4;
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
