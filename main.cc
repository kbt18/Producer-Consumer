/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"

void *producer (void *id);
void *consumer (void *id);

int main (int argc, char **argv)
{
  int q_size, n_jobs, n_producers, n_consumers;
  //check command line input is valid (numeric integers)

  // q_size = (int)*(argv + 1);
  // n_jobs = (int)*(argv + 2);
  // n_producers = (int)*(argv + 3);
  // n_consumers = (int)*(argv + 4);

  /* ~~~ EXAMPLE THREAD ~~~ */
  int parameter = 5

  pthread_t producerid;

  pthread_create (&producerid, NULL, producer, (void *) &parameter);

  pthread_join (producerid, NULL);

  return 0;
}

void *producer (void *parameter)
{

  // TODO

  int *param = (int *) parameter;

  cout << "Parameter = " << *param << endl;

  pthread_exit(0);
}

void *consumer (void *id)
{
    // TODO

  pthread_exit (0);

}
