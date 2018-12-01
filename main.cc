/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"

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

  typedef boost::circular_buffer<Job*> circular_buffer;
  circular_buffer cb{(long unsigned int)q_size};

  Job** job_array = new Job* [q_size];
  for (int i = 0; i < q_size; i++)
    job_array[i] = NULL;

  key_t semkey = SEM_KEY;
  int semid;
  try {
    create_sems(semid, semkey, q_size);
  } catch (int i) {
    return i;
  }

  pthread_t** producers = new pthread_t*[n_producers];
  for (int i = 0; i < n_producers; i++) {

    Producer_parameters* p_params = new Producer_parameters;

    p_params->njobs = n_jobs;
    p_params->semid = semid;
    p_params->producer_id = i + 1;
    p_params->ring_buff = &cb;
    p_params->job_array_pointer = job_array;

    producers[i] = new pthread_t;
    if (pthread_create (producers[i], NULL, producer, (void*) p_params) != 0) {
        cerr << "Error creating producer thread.\n";
        //must close other threads!!
        return (-1);
    }
  }

  pthread_t** consumers = new pthread_t*[n_consumers];
  for (int i = 0; i < n_consumers; i++) {

    Consumer_parameters* c_params = new Consumer_parameters;

    c_params->semid = semid;
    c_params->q_size = q_size;
    c_params->consumer_id = i + 1;
    c_params->ring_buff = &cb;
    c_params->job_array_pointer = job_array;

    consumers[i] = new pthread_t;
    if (pthread_create (consumers[i], NULL, consumer, (void*) c_params) != 0) {
      cerr << "Error creating consumer thread.\n";
      //must close other threads!
      return -1;
    };
  }

  for (int i = 0; i < n_producers; i++)
    if (pthread_join(*producers[i], NULL) != 0) {
      cerr << "Error joining producer thread.\n";
      return (-1);
    }

  for (int i = 0; i < n_consumers; i++)
    if (pthread_join(*consumers[i], NULL) < 0) {
      cerr << "Error joining consumer thread.\n";
      return (-1);
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
  Job** job_array = params->job_array_pointer;

  while (*njobs > 0) {
    Job* job = new Job;
    (*njobs)--;
    job->duration = (rand() % 10) + 1;

    if (sem_timedwait(*semid, 2, 20) < 0) {
      printf ("Producer (%i): Died of waiting.\n", *p_id);
      pthread_exit(0);
    }
    sem_wait(*semid, 0);

    job->id = cb->size() + 1;
    cb->push_back(job);

    // int index = 0;
    // while (job_array[index] != NULL) //will always be a space
    //   index++;
    // job->id = index + 1;
    // job_array[index] = job;

    sem_signal(*semid, 0);
    sem_signal(*semid, 1);

    printf ("Producer(%i): job id %i duration %i\n",
      *p_id, job->id, job->duration);

    sleep(rand() % 5 + 1);
  }

  printf ("Producer (%i): No more jobs to generate.\n", *p_id);
  pthread_exit(0);
}

void *consumer (void *parameter)
{
  Consumer_parameters* params = (Consumer_parameters*) parameter;
  int* semid = &(params->semid);
  int* q_size = &(params->q_size);
  int* c_id = &(params->consumer_id);
  cb_ptr cb = params->ring_buff;
  Job** jobs = params->job_array_pointer;

  do {
    if (sem_timedwait(*semid, 1, 20) < 0) {
      printf ("Consumer (%i): No more jobs left.\n", *c_id);
      pthread_exit(0);
    }
    sem_wait(*semid, 0);

    Job* job = cb->front();
    cb->pop_front();
    //Job* job = jobs[0];

    // int index = 0;
    // while (jobs[index] != NULL && index < *q_size)
    //   index++;
    //
    // for (int i = 0; i < index - 1; i++)
    //   jobs[i] = jobs[i + 1];

    // index--;
    // jobs[index] = NULL;

    sem_signal(*semid, 0);
    sem_signal(*semid, 2);

    int id = job->id;
    int duration = job->duration;
    //Shouldn't this job id always be zero?
    printf ("Consumer(%i): job id %i executing sleep duration %i\n",
      *c_id, id, duration);

    sleep(duration);

    printf ("Consumer(%i): job id %i completed.\n", *c_id, id);
  } while(true);

  pthread_exit (0);
}
