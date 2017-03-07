#include "access_functions.h"


_task_id dd_tcreate(unit_32 template_index, unit_32 deadline){

	return 0;
}


uint_32 dd_delete(uint_32 task_id){
	return 0;
}

uint_32 dd_return_active_list(task_list **list){
	return 0;
}

uint_32 dd_return_overdue_list(task_list **list){
	return 0;
}

void dd_init(){
	return 0;

}


/*linked list functions*/
bool isEmpty(task_list *head){
	return head ==NULL;
}

int length(task_list *head){
	   int length = 0;
	   task_list *current;

	   for(current = head; current != NULL; current = current->next_cell){
	      length++;
	   }

	   return length;
}

void displayForward(task_list *head) {

   //start from the beginning
   task_list *ptr = head;

   //navigate till the end of the list
   printf("\n[ ");

   while(ptr != NULL) {
      printf("(%d,%d) ",ptr->tid,ptr->creation_time);
      ptr = ptr->next_cell;
   }

   printf(" ]");
}

void displayBackward(task_list *head) {

   //start from the last
	task_list *ptr = last;

   //navigate till the start of the list
   printf("\n[ ");

   while(ptr != NULL) {

      //print data
      printf("(%d,%d) ",ptr->tid,ptr->creation_time);

      //move to next item
      ptr = ptr ->previous_cell;
      printf(" ");
   }

   printf(" ]");
}

//insert link at the first location
void insertFirst(task_list *head, task_list *tail, uint_32 tid, uint_32 deadline, uint_32 creation_time) {
   //create a link
   task_list *link = (task_list*) malloc(sizeof(task_list));
   link->tid = tid;
   link->deadline = deadline;
   link->creation_time = creation_time;

   if(isEmpty()) {
	  //make it the last link
	  tail = link;
   } else {
	  //update first prev link
	  head->previous_cell = link;
   }

   //point it to old first link
   link->next_cell = head;

   //point first to new first link
   head = link;
}

//insert link at the last location
void insertLast(task_list *tail, uint_32 tid, uint_32 deadline, uint_32 creation_time) {

   //create a link
   task_list *link = (task_list*) malloc(sizeof(task_list));
   link->tid = tid;
   link->deadline = deadline;
   link->creation_time = creation_time;

   if(isEmpty()) {
      //make it the last link
	   tail = link;
   } else {
      //make link a new last link
	   tail->next_cell = link;

      //mark old last node as prev of new link
      link->previous_cell = tail;
   }

   //point last to new last node
   tail = link;
}

//delete first item
task_list* deleteFirst(task_list *head, task_list *last) {

   //save reference to first link
	task_list *tempLink = head;

   //if only one link
   if(head->next_cell == NULL){
      last = NULL;
   } else {
      head->next->previous_cell = NULL;
   }

   head = head->next_cell;
   //return the deleted link
   free(tempLink);
   return tempLink;
}

task_list* deleteLast(task_list *head, task_list *last) {
   //save reference to last link
	task_list *tempLink = last;

   //if only one link
   if(head->next_cell == NULL) {
      head = NULL;
   } else {
      last->previous_cell->next_cell = NULL;
   }

   last = last->previous_cell;

   free(tempLink);
   //return the deleted link
   return tempLink;
}

//delete according to tid
task_list* delete(task_list* head, uint_32 tid, task_list *last) {

   //start from the first link
   task_list* current = head;
   task_list* previous = NULL;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->tid != tid) {
      //if it is last node

      if(current->next_cell == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;

         //move to next link
         current = current->next_cell;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next_cell;
   } else {
      //bypass the current link
      current->previous_cell->next_cell = current->next_cell;
   }

   if(current == last) {
      //change last to point to prev link
      last = current->previous_cell;
   } else {
      current->next_cell->previous_cell = current->previous_cell;
   }

   free(current);
   return current;
}

bool insertAfter(task_list* head, uint_32 tid, task_list *last, uint_32 newtid, uint_32 deadline, uint_32 creation_time) {
   //start from the first link
	task_list* current = head;

   //if list is empty
   if(head == NULL) {
      return false;
   }

   //navigate through list
   while(current->tid != tid) {

      //if it is last node
      if(current->next_cell == NULL) {
         return false;
      } else {
         //move to next link
         current = current->next_cell;
      }
   }

   //create a link
   task_list* newLink = (task_list*) malloc(sizeof(task_list));
   newLink->tid = newtid;
   newLink->deadline = deadline;
   newLink->creation_time = creation_time;

   if(current == last) {
      newLink->next_cell = NULL;
      last = newLink;
   } else {
      newLink->next_cell = current->next_cell;
      current->next_cell->previous_cell = newLink;
   }

   newLink->previous_cell = current;
   current->next_cell = newLink;
   return true;
}








