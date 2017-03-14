#include "access_functions.h"


_task_id dd_tcreate(uint32_t template_index, uint32_t deadline){


	_mqx_uint old_priority_ptr;
	_mqx_uint priority_ptr = 12;

	/*Create queue for dd_tcreate*/
	tcreate_qid = _msgq_open(TCREAT_QUEUE, 0);
	if(tcreate_qid == 0){
		printf("\nCould not open the tcreate queue\n");
		_task_block();
	}

	/*create tasks*/
	_task_id new_tid = _task_create(0, template_index,0);
	MSG_PTR msg_ptr = (MSG_PTR)_msg_alloc(msg_pool);
	_time_get_elapsed(&start_t);

	// change priority of the task to 12
	_task_get_priority(new_tid, &old_priority_ptr);
	_task_set_priority(new_tid, priority_ptr, &old_priority_ptr);
	printf("old pri: %d,  new pri: %d\n", old_priority_ptr, priority_ptr );

//	printf("tid %d id created\n",new_tid);

	msg_ptr->HEADER.SOURCE_QID = tcreate_qid;
	msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, DD_QUEUE);
	msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + sizeof(int)*4;
	msg_ptr->tid = new_tid;
//	msg_ptr->execution_time = execution_time;
	msg_ptr->deadline = deadline;
	msg_ptr->creation_time = start_t.SECONDS;
	msg_ptr->priority = priority_ptr;

	/*send msg to dd_scheduler*/
//	printf("qid in access %d\n",tcreate_qid);

	_msgq_send(msg_ptr);
	msg_ptr = _msgq_receive(tcreate_qid, 0);
	if(msg_ptr->tid == new_tid){
		puts("\nTask in scheduled list\n");
		_msgq_close(tcreate_qid);
		_msg_free(msg_ptr);
	}else{
		printf("tid>>>%d\n", msg_ptr->tid);
	}

//	_msg_free(msg_ptr);

	return new_tid;
}


uint32_t dd_delete(_task_id task_id,uint32_t creation_time){
	MSG_PTR msg_ptr = (MSG_PTR)_msg_alloc(msg_pool);
	puts("im deleting>>>");
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
   printf("\n[");

   while(ptr != NULL) {
      printf("(tid:%d, c:%d, d:%d) ",ptr->tid,ptr->creation_time, ptr->deadline);
      ptr = ptr->next_cell;
   }

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

	while((current->tid != tid) || (current->creation_time != creation_time)){
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

}

// might have to call task_block on each created task, so scheduler can unblock and run
void schedule_task(_task_id tid)
{
	TD_STRUCT_PTR td_ptr = _task_get_td(tid);
	if ((td_ptr != NULL) && (td_ptr->STATE == WAIT_BLOCKED)){
	 _task_ready(td_ptr);
	}

}


















