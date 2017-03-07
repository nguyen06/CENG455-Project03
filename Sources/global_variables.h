/*
 * global_variables.h
 *
 *  Created on: Feb 6, 2017
 *      Author: nguyen06
 */
#include <mqx.h>
#include <message.h>
#include <mutex.h>

#ifndef SOURCES_GLOBAL_VARIABLES_H_
#define SOURCES_GLOBAL_VARIABLES_H_

#define NUM_USER_TASKS 4

//Task IDs
#define DD_TASK 5
#define CREATOR_TASK 6

//Queue IDs
#define DD_QUEUE 7
#define CREATOR_QUEUE 8
#define USER1_QUEUE 9
#define PUTLINE_QUEUE 10
#define USER2_QUEUE 11
#define USER3_QUEUE 12
#define USER4_QUEUE 13

_queue_id handler_qid;
_queue_id putline_qid;

char output_buf[200];
char putline_buf[200];
char getline_buf[200];
int counter;// = 11;
char Cr = 0x0d;
int j;




//message structure
typedef struct{
	MESSAGE_HEADER_STRUCT HEADER;
	unsigned char DATA[200];
}MESSAGE, *MSG_PTR;


//task structure
typedef struct{
	uint_32 tid;
	uint_32 deadline;
	uint_32 creation_time;
	struct task_list *next_cell;
	struct task_list *previous_cell;
}task_list, task_list;

MSG_PTR dd_msg_ptr;


#endif /* SOURCES_GLOBAL_VARIABLES_H_ */
