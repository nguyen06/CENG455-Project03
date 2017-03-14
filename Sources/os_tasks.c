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

//	intterrupt_qid = _msgq_open(INTERRUPT_QUEUE, 0);
//	if(intterrupt_qid == 0){
//		printf("\nCould not open the handler isr queue\n");
//		_task_block();
//	}

	delay(500);
	printf("im in dd_qid>>>%d\n", dd_qid);

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

	delay(500);
	puts("\nim herer\n");
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */

	  // receive message from task create
	  dd_msg_ptr = _msgq_receive(dd_qid, 0);

	  if(dd_msg_ptr != NULL){
		  if(dd_msg_ptr->HEADER.SOURCE_QID == tcreate_qid){;
			  insert(&dd_active_list_head, &dd_active_list_tail, dd_msg_ptr->tid,
					  dd_msg_ptr->deadline, dd_msg_ptr->creation_time);

			  // display the active tasks
			  displayForward(dd_active_list_head);

			  // send msg back to ddt_create
			  dd_msg_ptr->HEADER.SOURCE_QID = dd_qid;
			  dd_msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, TCREAT_QUEUE);


			  _msgq_send(dd_msg_ptr);

			  /*if higher priority task(lower deadline) arriving, preept*/
//			  if(running_task != dd_active_list_head){
//				  interrupt_occurs = true;
//			  }
		  }

		  // Receive from delete function and delete task from list
		  if(dd_msg_ptr->HEADER.SOURCE_QID == delete_qid){
			  _task_id tid = dd_msg_ptr->tid;
			  puts(" im deleting");
			  uint32_t c_time = dd_msg_ptr->creation_time;
			  delete(&dd_active_list_head, &dd_active_list_tail, tid, c_time);
			  displayForward(dd_active_list_head);
		  }
		  _msg_free(dd_msg_ptr);
	  }


	  // start scheduling tasks
	  if(dd_msg_ptr->HEADER.SOURCE_QID == tcreate_qid)
		  schedule_task(dd_msg_ptr->tid);

	  /*test for linked insert and delete*/
//	  printf("tt>>>>>> %d\n", tt);
//	  if(tt == 5){
////		  delete(&dd_active_list_head, &dd_active_list_tail, 65541, 9);
//		  puts("im tt");
//		  displayForward(dd_active_list_head);
//	  }
	 // OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : Task6_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Task6_task(os_task_param_t task_init_data)
{
	/*receive msg for interrupt*/
	is_running = 1;
	/*Do something*/
	running_task = _task_get_id();
	_time_get_elapsed(&start_t);
	uint32_t stime = start_t.SECONDS;
	int prev_sec = -1;
	int remaining_execution_t = task_init_data+1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-stime < 3) {
#endif
    /* Write your code here ... */

//	_time_get_elapsed(&start_t);
//
//	if(start_t.SECONDS != prev_sec){
//		prev_sec = start_t.SECONDS;
//		remaining_execution_t--;
//		printf("seconds>>>%d\n", start_t.SECONDS);
//		printf("remaining_execution_t>>>%d\n", remaining_execution_t);
//	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */


#ifdef PEX_USE_RTOS
  }
#endif
//  dd_delete(_task_get_id(), stime);
  puts("task6 ended\n");
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


//	  if(start_t.SECONDS == 6 && start_t.SECONDS != prev_sec){
//
//	  }
	  /*Create periodic tasks*/
	  if(ptask_num<6){
		 _time_get_elapsed(&start_t);
		  /*Avoid create several tasks at a same time*/
		  if(start_t.SECONDS % 1 == 0 && start_t.SECONDS != prev_sec){
			  prev_sec = start_t.SECONDS;
			  printf("\ntask%d\n", ptask_num);
//			  dd_tcreate(TASK2_TASK,10,3);
			  if(ptask_num == 0)
			  	  dd_tcreate(TASK1_TASK,5);
			  if(ptask_num == 1)
				  dd_tcreate(TASK2_TASK,7);
			  if(ptask_num == 2)
				  dd_tcreate(TASK3_TASK,10);
			  if(ptask_num == 3)
				  dd_tcreate(TASK4_TASK,13);
			  if(ptask_num == 4)
				  dd_tcreate(TASK5_TASK,9);
			  if(ptask_num == 5)
			  	dd_tcreate(TASK6_TASK,11);
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
  printf("tt in creator>>>>%d\n", tt);
  dd_delete(65541,10);
  puts("\nGenerator finished\n");
}

/*
** ===================================================================
**     Callback    : Idle_task1
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Idle_task1(os_task_param_t task_init_data)
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

/*
** ===================================================================
**     Callback    : Task5_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Task5_task(os_task_param_t task_init_data)
{
	/*receive msg for interrupt*/
	is_running = 1;
	/*Do something*/
	running_task = _task_get_id();
	_time_get_elapsed(&start_t);
	uint32_t stime = start_t.SECONDS;
	int prev_sec = -1;
	int remaining_execution_t = task_init_data+1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-stime < 3) {
#endif
    /* Write your code here ... */

//	_time_get_elapsed(&start_t);
//
//	if(start_t.SECONDS != prev_sec){
//		prev_sec = start_t.SECONDS;
//		remaining_execution_t--;
//		printf("seconds>>>%d\n", start_t.SECONDS);
//		printf("remaining_execution_t>>>%d\n", remaining_execution_t);
//	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */


#ifdef PEX_USE_RTOS
  }
#endif
//  dd_delete(_task_get_id(), stime);
  puts("task5 ended\n");
  is_running = 0;
}

/*
** ===================================================================
**     Callback    : Task4_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Task4_task(os_task_param_t task_init_data)
{
	/*receive msg for interrupt*/
	is_running = 1;
	/*Do something*/
	running_task = _task_get_id();
	_time_get_elapsed(&start_t);
	uint32_t stime = start_t.SECONDS;
	int prev_sec = -1;
	int remaining_execution_t = task_init_data+1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-stime < 3) {
#endif
    /* Write your code here ... */

//	_time_get_elapsed(&start_t);
//
//	if(start_t.SECONDS != prev_sec){
//		prev_sec = start_t.SECONDS;
//		remaining_execution_t--;
//		printf("seconds>>>%d\n", start_t.SECONDS);
//		printf("remaining_execution_t>>>%d\n", remaining_execution_t);
//	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */


#ifdef PEX_USE_RTOS
  }
#endif
//  dd_delete(_task_get_id(), stime);
  puts("task4 ended\n");
  is_running = 0;
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
	/*receive msg for interrupt*/
	is_running = 1;
	/*Do something*/
	running_task = _task_get_id();
	_time_get_elapsed(&start_t);
	uint32_t stime = start_t.SECONDS;
	int prev_sec = -1;
	int remaining_execution_t = task_init_data+1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-stime < 3) {
#endif
    /* Write your code here ... */

//	_time_get_elapsed(&start_t);
//
//	if(start_t.SECONDS != prev_sec){
//		prev_sec = start_t.SECONDS;
//		remaining_execution_t--;
//		printf("seconds>>>%d\n", start_t.SECONDS);
//		printf("remaining_execution_t>>>%d\n", remaining_execution_t);
//	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */


#ifdef PEX_USE_RTOS
  }
#endif
//  dd_delete(_task_get_id(), stime);
  puts("task3 ended\n");
  is_running = 0;
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
	/*receive msg for interrupt*/
	is_running = 1;
	/*Do something*/
	running_task = _task_get_id();
	_time_get_elapsed(&start_t);
	uint32_t stime = start_t.SECONDS;
	int prev_sec = -1;
	int remaining_execution_t = task_init_data+1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-stime < 3) {
#endif
    /* Write your code here ... */

//	_time_get_elapsed(&start_t);
//
//	if(start_t.SECONDS != prev_sec){
//		prev_sec = start_t.SECONDS;
//		remaining_execution_t--;
//		printf("seconds>>>%d\n", start_t.SECONDS);
//		printf("remaining_execution_t>>>%d\n", remaining_execution_t);
//	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */


#ifdef PEX_USE_RTOS
  }
#endif
  puts("task2 ended\n");
  is_running = 0;
}

/*
** ===================================================================
**     Callback    : Task1_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void Task1_task(os_task_param_t task_init_data)
{
	/*receive msg for interrupt*/
	is_running = 1;
	/*Do something*/
	running_task = _task_get_id();
	_time_get_elapsed(&start_t);
	uint32_t stime = start_t.SECONDS;
	int prev_sec = -1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-stime < 3) {
#endif
    /* Write your code here ... */
	_time_get_elapsed(&start_t);

	if(start_t.SECONDS != prev_sec){

		puts("in if>>>>>>>>>>>>>>>>>>>>>>>>>>");
		prev_sec = start_t.SECONDS;
		printf("seconds>>>%d\n", start_t.SECONDS);
	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */


#ifdef PEX_USE_RTOS
  }
#endif
  puts("task1 ended\n");
  is_running = 0;
}


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
