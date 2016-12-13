/**
Tejas Patil -006949762
*/

#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>


struct Node{
int data;
struct Node* next;
struct Node* prev;
};

struct Node* Head;
struct Node* Tail;
struct Node* tempnext;


/**
 * Function to initialize any global variables for the scheduler.
 */
void init(){

Head = NULL;
Tail = NULL;

}

/**
 * Function to add a process to the scheduler
 * @Param pid - the ID for the process/thread to be added to the
 *      scheduler queue
 * @return true/false response for if the addition was successful
 */
int addProcess(int pid){



    struct Node* currNode = (struct Node*)malloc(sizeof(struct Node));
    struct Node* temp = Head;

    if(Head == NULL)
    {

    currNode->data = pid;
    currNode->next = currNode;
    currNode->prev = currNode;

    Head = currNode;
    Tail = currNode;

    return 1;
    }

    else{

  //  while(temp->next != NULL) temp = temp->next;

    temp = Tail;

    currNode->data = pid;

    temp->next = currNode;

    currNode->prev = temp;

    currNode->next = Head;

    Head->prev = Tail;

    Tail = currNode;
    tempnext = Head;

    return 1;
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

 struct Node* temp = Head;

 while(temp->data != pid) temp = temp->next;

 if(temp == Head && Head->next == Head){

    Head=NULL;
    free(temp);
    return 1;
 }

 else if(temp == Tail){

    Tail = temp->prev;

    Tail->next = Head;

    Head->prev = Tail;

    free(temp);

 return 1;
 }

 else if(temp == Head && Head->next != Head){

 Head = temp->next;

 Head->prev = Tail;

 Tail->next = Head;

 free(temp);


 return 1;
 }

 else{

    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;

    free(temp);

 return 1;
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

*time = 4;
int pid;


//tempnext = Head;

if(Head != NULL)
{
pid = tempnext->data;
tempnext = tempnext->next;
//printf("in next loop\n");


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

if(Head == NULL){

    return 0;
}
else
    return 1;
}
