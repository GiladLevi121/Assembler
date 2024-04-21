#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

/* Global variables*/
volatile sig_atomic_t sem = 0; /* Binary semaphore */
sigset_t signal_mask; /* Signal mask*/

int init_sem(int value) {
    if (value != 0 && value != 1)
        return -1; /* Invalid value for semaphore initialization*/
    sem = value;
    sigaddset(&signal_mask, SIGUSR1);
    if (pthread_sigmask(SIG_BLOCK, &signal_mask, NULL) != 0) {
        perror("pthread_sigmask");
        return -1;
    }
    return 0;
}

void down_sem() {
    while (1) {
        if (__sync_bool_compare_and_swap(&sem, 1, 0))
            break;
        else{
            int sig;
            sigwait(&signal_mask, &sig);
        }
    }
}

void up_semo (){
    sem = 1;
    kill(getpid(), SIGUSR1); /*Send a signal to the process itself*/
}

