#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>

//volatile int running = 1;
char *buffer = NULL;

int index = 0;


void *run(void *data)
{ms
	int id = (int)data;
	printf("Inicia thread %d\n", id);

	buffer[pointer] = 'A';
	index++;
	return 0;
}

void print_sched(int policy)
{
	int priority_min, priority_max;

	switch(policy){
		case SCHED_DEADLINE:
			printf("SCHED_DEADLINE");
			break;
		case SCHED_FIFO:
			printf("SCHED_FIFO");
			break;
		case SCHED_RR:
			printf("SCHED_RR");
			break;
		case SCHED_NORMAL:
			printf("SCHED_OTHER");
			break;
		case SCHED_BATCH:
			printf("SCHED_BATCH");
			break;
		case SCHED_IDLE:
			printf("SCHED_IDLE");
			break;
		default:
			printf("unknown\n");
	}
	priority_min = sched_get_priority_min(policy);
	priority_max = sched_get_priority_max(policy);
	printf(" PRI_MIN: %d PRI_MAX: %d\n", priority_min, priority_max);
}

int setpriority(pthread_t *thr, int newpolicy, int newpriority)
{
	int policy, ret;
	struct sched_param param;

	if (newpriority > sched_get_priority_max(newpolicy) || newpriority < sched_get_priority_min(newpolicy)){
		printf("Invalid priority: MIN: %d, MAX: %d", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));

		return -1;
	}

	pthread_getschedparam(*thr, &policy, &param);
	printf("current: ");
	print_sched(policy);

	param.sched_priority = newpriority;
	ret = pthread_setschedparam(*thr, newpolicy, &param);
	if (ret != 0)
		perror("perror(): ");

	pthread_getschedparam(*thr, &policy, &param);
	printf("new: ");
	print_sched(policy);

	return 0;
}

int main(int argc, char **argv)
{
	int N_THREADS = 0;
	int BUFFER_SIZE = 0;
	int POLICY_TYPE = 0;
	int POLICY_PRIORITY = 0;

	if (argc < 5) {
		printf("usage: ./%s <numero_de_threads> <tamanho_do_buffer_global_em_kilobytes> <politica> <prioridade>\n\n", argv[0]);
		return 0;
	}

	N_THREADS = atoi(argv[1]);
	BUFFER_SIZE = atoi(argv[2]);
	POLICY_TYPE = atoi(argv[3]);
	POLICY_PRIORITY = atoi(argv[4]);

	pthread_t thr[N_THREADS];
	buffer = (char*) malloc((sizeof(char)) * BUFFER_SIZE * 1024);

	printf("Inicio: %d %d %d %d\n", N_THREADS, BUFFER_SIZE, POLICY_TYPE, POLICY_PRIORITY);

	for(int i = 0; i < N_THREADS; i++) {
			pthread_create(&thr[i], NULL, run, (void*)i) ;
	}

	for(int i = 0; i < N_THREADS; i++) {
			pthread_join(thr[i], NULL);
	}
	printf("Fim\n");
	// setpriority(&thr, SCHED_FIFO, 1);
	return 0;
}
