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
**     Callback    : ddScheduler
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void ddScheduler(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */

	/*create active list and overdue list*/
	task_list* dd_active_list_head = NULL;
	task_list* dd_active_list_tail = NULL;
	task_list* dd_active_list_current = NULL;

	task_list* dd_overdue_list_head = NULL;
//	task_list* dd_overdue_list_tail = NULL;
	task_list* dd_overdue_list_current = NULL;

	_mqx_uint old_priority;
	_mqx_uint new_priority = 5;
	_mqx_uint check_priority;
	// change priority of the running task to 16
	if(_task_get_priority(_task_get_id(), &old_priority) != MQX_OK)
		printf("\nCouldnt get dd_SCHEDULER task Priority\n");
	if(_task_set_priority(_task_get_id(), new_priority, &old_priority) != MQX_OK)
		printf("\nCouldnt SET dd_SCHEDULER task Priority\n");
	if(_task_get_priority(_task_get_id(), &check_priority) != MQX_OK)
		printf("\nCouldnt get dd_SCHEDULER task Priority\n");
	printf("INISDE DD-SCHEDULER TASK--> old pri: %d,  new pri: %d\n", old_priority, check_priority);

　
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

　
	// create message pool
	msg_pool = _msgpool_create(sizeof(MESSAGE),NUM_USER_TASKS, 0, 0);

	delay(500);
	if (msg_pool == MSGPOOL_NULL_POOL_ID) {
		printf("\nCount not create a message pool\n");
		_task_block();
	}

  
#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */

	  // receive message from task create
	  dd_msg_ptr = _msgq_receive(dd_qid, 0);
	  wait(200);
	  printf("\ndead line %d\n\n",dd_msg_ptr->deadline);
	  if(dd_msg_ptr != NULL){

		  if(dd_msg_ptr->HEADER.SOURCE_QID == tcreate_qid){

　
			  //put the link list, and sort it
			  insert(&dd_active_list_head, &dd_active_list_tail, dd_msg_ptr->tid,
					  dd_msg_ptr->deadline, dd_msg_ptr->creation_time);

			  // display the active tasks
			  puts("JUST ADDED TO LIST");
			  displayForward(dd_active_list_head);

			  // send msg back to ddt_create
			  dd_msg_ptr->HEADER.SOURCE_QID = dd_qid;
			  dd_msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, TCREAT_QUEUE);
			  wait(200);
			  printf("INSIDE DD-SCEHDULER -> value of targer is %d\n",dd_msg_ptr->HEADER.TARGET_QID);

//			  delay(100);
//			  /*if higher priority task(lower deadline) arriving, preept*/
//			  printf("running_task:%d, dd_active_list_head->tid:%d\n", running_task,dd_active_list_head->tid);

　
			  _msgq_send(dd_msg_ptr);
		  }

		  // Receive from delete function and delete task from list
		  else if(dd_msg_ptr->HEADER.SOURCE_QID == delete_qid){
			  _task_id tid = dd_msg_ptr->tid;
			  puts("MESSAGE FROM DELETE FUNCTION");
			  uint32_t c_time = dd_msg_ptr->creation_time;

//			  delay(100);
//			  puts("\n deleting from linked list");
			  delete(&dd_active_list_head, &dd_active_list_tail, tid, c_time);

//			  delay(100);
//			  printf("list after deletion, tid is===%d\n", tid);

			  displayForward(dd_active_list_head);
			  _msg_free(dd_msg_ptr);
		  }

		  if(running_task != dd_active_list_head->tid){
			delay(100);
			printf("\nINSIDE SHCEDULE TASK. RUNNING TASK IS %d\n", running_task);
			schedule_task(dd_active_list_head->tid);
		  }

　
	  }

　
	  // start scheduling tasks
//	  if(dd_msg_ptr->HEADER.SOURCE_QID == tcreate_qid)
//		  schedule_task(dd_active_list_head->tid, dd_msg_ptr->priority);

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
	/*Do something*/
	_time_get_elapsed(&start_t);
	uint32_t start_time = start_t.SECONDS;
	int prev_sec = -1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-start_time < 5) {
#endif
    /* Write your code here ... */

	_time_get_elapsed(&start_t);

	if(start_t.SECONDS != prev_sec){
		prev_sec = start_t.SECONDS;
		printf("Task6 seconds>>>%d\n", start_t.SECONDS);
	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */

　
#ifdef PEX_USE_RTOS
  }
#endif
  dd_delete(_task_get_id(), start_time);
  puts("task6 ended\n");
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

	_mqx_uint old_priority;
	_mqx_uint new_priority = 3;

	// change priority of the running task to 16
	if(_task_get_priority(_task_get_id(), &old_priority) != MQX_OK)
		printf("\nCouldnt get task_CREATOR task Priority\n");
	if(_task_set_priority(_task_get_id(), new_priority, &old_priority) != MQX_OK)
		printf("\nCouldnt SET task_CREATOR task Priority\n");

	printf("INISDE CREATOR--> old pri: %d,  new pri: %d\n", old_priority, new_priority);

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
		  if(start_t.SECONDS % 6 == 0 && start_t.SECONDS != prev_sec){

　
			  prev_sec = start_t.SECONDS;
			  printf("\nFROM TASK CREATOR -> Generated Task%d\n", ptask_num+1);
			  if(ptask_num == 0)
			  	  dd_tcreate(TASK1_TASK,10);
			  else if(ptask_num == 1)
				  dd_tcreate(TASK2_TASK,15);
			  else if(ptask_num == 2)
				  dd_tcreate(TASK3_TASK,25);
			  else if(ptask_num == 3)
				  dd_tcreate(TASK4_TASK,20);
			  else if(ptask_num == 4)
				  dd_tcreate(TASK5_TASK,30);
			  else if(ptask_num == 5)
			  	dd_tcreate(TASK6_TASK,36);

			  ptask_num++;
		  }
	  }else{
		  break;
	  }
	  OSA_TimeDelay(10);

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
//  dd_delete(65541,10);
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
	delay(100);
	puts("im idle task>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	is_running = 1;
	/*Do something*/

	_time_get_elapsed(&start_t);
	uint32_t start_time = start_t.SECONDS;
	int prev_sec = -1;

	_mqx_uint old_priority;
	_mqx_uint new_priority = 11;

	// change priority of the running task to 16
	if(_task_get_priority(_task_get_id(), &old_priority) != MQX_OK)
		printf("\nCouldnt get idle task Priority\n");
	if(_task_set_priority(_task_get_id(), new_priority, &old_priority) != MQX_OK)
		printf("\nCouldnt SET idle task Priority\n");

　
	printf("INISDE IDLE TASK--> old pri: %d,  new pri: %d\n", old_priority, new_priority);

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */

	_time_get_elapsed(&start_t);
	delay(100);
//	printf("IDEL IS RUNNING");

	if(start_t.SECONDS != prev_sec){
		prev_sec = start_t.SECONDS;
		printf("IDLE --seconds>>>%d\n", start_t.SECONDS);
	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */
    
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

	/*Do something*/
	_time_get_elapsed(&start_t);
	uint32_t start_time = start_t.SECONDS;
	int prev_sec = -1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-start_time < 5) {
#endif
    /* Write your code here ... */

	_time_get_elapsed(&start_t);

	if(start_t.SECONDS != prev_sec){
		prev_sec = start_t.SECONDS;
		printf("Task5 seconds>>>%d\n", start_t.SECONDS);
	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */

　
#ifdef PEX_USE_RTOS
  }
#endif
//  dd_delete(_task_get_id(), start_time);
  puts("task5 ended\n");
  dd_delete(_task_get_id(), start_time);
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
	/*Do something*/
	_time_get_elapsed(&start_t);
	uint32_t start_time = start_t.SECONDS;
	int prev_sec = -1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-start_time < 5) {
#endif
    /* Write your code here ... */

	_time_get_elapsed(&start_t);

	if(start_t.SECONDS != prev_sec){
		prev_sec = start_t.SECONDS;
		printf("Task4 seconds>>>%d\n", start_t.SECONDS);
	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */

　
#ifdef PEX_USE_RTOS
  }
#endif
//  dd_delete(_task_get_id(), start_time);
  puts("task4 ended\n");
  dd_delete(_task_get_id(), start_time);
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
	/*Do something*/
	_time_get_elapsed(&start_t);
	uint32_t start_time = start_t.SECONDS;
	int prev_sec = -1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-start_time < 10) {
#endif
    /* Write your code here ... */

	_time_get_elapsed(&start_t);

	if(start_t.SECONDS != prev_sec){
		prev_sec = start_t.SECONDS;
		printf("Task3 seconds>>>%d\n", start_t.SECONDS);
	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */

　
#ifdef PEX_USE_RTOS
  }
#endif
//  dd_delete(_task_get_id(), start_time);
  puts("task3 ended\n");
  dd_delete(_task_get_id(), start_time);
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
	/*Do something*/
	_time_get_elapsed(&start_t);
	uint32_t start_time = start_t.SECONDS;
	int prev_sec = -1;

//		puts("im in task2");
//		printf("/nim in task2,   start_t.SECONDS:%d,start_time:%d\n", start_t.SECONDS, start_time);

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-start_time < 5) {
#endif
    /* Write your code here ... */
	  delay(100);

	  _time_get_elapsed(&start_t);
//	  printf("/nim in task2, in while  start_t.SECONDS:%d,start_time:%d\n", start_t.SECONDS, start_time);
	  	if(start_t.SECONDS != prev_sec){
	  		prev_sec = start_t.SECONDS;
	  		delay(100);
	  		printf("TASK2 seconds>>>%d\n", start_t.SECONDS);
	  	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */

　
#ifdef PEX_USE_RTOS
  }
#endif
  delay(100);
  puts("task2 ended>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  dd_delete(_task_get_id(), start_time);
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
	uint32_t start_time = start_t.SECONDS;
	int prev_sec = -1;

#ifdef PEX_USE_RTOS
  while (start_t.SECONDS-start_time < 10) {
#endif
    /* Write your code here ... */
	_time_get_elapsed(&start_t);

	if(start_t.SECONDS != prev_sec){
		prev_sec = start_t.SECONDS;
		printf("Task1 seconds>>>%d\n", start_t.SECONDS);
	}
//    OSA_TimeDelay(10);                 /* Example code (for task release) */

　
#ifdef PEX_USE_RTOS
  }
#endif
  puts("task1 ended\n");
  dd_delete(_task_get_id(), start_time);
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
