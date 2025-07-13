#include <llulu/lu_hooks.h>
#include <llulu/lu_error.h>

#include <stddef.h>

typedef struct lu_hook_lists {
    lu_hook_task *prio_list[LU_PRIORITY_COUNT];
    void *hook_data;
} lu_hook_lists;

static lu_hook_lists hooks[LU_HOOK_COUNT];

void
lu_hook_notify(int hook, void *hook_data)
{
    lu_hook_task *it = hooks[hook].prio_list[LU_PRIORITY_INTERRUPT];
    while (it) {
        it->run(it->task_data, hook_data);
        it = it->next;
    }

    hooks[hook].hook_data = hook_data;

    // TODO: async and discardable calls
}

void
lu_hook_attach(int hook, int priority, lu_hook_task *task)
{
    lu_err_assert(priority == LU_PRIORITY_INTERRUPT); // TODO: Temporal until other priorities are implemented
    lu_err_expects(task);
    lu_err_expects(hook >= 0 && hook < LU_HOOK_COUNT);
    lu_err_expects(priority >= 0 && priority < LU_PRIORITY_COUNT);

    task->next = hooks[hook].prio_list[priority];
    hooks[hook].prio_list[priority] = task;

    lu_err_ensures(hooks[hook].prio_list[priority] != NULL);
}

void
lu_hook_dettach(int hook, int priority, lu_hook_task *task)
{
    if (!task) {
        return;
    }

    lu_err_expects(hook >= 0 && hook < LU_HOOK_COUNT);
    lu_err_expects(priority >= 0 && priority < LU_PRIORITY_COUNT);

    lu_hook_task *it = hooks[hook].prio_list[priority];

    if (it == task) {
        hooks[hook].prio_list[priority] = it->next;
    } else {
        while (it->next) {
            if (it->next == task) {
                it->next = it->next->next;
                break;
            } else {
                it = it->next;
            }
        }
    }
    task->next = NULL;
}
