#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>



int main(int argc, const char *argv[])
{
	int s, n;
	char *haddr;

	pid_t pid;

	struct sockaddr_in svr_addr;

	char buf[1024];
	memset(buf, 0, 1024);

	if(argc < 3){
			fprintf(stderr, "client [server_ip] [server_port]\n");
			exit(0);
	}

	haddr = argv[1];

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
			fprintf(stderr, "can't open socket\n");
			exit(0);
	}

	memset(&svr_addr, 0, sizeof(svr_addr));

	svr_addr.sin_family = PF_INET;
	svr_addr.sin_addr.s_addr = inet_addr(haddr);
	svr_addr.sin_port = htons(9006);

	if(connect(s, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0){
			fprintf(stderr, "can't connect server\n");
			exit(0);
	}


	if((pid = fork()) == 0){
			while(n = read(s, buf, 1024)){
					printf("%s", buf);
					memset(buf, 0, 1024);
			}
	} else {
			while(n = read(0, buf, 1024)){
					write(s, buf, n);
					memset(buf, 0, 1024);
			}
	}
}
