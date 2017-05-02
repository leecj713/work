#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

int fd_lower_forward, fd_upper_forward, fd_number_forward;
int fd_lower_reverse, fd_upper_reverse, fd_number_reverse;
time_t start, end;
char lf = '\n';

pthread_t lower_forward_thread, upper_forward_thread, number_forward_thread;
pthread_t lower_reverse_thread, upper_reverse_thread, number_reverse_thread;




int record_count = 1000;





void *lower_forward_process(void *args)
{
	int i;
	char idx;

	
	/* lower forward file */
	if((fd_lower_forward = open("./result_parallel/lower_forward", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		exit(-1);
	}

	
	for(i=0; i<record_count;i++){
		for(idx=0x61; idx<=0x7A; idx++){
			write(fd_lower_forward, &idx, 1);
		}
		write(fd_lower_forward, &lf, 1);
	}

	close(fd_lower_forward);

	printf("lower forward completed\n");

}



void *lower_reverse_process(void *args)
{
	int i;
	char idx;

	
	/* lower reverse file */
	if((fd_lower_reverse = open("./result_parallel/lower_reverse", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		exit(-1);
	}

	
	for(i=0; i<record_count;i++){
		for(idx=0x7A; idx>=0x61; idx--){
			write(fd_lower_reverse, &idx, 1);
		}
		write(fd_lower_reverse, &lf, 1);
	}

	close(fd_lower_reverse);

	printf("lower reverse completed\n");
}

void *upper_forward_process(void *args)
{
	int i;
	char idx;
	/* upper foward file */

	if((fd_upper_forward = open("./result_parallel/upper_forward", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		exit(-1);
	}
	
	for(i=0; i<record_count;i++){
		for(idx=0x41; idx<=0x5A; idx++){
			write(fd_upper_forward, &idx, 1);
		}
		write(fd_upper_forward, &lf, 1);
	}

	close(fd_upper_forward);

	printf("upper forward completed\n");
}


void *upper_reverse_process(void *args)
{
	int i;
	char idx;

	
	/* upper reverse file */
	if((fd_upper_reverse = open("./result_parallel/upper_reverse", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		exit(-1);
	}

	
	for(i=0; i<record_count;i++){
		for(idx=0x5A; idx>=0x41; idx--){
			write(fd_upper_reverse, &idx, 1);
		}
		write(fd_upper_reverse, &lf, 1);
	}

	close(fd_upper_reverse);


	printf("upper reverse completed\n");
}


void *number_forward_process(void *args)
{
	int i;
	char idx;

	/* number forward file */
	
	if((fd_number_forward = open("./result_parallel/number_forward", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		exit(-1);
	}
	
	for(i=0; i<record_count;i++){
		for(idx=0x30; idx<=0x39; idx++){
			write(fd_number_forward, &idx, 1);
		}
		write(fd_number_forward, &lf, 1);
	}

	close(fd_number_forward);


	printf("number forward completed\n");

}


void *number_reverse_process(void *args)
{
	int i;
	char idx;

	
	/* number reverse file */
	if((fd_number_reverse = open("./result_parallel/number_reverse", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		exit(-1);
	}

	
	for(i=0; i<record_count;i++){
		for(idx=0x39; idx>=0x30; idx--){
			write(fd_number_reverse, &idx, 1);
		}
		write(fd_number_reverse, &lf, 1);
	}

	close(fd_number_reverse);

	printf("number reverse completed\n");

}

int main(int argc, const char *argv[])
{
	int rc_lower_forward, rc_upper_forward, rc_number_forward;
	int rc_lower_reverse, rc_upper_reverse, rc_number_reverse;
	int status;

	if(argc >= 2){
		record_count = atoi(argv[1]);
	}


	/* record start time */
	time(&start);


	pthread_create(&lower_forward_thread, NULL, lower_forward_process, NULL);
	pthread_create(&lower_reverse_thread, NULL, lower_reverse_process, NULL);
	pthread_create(&upper_forward_thread, NULL, upper_forward_process, NULL);
	pthread_create(&upper_reverse_thread, NULL, upper_reverse_process, NULL);
	pthread_create(&number_forward_thread, NULL, number_forward_process, NULL);
	pthread_create(&number_reverse_thread, NULL, number_reverse_process, NULL);

	rc_lower_forward = pthread_join(lower_forward_thread, (void **)&status);
	rc_lower_reverse = pthread_join(lower_reverse_thread, (void **)&status);
	rc_upper_forward = pthread_join(upper_forward_thread, (void **)&status);
	rc_upper_reverse = pthread_join(upper_reverse_thread, (void **)&status);
	rc_number_forward = pthread_join(number_forward_thread, (void **)&status);
	rc_number_reverse = pthread_join(number_reverse_thread, (void **)&status);

	if(rc_lower_forward || rc_lower_reverse || rc_upper_forward || rc_upper_reverse || rc_number_forward || rc_number_reverse){
		fprintf(stderr, "return code from pthread_join() is not zero\n");
		return -1;
	}

	/* record end time */

	time(&end);


	printf("spend time : %d seconds\n", (int)(end-start));
}
