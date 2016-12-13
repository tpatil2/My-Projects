/**
*Name: Tejas  Patil
*Csu Id : 006949762
*Assignment No 5  - DNS
*/

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


pthread_mutex_t mutq;
pthread_mutex_t mutf;
queue que;
FILE* outputfp = NULL;
int threadspresent=1;

int  main(int argc, char* argv[]){

int num_res = sysconf(_SC_NPROCESSORS_ONLN);
int num_req = argc-2;
int i;



if (num_res < 2)
    { num_res = 2; }


pthread_t requestedthread[num_req];
pthread_t resolvedthread[num_res];


pthread_mutex_init(&mutq,NULL);
pthread_mutex_init(&mutf,NULL);
queue_init(&que,50);


 /* Check Arguments */
    if(argc < MINARGS){
	fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
	fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
	return EXIT_FAILURE;
    }

     /* Open Output File */
    outputfp = fopen(argv[(argc-1)], "w");
    if(!outputfp){
	perror("Error Opening Output File");
	return EXIT_FAILURE;
    }


    //for resolver threads
    for( i=0; i<num_res; i++){

    pthread_create(&(resolvedthread[i]),NULL,resolverfunction, NULL);

    }
    //for requested threads
    for( i=1; i<(argc-1); i++){

    pthread_create(&(requestedthread[i-1]),NULL,requestfunction, argv[i]);

    }

    //join for both
    for(i=0; i<num_req; i++){
    pthread_join(requestedthread[i],NULL);
    }

    threadspresent=0;

    for(i=0; i<num_res; i++){
    pthread_join(resolvedthread[i],NULL);
    }

    pthread_mutex_destroy(&mutq);
    pthread_mutex_destroy(&mutf);

    fclose(outputfp);
    queue_cleanup(&que);

    return EXIT_SUCCESS;

}


void* requestfunction(void* host_file){

char hostname[SBUFSIZE];
FILE* inputfp = NULL;
char errorstr[SBUFSIZE];
char* name;
int que_pushed=1;

    /* Open Input File */
	inputfp = fopen(host_file, "r");
	if(!inputfp){
	    sprintf(errorstr, "Error Tejas Opening Input File");
	    perror(errorstr);
	}


	 while(fscanf(inputfp, INPUTFS, hostname) > 0){
            que_pushed=0;
            while(que_pushed==0){
            pthread_mutex_lock(&mutq);
            if(queue_is_full(&que)){
                pthread_mutex_unlock(&mutq);
                usleep(rand()%100);
            }
            else{
                name = malloc(SBUFSIZE);
                strncpy(name,hostname, SBUFSIZE);
                queue_push(&que,name);
                pthread_mutex_unlock(&mutq);
                que_pushed=1;
                }
            }
        }

    fclose(inputfp);
    return NULL;

}


void* resolverfunction(){

char* hostname;
char firstipstr[INET6_ADDRSTRLEN];


    while(threadspresent == 1 || !queue_is_empty(&que)){
        pthread_mutex_lock(&mutq);
        if(!queue_is_empty(&que)){

            hostname=queue_pop(&que);

            if(hostname != NULL){

            pthread_mutex_unlock(&mutq);
            if(dnslookup(hostname, firstipstr, sizeof(firstipstr))== UTIL_FAILURE){
            fprintf(stderr, "dnslookup error: %s\n", hostname);
            strncpy(firstipstr, "", sizeof(firstipstr));
            }
                //print to outpot file;
                pthread_mutex_lock(&mutf);
                fprintf(outputfp, "%s,%s\n", hostname, firstipstr);
                pthread_mutex_unlock(&mutf);
            }
                free(hostname);
        }
        else{
           pthread_mutex_unlock(&mutq);
        }
    }
    return NULL;
}
