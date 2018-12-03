/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"

void *producer (void *id);
void *consumer (void *id);

int main (int argc, char **argv)
{

  if (!is_valid_input(argc, argv))
    return 1;

  int q_size = atoi(argv[1]);
  int n_jobs = atoi(argv[2]);
  int n_producers = atoi(argv[3]);
  int n_consumers = atoi(argv[4]);

  typedef boost::circular_buffer<Job*> circular_buffer;
  circular_buffer cb((long unsigned int)q_size);

  Job** job_array = new Job* [q_size];
  for (int i = 0; i < q_size; i++)
    job_array[i] = NULL;

  key_t semkey = SEM_KEY;
  int semid;
  if (create_sems(semid, semkey, q_size) < 0)
    return -1;

  pthread_t** producers = new pthread_t*[n_producers];
  for (int i = 0; i < n_producers; i++) {
    producers[i] = new pthread_t;
    Producer_parameters* p_params = new Producer_parameters {
      n_jobs,
      semid,
      i + 1,
      &cb
    };

    if (pthread_create (producers[i], NULL, producer, (void*) p_params) != 0) {
        cerr << "Error creating producer thread.\n";
        pthread_exit(0);
    }
  }

  pthread_t** consumers = new pthread_t*[n_consumers];
  for (int i = 0; i < n_consumers; i++) {
    consumers[i] = new pthread_t;
    Consumer_parameters* c_params = new Consumer_parameters {
      semid,
      i + 1,
      &cb
    };

    if (pthread_create (consumers[i], NULL, consumer, (void*) c_params) != 0) {
      cerr << "Error creating consumer thread.\n";
      pthread_exit(0);
    };
  }

  for (int i = 0; i < n_producers; i++)
    if (pthread_join(*producers[i], NULL) != 0) {
      cerr << "Error joining producer thread.\n";
      pthread_exit(0);
    }

  for (int i = 0; i < n_consumers; i++)
    if (pthread_join(*consumers[i], NULL) < 0) {
      cerr << "Error joining consumer thread.\n";
      pthread_exit(0);
    }

  sem_close(semid);
  return(0);
}

void *producer (void *parameter)
{
  Producer_parameters* params = (Producer_parameters*) parameter;
  int* semid = &(params->semid);
  int* njobs = &(params->njobs);
  int* p_id = &(params->producer_id);
  cb_ptr cb = params->ring_buff;

  while (*njobs > 0) {
    Job* job = new Job;
    (*njobs)--;
    job->duration = (rand() % 10) + 1;

    if (sem_timedwait(*semid, 2, 20) < 0) {
      printf ("Producer(%i): Died waiting for a space in the queue.\n", *p_id);
      pthread_exit(0);
    }
    sem_wait(*semid, 0);

    job->id = cb->size() + 1;
    cb->push_back(job);

    sem_signal(*semid, 0);
    sem_signal(*semid, 1);

    printf ("Producer(%i): job id %i duration %i\n",
      *p_id, job->id, job->duration);

    sleep((rand() % 5) + 1);
  }

  printf ("Producer(%i): No more jobs to generate.\n", *p_id);
  pthread_exit(0);
}

void *consumer (void *parameter)
{
  Consumer_parameters* params = (Consumer_parameters*) parameter;
  int* semid = &(params->semid);
  int* c_id = &(params->consumer_id);
  cb_ptr cb = params->ring_buff;

  do {
    if (sem_timedwait(*semid, 1, 20) < 0) {
      printf ("Consumer(%i): No more jobs left.\n", *c_id);
      pthread_exit(0);
    }
    sem_wait(*semid, 0);

    Job* job = cb->front();
    cb->pop_front();

    sem_signal(*semid, 0);
    sem_signal(*semid, 2);

    int id = job->id;
    int duration = job->duration;

    printf ("Consumer(%i): job id %i executing sleep duration %i\n",
      *c_id, id, duration);

    sleep(duration);

    printf ("Consumer(%i): job id %i completed.\n", *c_id, id);
  } while(true);

  pthread_exit (0); // redundant
}
