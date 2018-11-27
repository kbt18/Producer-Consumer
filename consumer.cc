#include "helper.h"

void *consumer (void *parameter)
{
  // TODO
  Consumer_parameters* params = (Consumer_parameters*) parameter;
  int* semid = &(params->semid);
  Job* job;

  do{

    if (sem_timedwait(*semid, 1, 20) < 0) {
      printf ("Consumer (%i): No more jobs left.\n", params->consumer_id);
      pthread_exit(0);
    }
  sem_wait(*semid, 0);

  // remove item from buffer
  job = params->job_array_pointer[0];

  int index = 0;
  while (params->job_array_pointer[index] != NULL && index < params->q_size)
    index++;

  for (int i = 0; i < index - 1; i++)
    params->job_array_pointer[i] = params->job_array_pointer[i + 1];

  index--;
  params->job_array_pointer[index] = NULL;

  sem_signal(*semid, 0);
  sem_signal(*semid, 2);

  // consumes item
  int id = job->id;
  int duration = job->duration;
  printf ("Consumer(%i): job id %i executing sleep duration %i\n",
    params->consumer_id, id, duration);

  sleep(duration);
  printf ("Consumer(%i): job id %i completed.\n", params->consumer_id, id);
}while(true);

  pthread_exit (0);
}
