// scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

void init_scheduler();
void create_task(void (*func)());
void start_scheduler();

#endif
