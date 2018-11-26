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

  int q_size = atoi(argv[1]), n_jobs = atoi(argv[2]);
  int n_producers = atoi(argv[3]), n_consumers = atoi(argv[4]);
  int param = 1;

  // Job job_array[q_size];

  // create semaphores
  key_t semkey = SEM_KEY;
  int semid = sem_create(semkey, 3);
  sem_init(semid, 0, 1); //mutex
  sem_init(semid, 1, 0); //full
  sem_init(semid, 2, q_size); //empty

  Producer_parameters p_params = {
    .njobs = n_jobs,
    .semid = semid };

  pthread_t** producers = new pthread_t*[n_producers];
  for (int i = 0; i < n_producers; i++) {
    producers[i] = new pthread_t;
    pthread_create (producers[i], NULL, producer, (void*) &p_params);
  }

  pthread_t** consumers = new pthread_t*[n_consumers];
  for (int i = 0; i < n_consumers; i++) {
    consumers[i] = new pthread_t;
    pthread_create (consumers[i], NULL, consumer, (void*) &param);
  }

  for (int i = 0; i < n_producers; i++)
    pthread_join(*producers[i], NULL);

  for (int i = 0; i < n_consumers; i++)
    pthread_join(*consumers[i], NULL);

  sem_close(semid);
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
  int* njobs = &(params->njobs);

  Job* job = new Job;
  job->duration = (rand() % 10) + 1;

  sem_wait(*semid, 2);
  sem_wait(*semid, 0);
  cout << "entering critical section" << endl;

  int index = sem_checkval(*semid, 1);
  int id = index + 1;
  job->id = id;

  cout << "index is " << index << endl;
  cout << "id is " << id << endl;

  cout << "exiting critical section" << endl << endl;

  sem_signal(*semid, 0);
  sem_signal(*semid, 1);

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

void *consumer (void *id)
{
    // TODO

  pthread_exit (0);

}
