#include "helper.h"

void *producer (void *parameter)
{
  // TODO
  Producer_parameters* params = (Producer_parameters*) parameter;
  int* semid = &(params->semid);
  int* njobs = &(params->njobs);
  int* p_id = &(params->producer_id);
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

    int index = 0;
    while (job_array[index] != NULL) //will always be a space
      index++;
    job->id = index + 1;
    job_array[index] = job;

    sem_signal(*semid, 0);
    sem_signal(*semid, 1);

    printf ("Producer(%i): job id %i duration %i\n",
      *p_id, job->id, job->duration);

    sleep(rand() % 5 + 1);
  }

  printf ("Producer (%i): No more jobs to generate.\n", *p_id);
  pthread_exit(0);
}
