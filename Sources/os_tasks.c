/* ###################################################################
**     Filename    : os_tasks.c
**     Project     : Project03
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-06, 16:35, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         dd_task - void dd_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include "global_variables.h"
#include "access_functions.h"
#include <inttypes.h>
#include <stdbool.h>
//#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : dd_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void dd_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */

	/*create active list and overdue list*/
	task_list* dd_active_list_head = NULL;
	task_list* dd_active_list_tail = NULL;
	task_list* dd_active_list_current = NULL;

	task_list* dd_overdue_list_head = NULL;
//	task_list* dd_overdue_list_tail = NULL;
	task_list* dd_overdue_list_current = NULL;


	/*Create queue for dd_scheduler*/
	dd_qid = _msgq_open(DD_QUEUE, 0);
	if(dd_qid == 0){
		printf("\nCould not open the handler isr queue\n");
		_task_block();
	}

	delay(500);
	printf("im in dd_qid>>>%d\n", dd_qid);

	/*Create queue for dd_tcreate*/
	tcreate_qid = _msgq_open(TCREAT_QUEUE, 0);
	if(tcreate_qid == 0){
		printf("\nCould not open the tcreate queue\n");
		_task_block();
	}

	/*Create queue for dd_delete*/
	delete_qid = _msgq_open(DELETE_QUEUE, 0);
	if(delete_qid == 0){
		printf("\nCould not open the tcreate queue\n");
		_task_block();
	}
	printf("d_qid>>>%d", delete_qid);

	// create message pool
	msg_pool = _msgpool_create(sizeof(MESSAGE),NUM_USER_TASKS, 0, 0);

	delay(500);
	if (msg_pool == MSGPOOL_NULL_POOL_ID) {
		printf("\nCount not create a message pool\n");
		_task_block();
	}

	_task_id running_task = 0;

	delay(500);
	puts("\nim herer\n");
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */
	  dd_msg_ptr = _msgq_receive(dd_qid, 0);

	  if(dd_msg_ptr != NULL){
		  if(dd_msg_ptr->HEADER.SOURCE_QID == tcreate_qid){;

			  insert(&dd_active_list_head, &dd_active_list_tail, dd_msg_ptr->tid, dd_msg_ptr->deadline, dd_msg_ptr->creation_time);
//			  displayForward(dd_active_list_head);
		  }

		  if(dd_msg_ptr->HEADER.SOURCE_QID == delete_qid){
			  _task_id tid = dd_msg_ptr->tid;
			  uint32_t c_time = dd_msg_ptr->creation_time;
			  delete(&dd_active_list_head, &dd_active_list_tail, tid, c_time);
			  displayForward(dd_active_list_head);
		  }
		  free(dd_msg_ptr);
	  }

	  /*test for linked insert and delete*/
	  if(tt == 5){
//		  delete(&dd_active_list_head, &dd_active_list_tail, 65541, 9);
//		  puts("im tt");
		  displayForward(dd_active_list_head);
	  }
	  OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : Task2_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Task2_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
//	puts("\nim t2, im created");
	is_running = 1;
	/*Do something*/

	_time_get_elapsed(&start_t);
	uint32_t stime = start_t.SECONDS;
	int prev_sec = -1;
	int remaining_execution_t = task_init_data+1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-stime < task_init_data) {
#endif
    /* Write your code here ... */

	_time_get_elapsed(&start_t);

	if(start_t.SECONDS != prev_sec){
		prev_sec = start_t.SECONDS;
		remaining_execution_t--;
		printf("seconds>>>%d\n", start_t.SECONDS);
		printf("remaining_execution_t>>>%d\n", remaining_execution_t);
	}
    OSA_TimeDelay(10);                 /* Example code (for task release) */


#ifdef PEX_USE_RTOS
  }
#endif
//  dd_delete(_task_get_id(), stime);
  puts("task2 ended\n");
  is_running = 0;
}


/*
** ===================================================================
**     Callback    : Creator_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Creator_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */

	delay(500);
	int ptask_num = 0;
	int aptask_num = 0;
	int prev_sec = -1;
//	dd_tcreate(TASK2_TASK,10,5);
	_time_get_elapsed(&start_t);

	int execute_t = 0;
	int arrive_t = 0;
	int time_interval;

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */

	  /*Create periodic tasks*/
	  if(start_t.SECONDS == 6 && start_t.SECONDS != prev_sec){

	  }

	  if(ptask_num<5){
		 _time_get_elapsed(&start_t);
		  OSA_TimeDelay(10);
		  /*Avoid create several tasks at a same time*/
		  if(start_t.SECONDS % 5 == 0 && start_t.SECONDS != prev_sec){
			  prev_sec = start_t.SECONDS;
			  printf("pt count: %d\n", ptask_num);
//			  dd_tcreate(TASK2_TASK,10,3);
			  if(ptask_num == 0)
			  	  dd_tcreate(TASK2_TASK,5,3);
			  if(ptask_num == 1)
				  dd_tcreate(TASK2_TASK,7,3);
			  if(ptask_num == 2)
				  dd_tcreate(TASK2_TASK,2,3);
			  if(ptask_num == 3)
				  dd_tcreate(TASK2_TASK,9,3);
			  if(ptask_num == 4)
				  dd_tcreate(TASK2_TASK,6,3);
			  ptask_num++;
		  }
	  }else{
		  break;
	  }

	  /*Create aperiodic tasks*/
//	  if(aptask_num<5){
//		  if(is_running == 0){
//			  /*random number seed*/
//			  srand(time(0));
//			  is_running = 1;
//			  /*get current time*/
//			 _time_get_elapsed(&start_t);
//
//
//			  time_interval = generateRandom(2, 8);
//
//			  /*Avoid create several tasks at a same time*/
//			  if(start_t.SECONDS != prev_sec){
//
//				  /*set the arrival time for next task */
//				  arrive_t = start_t.SECONDS+ time_interval;
//				  printf("\naperiodic task %d\n", aptask_num);
//				  printf("arrival_t: %d\n", arrive_t);
//				  execute_t = generateRandom(2, 8);
//				  printf("time_interval: %d\n", time_interval);
//				  printf("execute_t: %d\n\n", execute_t);
//				  while(start_t.SECONDS < arrive_t){
//					  _time_get_elapsed(&start_t);
//					  OSA_TimeDelay(10);
//				  }
//				  prev_sec = start_t.SECONDS;
//				  dd_tcreate(TASK2_TASK,10,execute_t);
//				  aptask_num++;
//			  }
//		  }else{
//			  OSA_TimeDelay(10);
//		  }
//	  }else{
//		  break;
//	  }

                 /* Example code (for task release) */

    
#ifdef PEX_USE_RTOS   
  }
#endif
  tt = ptask_num;
  printf("%d\n", tt);
  puts("\nGenerator finished\n");
}

/*
** ===================================================================
**     Callback    : Task3_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Task3_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
	is_running = 1;
	/*Do something*/

	_time_get_elapsed(&start_t);
	uint32_t stime = start_t.SECONDS;
	int prev_sec = -1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-stime < task_init_data) {
#endif
    /* Write your code here ... */

	_time_get_elapsed(&start_t);

	if(start_t.SECONDS != prev_sec){
		prev_sec = start_t.SECONDS;
		printf("seconds>>>%d\n", start_t.SECONDS);
	}
    OSA_TimeDelay(10);                 /* Example code (for task release) */
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
