/*
 * write template functions that are guaranteed to read and write the
 * number of bytes desired
 */

#ifndef fancyRW_h
#define fancyRW_h
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include<iostream>
using namespace std;

template<typename T>
int READ(int fd, T* obj_ptr, int count)
{
  char* addr=(char*)obj_ptr;
  repeat:
    int ret = read(fd,addr,count);
    if(ret==-1 && errno == EINTR){
        goto repeat;
      }else if(ret==-1){
        perror("Failed to Read");
        return -1;
      }

  return ret;
  //loop. Read repeatedly until count bytes are read in
}


template<typename T>
int WRITE(int fd, T* obj_ptr, int count)
{
  char* addr=(char*)obj_ptr;
  int total=0;

  while(count>0){
      int written = write(fd,addr,count);
        if(written==-1 && errno == EINTR){
          continue;
        }else if(written==-1){
          perror("Failed to write");
          return (total == 0) ? -1 : total;
          }
    count -= written;
    addr += written;
    total +=written;
   //loop. Write repeatedly until count bytes are written out
}
return total;
}
#endif


/*
//Example of a method for testing your functions shown here:
//opened/connected/etc to a socket with fd=7
//
int count=write(7, "Todd Gibson", 11);
cout << count << endl;//will display a number between 1 and 11
int count=write(7, "d Gibson", 8);
//
//How to test your READ template. Read will be: READ(7, ptr, 11);
write(7, "To", 2);
write(7, "DD ", 3);
write(7, "Gibso", 5);
write(7, "n", 1);
//
//
*/
