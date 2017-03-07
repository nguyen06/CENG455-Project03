/*
 * access_functions.h
 *
 *  Created on: Mar 6, 2017
 *      Author: nguyen06
 */
#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include "global_variables.h"

#ifndef SOURCES_ACCESS_FUNCTIONS_H_
#define SOURCES_ACCESS_FUNCTIONS_H_

_task_id dd_tcreate(uint_32 template_index, uint_32 deadline);
uint_32 dd_delete(uint_32 task_id);
uint_32 dd_return_active_list(task_list **list);
uint_32 dd_return_overdue_list(task_list **list);
void dd_init();

/*linked list functions*/
bool isEmpty(task_list *head);
int length(task_list *head);
void displayForward(task_list *head);
void displayBackward(task_list *head);
void insertFirst(task_list *head, task_list *tail, uint_32 tid, uint_32 deadline, uint_32 creation_time);
void insertLast(task_list *tail, uint_32 tid, uint_32 deadline, uint_32 creation_time);
task_list* deleteFirst(task_list *head, task_list *last);
task_list* deleteLast(task_list *head, task_list *last);
task_list* delete(task_list* head, uint_32 tid, task_list *last);
insertAfter(task_list* head, uint_32 tid, task_list *last, uint_32 newtid, uint_32 deadline, uint_32 creation_time);


#endif /* SOURCES_ACCESS_FUNCTIONS_H_ */
