#include "scheduler.h"
#include <stdio.h>

static Task tasks[MAX_PROCESSES];
static int task_count = 0;
static int current = -1;

void init_scheduler() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        tasks[i].id = -1;
        tasks[i].state = TASK_FINISHED;
        tasks[i].entry = 0;
    }
    task_count = 0;
    current = -1;

    printf("[SCHED] Initialized.\n");
}

int create_task(TaskFunction func) {
    if (task_count >= MAX_PROCESSES) {
        printf("[SCHED] ERROR: Max task limit reached.\n");
        return -1;
    }

    tasks[task_count].id = task_count;
    tasks[task_count].state = TASK_READY;
    tasks[task_count].entry = func;

    printf("[SCHED] Created task %d.\n", task_count);

    return task_count++;
}

static int pick_next_task() {
    if (task_count == 0) return -1;

    for (int i = 1; i <= task_count; i++) {
        int idx = (current + i) % task_count;
        if (tasks[idx].state == TASK_READY)
            return idx;
    }

    return -1;
}

void scheduler_tick() {
    int next = pick_next_task();
    if (next < 0) return;

    current = next;
    tasks[current].state = TASK_RUNNING;

    printf("[SCHED] Running task %d...\n", current);

    tasks[current].entry();

    tasks[current].state = TASK_FINISHED;
}

void scheduler_run() {
    int unfinished = 1;

    while (unfinished) {
        unfinished = 0;

        for (int i = 0; i < task_count; i++) {
            if (tasks[i].state != TASK_FINISHED) {
                scheduler_tick();
                unfinished = 1;
            }
        }
    }

    printf("[SCHED] All tasks completed.\n");
}

int get_running_task_id() {
    return current;
}
