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

#define NUM_USER_TASKS 10

//Task IDs
#define DD_TASK 5
#define CREATOR_TASK 6

//Queue IDs
#define DD_QUEUE 7
#define CREATOR_QUEUE 8
#define TCREAT_QUEUE 9
#define DELETE_QUEUE 10
#define USER2_QUEUE 11
#define USER3_QUEUE 12
#define USER4_QUEUE 13

_queue_id dd_qid;
_queue_id tcreate_qid;
_queue_id delete_qid;

char output_buf[200];
char putline_buf[200];
char getline_buf[200];
int counter;// = 11;
char Cr = 0x0d;
//int srand_;




//message structure
typedef struct{
	MESSAGE_HEADER_STRUCT HEADER;
	_task_id tid;
	int deadline;
	int creation_time;
}MESSAGE, *MSG_PTR;


//task structure
typedef struct t_list{
	uint32_t tid;
	uint32_t deadline;
	uint32_t creation_time;
	struct t_list *next_cell;
	struct t_list *previous_cell;
}task_list;

MSG_PTR dd_msg_ptr;
TIME_STRUCT start_t;

int is_running = 0;
int tt = 0;
bool interrupt_occurs = false;

/*arrival time of aperiodic task*/
//int arrive_t = 0;

_pool_id msg_pool;



#endif /* SOURCES_GLOBAL_VARIABLES_H_ */


