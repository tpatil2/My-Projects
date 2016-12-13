
/*****************************************************************************
*Name: Tejas  Patil
*Csu Id : 006949762
*Assignment No 5  - DNS CSCI340-DNS-Name-Resolution-Engine
******************************************************************************/




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "multi-lookup.h"
#include <pthread.h>
#include <unistd.h>



#include "util.h"
#include "queue.h"


#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"


//global variables
//mutex delceration

pthread_mutex_t mutq;
pthread_mutex_t mutf;
queue que;// queue object
FILE* outputfp = NULL;//output file
int threadspresent_flag = 1;// flag to check running threads




/********************************************************************************************
*Main Function: defination which accepts input file and creats threads and also handles errors
*               while creating threads and opeing file.
*
*input: file inputs
*return:int
*********************************************************************************************/



int  main(int argc, char* argv[]){

//local variables

int num_res = sysconf(_SC_NPROCESSORS_ONLN);//current online process
int i;
int rc;



//MIN_RESOLVER_THREADS

    if(num_res < 2){

        num_res=2;

    }


//threads decleration

pthread_t requestedthread[argc-2];
pthread_t resolvedthread[num_res];


//mutex initialization

    if(0 != pthread_mutex_init(&mutq,NULL)){

        perror("queue mutex init failed\n");
        return EXIT_FAILURE;

    }


    if(0 != pthread_mutex_init(&mutf,NULL)){

        perror("file mutex init failed\n");
        return EXIT_FAILURE;

    }


    //initialize queue with size

    if(QUEUE_FAILURE == queue_init(&que,50)){

        perror("Error queue init failed\n");
        return EXIT_FAILURE;

    }



    //Check Arguments

    if(argc < MINARGS){

	fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
	fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);

	return EXIT_FAILURE;

    }

     // Open Output File

    outputfp = fopen(argv[(argc-1)], "w");

    if(!outputfp){

        perror("Error Opening Output File\n");
        return EXIT_FAILURE;

    }


    //for resolver threads
    for( i=0; i<num_res; i++){

        rc = pthread_create(&(resolvedthread[i]),NULL,resolverfunction, NULL);

        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    //for requested threads
    for( i=1; i<(argc-1); i++){

        rc = pthread_create(&(requestedthread[i-1]),NULL,requestfunction, argv[i]);

        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }


    //join for both requested and resolves threads
    for(i=0; i<(argc-2); i++){

        if(0 !=  pthread_join(requestedthread[i],NULL)){

        perror("Error pthread_join - requested Thread\n");
        return EXIT_FAILURE;

        }
    }


    threadspresent_flag=0;//set flag to o after termination of requested threads


    for(i=0; i<num_res; i++){

        if(0 !=   pthread_join(resolvedthread[i],NULL)){

        perror("Error pthread_join - resolved Thread\n");
        return EXIT_FAILURE;

        }

    }



    //destroying mutex after DNS resolutions


    pthread_mutex_destroy(&mutq);
    pthread_mutex_destroy(&mutf);


    fclose(outputfp);//close output file

    queue_cleanup(&que);//cleanup queue


    return EXIT_SUCCESS;

}

/********************************************************************************************
*Function: defination which accepts domain name threads from input file and push
*          them in to queue
*input: file containing domain names
*return: void
*********************************************************************************************/

void* requestfunction(void* host_file){


char hostname[SBUFSIZE];
FILE* inputfp = NULL;
char errorstr[SBUFSIZE];
char* name;// to store converted void* to char
int flag_pushed=1;//flag to record status of queue_push operation
char ip[SBUFSIZE];


    strcpy(ip,host_file);//converting void* host_file to char


    /* Open Input File */
	inputfp = fopen(host_file, "r");


	if(!inputfp){
	    sprintf(errorstr, "Error Opening Input File:%s", ip );
	    perror(errorstr);
	    //exit(-1);
	}

    else{
    //scan domin name and push into queue
	 while(fscanf(inputfp, INPUTFS, hostname) > 0){
            flag_pushed=0;

            while(flag_pushed==0){

            pthread_mutex_lock(&mutq);

            if(queue_is_full(&que)){

                pthread_mutex_unlock(&mutq);
                usleep(rand()%100);//after excedding queue size it sleeps

            }
            else{

                name = malloc(SBUFSIZE);
                strncpy(name,hostname, SBUFSIZE);


                queue_push(&que,name);
                pthread_mutex_unlock(&mutq);
                flag_pushed=1;

                }
            }
        }

    fclose(inputfp);
    }

  //  fclose(inputfp);
    return 0;

}

/********************************************************************************************
*Function: defination which pops domanin name from queue and resolve it to respective IP
*          address and print domain name and its IP address to output file
*
*input: None
*return: void
*********************************************************************************************/

void* resolverfunction(){

char* hostname;
char firstipstr[INET6_ADDRSTRLEN];


        while(threadspresent_flag == 1 || !queue_is_empty(&que)){//checks for threads and queue is not empty

            pthread_mutex_lock(&mutq);

            if(!queue_is_empty(&que)){

                hostname=queue_pop(&que);//pop domain name

            if(hostname != NULL){

                pthread_mutex_unlock(&mutq);

                //resolve domaion name to IP address


            if(dnslookup(hostname, firstipstr, sizeof(firstipstr))== UTIL_FAILURE){

                fprintf(stderr, "dnslookup error: %s\n", hostname);
                strncpy(firstipstr, "", sizeof(firstipstr));
            }

                //print to outpot file;

                pthread_mutex_lock(&mutf);

                fprintf(outputfp, "%s,%s\n", hostname, firstipstr);

                pthread_mutex_unlock(&mutf);

            }
                free(hostname);// removing hostname after resolving to IP
        }

        else{

           pthread_mutex_unlock(&mutq);
        }
    }

    return NULL;
}
