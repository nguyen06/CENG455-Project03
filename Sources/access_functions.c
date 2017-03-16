#include "access_functions.h"

　
_task_id dd_tcreate(uint32_t template_index, uint32_t deadline){
	_mqx_uint old_priority_ptr;
	_mqx_uint priority_ptr = 16;

	/*Create queue for dd_tcreate*/
	tcreate_qid = _msgq_open(TCREAT_QUEUE, 0);

	if(tcreate_qid == 0){
		printf("\nCould not open the tcreate queue\n");
		_task_block();
	}

	/*create tasks*/
	// auto blobk task at creation
	_task_id new_tid = _task_create_blocked(0, template_index,0);

	MSG_PTR msg_ptr = (MSG_PTR)_msg_alloc(msg_pool);
	_time_get_elapsed(&start_t);

	// change priority of the task to 16
	_task_get_priority(new_tid, &old_priority_ptr);
	_task_set_priority(new_tid, priority_ptr, &old_priority_ptr);
	delay(500);
	printf("\nDDCREATE -> old pri: %d,  new pri: %d\n", old_priority_ptr, priority_ptr);

　
　
//	printf("tid %d id created\n",new_tid);

	msg_ptr->HEADER.SOURCE_QID = tcreate_qid;
	msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, DD_QUEUE);
	msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + sizeof(int)*4;
	msg_ptr->tid = new_tid;
//	msg_ptr->execution_time = execution_time;
	msg_ptr->deadline = deadline;
	msg_ptr->creation_time = start_t.SECONDS;
	msg_ptr->priority = priority_ptr;

	delay(500);
	printf("INSIDE DD-tCREATE -> RECEIVING FROM SCHEDULER is %d\n",msg_ptr->HEADER.SOURCE_QID);

　
	delay(500);
	puts("from dd_tcreate, before sending msg");
	wait(200);
	printf("INSIDE DD-tCREATE -> TARGET QUEUE is %d\n",msg_ptr->HEADER.TARGET_QID);
	_msgq_send(msg_ptr);

　
//	puts("form dd_tcreate, before receiving msg");
	msg_ptr = _msgq_receive(tcreate_qid, 0);

	delay(100);
	puts("from dd_tcreate, after receiving msg");
	//
	if(msg_ptr == NULL){
		delay(500);
		puts("\nin dd_create, msg_ptr = NULL>>>>>>>>>>>>>>>>>>>>>>\n");
	}

	if(msg_ptr->tid == new_tid){
		delay(500);
		puts("\nTask in scheduled list\n");
		_msgq_close(tcreate_qid);
		_msg_free(msg_ptr);
	}else{
		printf("tid>>>%d\n", msg_ptr->tid);
	}

　
	return new_tid;
}

　
uint32_t dd_delete(_task_id task_id, uint32_t creation_time){
	MSG_PTR msg_ptr = (MSG_PTR)_msg_alloc(msg_pool);
	is_running = 0;
	running_task = 0;
	/*Create queue for delete_qid*/
	delete_qid = _msgq_open(DELETE_QUEUE, 0);
	if(delete_qid == 0){
		printf("\nCould not open the delete_qid queue\n");
		_task_block();
	}

//	puts("im deleting>>>");
	printf(" in in deleting and my creation time is: %d\n", creation_time);
	msg_ptr->HEADER.SOURCE_QID = delete_qid;
	msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, DD_QUEUE);
	msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + sizeof(int)*4;
	msg_ptr->tid = task_id;
	msg_ptr->deadline = 0;
	msg_ptr->creation_time = creation_time;

	/*send msg to dd_scheduler with tid and creation time*/
	_msgq_send(msg_ptr);

//	_msg_free(msg_ptr);
	return task_id;
}

uint32_t dd_return_active_list(task_list **list){
	return *list;
}

uint32_t dd_return_overdue_list(task_list **list){
	return *list;
}

void dd_init(){
	return;

}

int generateRandom(int l, int u){
    int result = (rand() % (u - l)) + l;
//    printf("result>>>%d\n",result);
    return result;
}

　
void delay(int val){
	for ( int c = 1 ; c <= val ; c++ )
		   for (int d = 1 ; d <= val ; d++ )
		   {}
}

　
/*linked list functions*/

int length(task_list **head){
	   int length = 0;
	   task_list *current;

	   for(current = *head; current != NULL; current = current->next_cell){
	      length++;
	   }

	   return length;
}

void displayForward(task_list* head) {
   //start from the beginning

   task_list *ptr = head;

   //navigate till the end of the list
   delay(100);
   printf("\n[");

   while(ptr != NULL) {
	   delay(100);
      printf("(tid:%d, c:%d, d:%d) ",ptr->tid,ptr->creation_time, ptr->deadline);
      ptr = ptr->next_cell;
   }
   delay(100);
   printf("]\n\n");
}

　
//insert link at the first location
void insert(task_list** head, task_list** last, uint32_t tid, uint32_t deadline, uint32_t creation_time) {
   //create a link
   task_list *link = (task_list*) malloc(sizeof(task_list));
   link->tid = tid;
   link->deadline = deadline;
   link->creation_time = creation_time;
//   link->execution_time = execution_time;
   link->next_cell = NULL;
   link->previous_cell = NULL;

   task_list* current = *head;

   /*if list is empty*/
   if(*head == NULL){
	   *last = link;
	   *head = link;
	   return;
   }

   int i = 0;

   while(current->deadline < deadline){
	   /*if it is last node*/
	   if(current->next_cell == NULL){
		   break;
	   }else{
		   current = current->next_cell;
	   }
   }

   if(current == *last){
	   if((*last)->deadline < deadline){
		   link->next_cell = NULL;
		   *last = link;
		   link->previous_cell = current;
		   current->next_cell = link;
		   return;
	   }else{
		   link->next_cell = *last;
		   link->previous_cell = (*last)->previous_cell;
		   (*last)->previous_cell->next_cell = link;
		   (*last)->previous_cell = link;
		   return;
	   }

   }else{
	   link->next_cell = current;
	   if(current != *head){
		   link->previous_cell = current->previous_cell;
		   current->previous_cell->next_cell = link;
	   }
	   current->previous_cell= link;
	   if(current == *head){
		   *head = link;
	   }
	   return;
   }

　
}

//delete according to tid
void delete(task_list** head, task_list** last, uint32_t tid, uint32_t creation_time) {

　
	task_list* current = *head;
	task_list* previous = NULL;

　
//	printf("%d, %d\n",*head->tid, *head->creation_time);
	if(*head == NULL){
		return;
	}

//	delay(500);
//	printf("im in deleting the likined list and tid is:%d\n", tid);
//	delay(500);
//	printf("im in deleting the likined list and current->tid is:%d\n", current->tid);
//	delay(500);
//	printf("im in deleting the likined list and creation_time is:%d\n", creation_time);
//	delay(500);
//	printf("im in deleting the likined list and current->creation_time is:%d\n", current->creation_time);

　
//	while((current->tid != tid) || (current->creation_time != creation_time)){
	while((current->tid != tid)){
		if(current->next_cell == NULL){
			return;
		}else{
			previous = current;
			current = current->next_cell;
		}
	}

　
	if(current == *head){
		*head = (*head)->next_cell;
	}else{
		current->previous_cell->next_cell = current->next_cell;
	}

	if(current == *last){
		*last = current->previous_cell;
	}else{
		current->next_cell->previous_cell = current->previous_cell;
	}
	delay(100);
	puts("end of delete>>>>>>>>>>>>>>>>>>>>>>>>>");

}

// might have to call task_block on each created task, so scheduler can unblock and run
void schedule_task(_task_id tid)
{

	_mqx_uint old_priority;
	_mqx_uint change_task_new_priority = 16;
	_mqx_uint shdled_priority = 10;

	// changes priority of prev running task
	if(is_running == 1){
	// change priority of the running task to 16
		delay(100);
		puts("\nis_running == 1, in schedule_task");
		if(_task_get_priority(running_task, &old_priority) != MQX_OK)
			printf("\nCouldnt get running task Priority\n");
		if(_task_set_priority(running_task, change_task_new_priority, &old_priority) != MQX_OK)
			printf("\nCouldnt SET running task Priority\n");
		delay(100);
		printf("\nREPLACING PRIORITY >>>old pri: %d,  new pri: %d\n", old_priority, change_task_new_priority);
	}

　
　
	running_task = tid;

　
	if(is_running == 0){
		is_running = 1;
		// change priority of the task to 10
		if(_task_get_priority(running_task, &old_priority) != MQX_OK)
			printf("\nCouldnt get task Priority\n");
		if(_task_set_priority(running_task, shdled_priority, &old_priority) != MQX_OK)
			printf("\nCouldnt SET new_task Priority\n");
		delay(100);
		printf("\nINSIDE SCHEDULE TASK>>>old pri: %d,  new pri: %d\n", old_priority, shdled_priority);

　
		TD_STRUCT_PTR td_ptr = _task_get_td(tid);

		if (td_ptr != NULL){
			delay(500);
			printf("Task Ready called\n");
			_task_ready(td_ptr);
		}
	}

}
