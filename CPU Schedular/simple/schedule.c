/**
Tejas Patil -006949762
*/


#include "schedule.h"
#include <stdlib.h>

/**
*structure for doubly linked list.
*/

struct Node{
int data;
struct Node* next;
struct Node* prev;
};

struct Node* Head;

/**
 * Function to initialize any global variables for the scheduler.
 */
void init(){

Head = NULL;

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
    currNode->next = NULL;
    currNode->prev = NULL;

    Head = currNode;

    return 1;
    }

    else{

    while(temp->next != NULL) temp = temp->next;

    currNode->data = pid;
    currNode->next = NULL;

    temp->next = currNode;
    currNode->prev = temp;

    return 1;
    }

    return 0;
}

/**
 * Function to get the next process from the scheduler
 *
 * @Return returns the thread id of the next process that should be
 *      executed, returns -1 if there are no processes
 */
int nextProcess(){

struct Node* temp = Head;
int npid;

if(Head != NULL){


npid = temp->data;
Head=Head->next;
free(temp);

return npid;

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
