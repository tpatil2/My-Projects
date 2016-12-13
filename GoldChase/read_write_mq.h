#ifndef read_write_mq_h
#define read_write_mq_h
#include <iostream>
using namespace std;

void read_Dmessage(int pnum);
//void read_Dmessage2(int pnum);

int write_Dqueue(string st);
//int write_Dqueue2(string st);

int read_Dqueue(int pnum);
//int read_Dqueue2(int pnum);

mqd_t Dreadqueue_fd[5];
//mqd_t Dreadqueue_fd2[5];

string mq_name1="/none";
//string mq_name2="/none";

#endif
