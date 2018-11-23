/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"

void *producer (void *id);
void *consumer (void *id);

int main (int argc, char **argv)
{

  if (argc != 5) {
    cerr << "incorrect number of command line arguments" << endl;
    return(1);
  }

  for (int i = 1; i < argc; i++) {
    if (!is_integer(argv[i])) {
      cerr << "invalid command line argument" << endl;
      return(1);
    }
  }

  int q_size = atoi(argv[1]), n_jobs = atoi(argv[2]);
  int n_producers = atoi(argv[3]), n_consumers = atoi(argv[4]);
  int param = 1;

  // Job job_array[q_size];
  //
  int mutex = 1;
  // int sem_empty = 1;
  // int sem_full = 1;

  pthread_t** producers = new pthread_t*[n_producers];
  for (int i = 0; i < n_producers; i++) {
    producers[i] = new pthread_t;
    pthread_create (producers[i], NULL, producer, (void*) &mutex);
  }

  pthread_t** consumers = new pthread_t*[n_consumers];
  for (int i = 0; i < n_consumers; i++) {
    consumers[i] = new pthread_t;
    pthread_create (consumers[i], NULL, consumer, (void*) &param);
  }

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

void *producer (void *mutex)
{

  // TODO
  int* mute = (int*) mutex;
  wait(*mute);

  cout << "entering critical region" << endl;
  cout << "exiting critical region" << endl;

  signal(*mute);

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
