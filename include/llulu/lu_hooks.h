#ifndef LLULU_LU_HOOKS_H
#define LLULU_LU_HOOKS_H

enum {
    LU_HOOK_PRE_INIT,
    LU_HOOK_POST_INIT,
    LU_HOOK_PRE_SHUTDOWN,
    LU_HOOK_POST_SHUTDOWN,
    LU_HOOK_PRE_UPDATE,
    LU_HOOK_POST_UPDATE,
    LU_HOOK_PRE_RENDER,
    LU_HOOK_POST_RENDER,
    LU_HOOK_ABORT,
    LU_HOOK_ERROR,
    LU_HOOK_TRACE,
    LU_HOOK_COUNT
};

enum {
    /* Run the tasksk from the thread that broadcasted the hook notification.
       Every entry with this level of priority is guaranteed to be executed before
       leaving the hook notification scope. */
    LU_PRIORITY_INTERRUPT, 

    /* Run the tasks from another thread without blocking the main execution.
       If the frame ends and there are entries remaining, blocks until the list is processed
       before moving to the next frame. */
    LU_PRIORITY_ASYNC, 

    /* Run the tasks from another thread once all the async entries are processed.
       If the frame ends skips the remaining entries and move to the next frame without blocking. */
    LU_PRIORITY_DISCARDABLE,
    LU_PRIORITY_COUNT
};

typedef void (*lu_hook_callback)(void *task_data, void *hook_data);

typedef struct lu_hook_task {
    lu_hook_callback run;
    void *task_data;
    struct lu_hook_task *next;
} lu_hook_task;

void lu_hook_notify(int hook, void *hook_data);
void lu_hook_attach(int hook, int priority, lu_hook_task *task);
void lu_hook_dettach(int hook, int priority, lu_hook_task *task);

#endif /* LLULU_LU_HOOKS_H */
