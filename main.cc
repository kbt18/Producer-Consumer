/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"
#include "consumer.h"
#include "producer.h"

void *producer (void *id);
void *consumer (void *id);

int main (int argc, char **argv)
{

  try {
    valid_input(argc, argv);
  } catch (int i) {
    return i;
  }

  int q_size = atoi(argv[1]);
  int n_jobs = atoi(argv[2]);
  int n_producers = atoi(argv[3]);
  int n_consumers = atoi(argv[4]);

  Job** job_array = new Job* [q_size];
  for (int i = 0; i < q_size; i++)
    job_array[i] = NULL;

  // create semaphore set
  key_t semkey = SEM_KEY;
  int semid = sem_create(semkey, 3);
  if (semid < 0) {
    cout << "key error" << endl;
    return(-1);
  }

  sem_init(semid, 0, 1); //mutex
  sem_init(semid, 1, 0); //full
  sem_init(semid, 2, q_size); //empty

  pthread_t** producers = new pthread_t*[n_producers];
  for (int i = 0; i < n_producers; i++) {

    Producer_parameters* p_params = new Producer_parameters;

    p_params->njobs = n_jobs;
    p_params->semid = semid;
    p_params->producer_id = i + 1;
    p_params->job_array_pointer = job_array;

    producers[i] = new pthread_t;
    pthread_create (producers[i], NULL, producer, (void*) p_params);
  }

  pthread_t** consumers = new pthread_t*[n_consumers];
  for (int i = 0; i < n_consumers; i++) {

    Consumer_parameters* c_params = new Consumer_parameters;

    c_params->semid = semid;
    c_params->q_size = q_size;
    c_params->consumer_id = i + 1;
    c_params->job_array_pointer = job_array;

    consumers[i] = new pthread_t;
    pthread_create (consumers[i], NULL, consumer, (void*) c_params);
  }

  for (int i = 0; i < n_producers; i++)
    pthread_join(*producers[i], NULL);

  for (int i = 0; i < n_consumers; i++)
    pthread_join(*consumers[i], NULL);

  sem_close(semid);
  return(0);
}
