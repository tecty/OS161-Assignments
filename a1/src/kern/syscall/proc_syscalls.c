/*
 * Process-related syscalls.
 * New for ASST1.
 */

#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <current.h>
#include <pid.h>
#include <machine/trapframe.h>
#include <syscall.h>
#include <copyinout.h>

/*
 * sys_fork
 * 
 * create a new process, which begins executing in md_forkentry().
 */


int
sys_fork(struct trapframe *tf, pid_t *retval)
{
	struct trapframe *ntf; /* new trapframe, copy of tf */
	int result;

	/*
	 * Copy the trapframe to the heap, because we might return to
	 * userlevel and make another syscall (changing the trapframe)
	 * before the child runs. The child will free the copy.
	 */

	ntf = kmalloc(sizeof(struct trapframe));
	if (ntf==NULL) {
		return ENOMEM;
	}
	*ntf = *tf; /* copy the trapframe */

	result = thread_fork(curthread->t_name, enter_forked_process, 
			     ntf, 0, retval);
	if (result) {
		kfree(ntf);
		return result;
	}

	return 0;
}

/*
 * sys_getpid
 * Placeholder to remind you to implement this.
 */
pid_t sys_getpid(void) {
    return curthread->t_pid;
}

/*
 * sys_waitpid
 * Placeholder comment to remind you to implement this.
 */
int sys_waitpid(pid_t pid, userptr_t status, int flags, int* error) {
    int status_int, retval, result;
    // The flag must be set to WNOHANG or 0, otherwise the EINVAL error is 
    // raised. 
    // WNOHANG == 1 and the above include statement satisfies netbeans but
    // the compiler is not satisfied.
    if (!(flags == 1 || flags == 0)) {
	*error = EINVAL;
        return -1;
    // If the status argument is an invalid argument, it will return EFAULT.
    } else if (status == NULL) {
	*error = EFAULT;
        return -1;
    }
    // ESRCH and ECHILD type error situations are handled by pid_join.
    retval = pid_join(pid, &status_int, flags);
    result = copyout(&status_int, status, sizeof (int));
    if (result) {
        *error = result;
        return -1;
    }
    return retval;
}


/*
 * sys_kill
 * Placeholder comment to remind you to implement this.
 */



