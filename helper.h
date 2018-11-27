/******************************************************************
 * Header file for the helper functions. This file includes the
 * required header files, as well as the function signatures and
 * the semaphore values (which are to be changed as needed).
 ******************************************************************/
// #include <signal.h>
// #include <semaphore.h>

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/time.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <pthread.h>
# include <ctype.h>
# include <iostream>
using namespace std;

# define SEM_KEY 0x76; // Change this number as needed

union semun {
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    ushort *array;         /* used for GETALL and SETALL */
};

int check_arg (char *);
int sem_create (key_t key, int num);
int sem_init (int id, int num, int value);
void sem_wait (int, short unsigned int);
void sem_signal (int, short unsigned int);
int sem_close (int);

// ~~~ MY HELPER FUNCTIONS ~~~ //
struct Job {
  int id;
  int duration;
};

struct Producer_parameters {
  int njobs;
  int semid;
  Job** job_array_pointer;
};

struct Consumer_parameters {
  int semid;
  int q_size;
  Job** job_array_pointer;
};

int sem_checkval(int id, int num);

// void sem_timedwait (int id, short unsigned int num, int time);

// struct timespec {
//     time_t tv_sec;      /* Seconds */
//     long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
// };

bool is_integer(const char* c_string);
void valid_input(int argc, char** argv);

// ~~~ TEST CODE ~~~ //
