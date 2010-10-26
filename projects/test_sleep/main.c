#include <stdio.h>
#include <thread.h>
#include <kernel.h>
#include <hwtimer.h>

int integer = 0;
int i = 0;

void second_thread(void) {
    printf("Woke up!\n");
    while(1) {
        integer++;
        printf("sleeper: running. integer=%i, i=%i.\n", integer, i);
        if (integer % 100 == 0) { 
            printf("Going to sleep.\n");
            thread_sleep();
        }
    }
}

tcb second_thread_tcb;
char second_thread_stack[KERNEL_CONF_STACKSIZE_DEFAULT];

int main(void)
{
    hwtimer_init();

    printf("Hello world!\n");

    int pid = thread_create(&second_thread_tcb, second_thread_stack, sizeof(second_thread_stack), PRIORITY_MAIN-1, CREATE_STACKTEST | CREATE_SLEEPING | CREATE_WOUT_YIELD, second_thread, "sleeper");

    if (pid < 0) {
        puts("Error creating second_thread! Stopping test.");
        while(1);
    }

    while(1) {
        i++;
        printf("   main: running. integer=%i, i=%i.\n", integer, i);
        if (i % 100 == 0) { 
            printf("Waking up sleeper.\n");
            thread_wakeup(pid);
            fk_yield();
        }
    }
}
