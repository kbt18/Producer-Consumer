/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"

void *producer (void *id);
void *consumer (void *id);

int main (int argc, char **argv)
{

  //pthread_exit(0);
  try {
    valid_input(argc, argv);
  } catch (int i) {
    return i;
  }

  int q_size = atoi(argv[1]), n_jobs = atoi(argv[2]);
  int n_producers = atoi(argv[3]), n_consumers = atoi(argv[4]);
  // int param = 1;

  Job** job_array = new Job* [q_size];
  //initialise to NULL
  for (int i = 0; i < q_size; i++)
    job_array[i] = NULL;

  // create semaphores
  key_t semkey = SEM_KEY;
  int semid = sem_create(semkey, 3);
  if (semid < 0) {
    cout << "key error" << endl;
    return(-1);
  }

  sem_init(semid, 0, 1); //mutex
  sem_init(semid, 1, 0); //full
  sem_init(semid, 2, q_size); //empty

  // Producer_parameters p_params = {
  //   .njobs = n_jobs,
  //   .semid = semid,
  //   .job_array_pointer = job_array
  // };

  // Consumer_parameters c_params = {
  //   .semid = semid,
  //   .q_size = q_size,
  //   .job_array_pointer = job_array
  // };

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

  for (int i = 0; i < n_producers; i++) {
    pthread_join(*producers[i], NULL);
    printf ("Producer (%i): No more jobs to generate.\n", i + 1);
  }


  for (int i = 0; i < n_consumers; i++) {
    pthread_join(*consumers[i], NULL);
    printf ("Consumer (%i): No more jobs left.\n", i + 1);
  }


  sem_close(semid);

  // for (int i = 0; i < q_size; i++)
  //   cout << "job id " << (*job_array[i]).id << endl;
  //cout << "end of program\n";
  pthread_exit(0);

  /* ~~~ EXAMPLE THREAD ~~~ */
  // int parameter = 5;
  //
  // pthread_t producerid;
  //
  // pthread_create (&producerid, NULL, producer, (void *) &parameter);
  //
  // pthread_join (producerid, NULL);
  // //pthread_exit ((void*)producerid);
  // //exit(0);
  //
  // cout << "Doing some work after the exit/join" << endl;
  //
  // return 0;
}

void *producer (void *parameter)
{

  // TODO
  Producer_parameters* params = (Producer_parameters*) parameter;
  int* semid = &(params->semid);
  int njobs = params->njobs;

  while (njobs > 0) {
    Job* job = new Job;
    njobs--;
    //cout << *njobs << endl;
    job->duration = (rand() % 10) + 1;

    sem_timedwait(*semid, 2, 20);
    sem_wait(*semid, 0);
    //cout << "entering critical section" << endl;
    int index = 0;
    while (params->job_array_pointer[index] != NULL) //will always be a space
      index++;

    //int index = sem_checkval(*semid, 1); //gives non-deterministic behaviour
    job->id = index + 1;
    printf ("Producer(%i): job id %i duration %i\n", params->producer_id, job->id, job->duration);

    //cout << "adding job " << index + 1 << " at index " << index << endl;
    params->job_array_pointer[index] = job;
    //cout << "index is " << index << endl;

    //cout << (params->job_array_pointer[index])->id << endl;
    //cout << (params->job_array_pointer[index])->duration << endl;
    //cout << "exiting critical section" << endl << endl;
    sem_signal(*semid, 0);
    sem_signal(*semid, 1);
  }

  pthread_exit(0);
  // int *param = (int *) parameter;
  //
  // cout << "Parameter = " << *param << endl;
  //
  // sleep (2);
  // cout << "That was a good sleep - thank you" << endl;
  //
  // pthread_exit(0);
}

void *consumer (void *parameter)
{
  // TODO
  Consumer_parameters* params = (Consumer_parameters*) parameter;
  int* semid = &(params->semid);
  Job* job;

  do{

  sem_timedwait(*semid, 1, 20);
  sem_wait(*semid, 0);

  // remove item from buffer
  job = params->job_array_pointer[0];

  int index = 0;
  while (params->job_array_pointer[index] != NULL && index < params->q_size)
    index++;

  for (int i = 0; i < index - 1; i++)
    params->job_array_pointer[i] = params->job_array_pointer[i + 1];

  index--;
  //cout << "deleting element " << index << endl;
  params->job_array_pointer[index] = NULL;


  sem_signal(*semid, 0);
  sem_signal(*semid, 2);

  // consumes item
  int id = job->id;
  int duration = job->duration;
  printf ("Consumer(%i): job id %i executing sleep duration %i\n", params->consumer_id, id, duration);
  sleep(duration);

}while(true);
  pthread_exit (0);

}
