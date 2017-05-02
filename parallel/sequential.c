#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


int fd_lower_forward, fd_upper_forward, fd_number_forward;
int fd_lower_reverse, fd_upper_reverse, fd_number_reverse;
time_t start, end;
char lf = '\n';

int record_count = 1000;


int main(int argc, const char *argv[])
{
	int i;
	char idx;

	if(argc >= 2){
		record_count = atoi(argv[1]);
	}

	/* record start time */
	time(&start);

	/* lower case forward file */
	if((fd_lower_forward = open("./result_sequential/lower_foward", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		return -1;
	}
	
	for(i=0; i<record_count;i++){
		for(idx=0x61; idx<=0x7A; idx++){
			write(fd_lower_forward, &idx, 1);
		}
		write(fd_lower_forward, &lf, 1);
	}

	close(fd_lower_forward);

	printf("lower forward completed\n");

	/* lower case reverse file */
	if((fd_lower_reverse = open("./result_sequential/lower_reverse", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		return -1;
	}
	
	for(i=0; i<record_count;i++){
		for(idx=0x7A; idx>=0x61; idx--){
			write(fd_lower_reverse, &idx, 1);
		}
		write(fd_lower_reverse, &lf, 1);
	}

	close(fd_lower_reverse);

	printf("lower reverse completed\n");

	/* upper case forward file */

	if((fd_upper_forward = open("./result_sequential/upper_foward", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		return -1;
	}
	
	for(i=0; i<record_count;i++){
		for(idx=0x41; idx<=0x5A; idx++){
			write(fd_upper_forward, &idx, 1);
		}
		write(fd_lower_forward, &lf, 1);
	}

	close(fd_upper_forward);

	
	printf("upper forward completed\n");
	
	/* upper case reverse file */
	if((fd_upper_reverse = open("./result_sequential/upper_reverse", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		return -1;
	}
	
	for(i=0; i<record_count;i++){
		for(idx=0x5A; idx>=0x41; idx--){
			write(fd_upper_reverse, &idx, 1);
		}
		write(fd_upper_reverse, &lf, 1);
	}

	close(fd_upper_reverse);

	printf("upper reverse completed\n");
	
	/* number forward file */
	
	if((fd_number_forward = open("./result_sequential/number_foward", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		return -1;
	}
	
	for(i=0; i<record_count;i++){
		for(idx=0x30; idx<=0x39; idx++){
			write(fd_number_forward, &idx, 1);
		}
		write(fd_lower_forward, &lf, 1);
	}

	close(fd_number_forward);
	
	printf("number forward completed\n");

	/*  number reverse file */
	if((fd_number_reverse = open("./result_sequential/number_reverse", O_WRONLY|O_CREAT|O_TRUNC|O_SYNC, 0644)) < 0)
	{
		fprintf(stderr, "cannot open file\n");
		return -1;
	}
	
	for(i=0; i<record_count;i++){
		for(idx=0x39; idx>=0x30; idx--){
			write(fd_number_reverse, &idx, 1);
		}
		write(fd_number_reverse, &lf, 1);
	}

	close(fd_number_reverse);
	
	
	printf("number reverse completed\n");

	/* record end time */

	time(&end);


	printf("spend time : %d seconds\n", (int)(end-start));
}
