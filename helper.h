/******************************************************************
 * Header file for the helper functions. This file includes the
 * required header files, as well as the function signatures and
 * the semaphore values (which are to be changed as needed).
 ******************************************************************/
// #include <signal.h>
// #include <semaphore.h>

#include <boost/circular_buffer.hpp>

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

# define SEM_KEY 0x387; // Change this number as needed

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

struct Job {
  int id;
  int duration;
};

typedef boost::circular_buffer<Job*>* cb_ptr;

struct Producer_parameters {
  int njobs;
  int semid;
  int producer_id;
  cb_ptr ring_buff;
  Job** job_array_pointer;
};

struct Consumer_parameters {
  int semid;
  int q_size;
  int consumer_id;
  cb_ptr ring_buff;
  Job** job_array_pointer;
};

int sem_timedwait (int id, short unsigned int num, int time);
// id is the id of a semaphore set.
// num is the semaphore number in the set that you wish to wait on.
// time is the time limit in seconds after which the semaphore will
// stop waiting and an error code will be returned.

bool is_integer(const char* c_string);
// Checks if a string is an integer.

void valid_input(int argc, char** argv);
// Checks if argc == 5 and if arguments 2 to 5 are integers.

void create_sems(int& semid, key_t semkey, int q_size);
// semid is the id of a semaphore set
// semkey is a key which will be used to create the semaphore set
// q_size is the maximum size of...
// Creates three semaphres, mutex, empty and full.
