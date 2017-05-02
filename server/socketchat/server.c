#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


#define MAX_CLIENT	10000
#define DEFAULT_PORT	9006
#define MAX_EVENTS	10000


int g_svr_sd;
int g_svr_port;


struct {
		int cli_sd;
		char cli_ip[20];
} g_client[MAX_CLIENT];

int g_epoll_fd;

struct epoll_event g_events[MAX_EVENTS];


void init_data(void);
void init_server(int);
void epoll_init(void);
void epoll_cli_add(int);

void userpool_add(int, char *);



void init_data(void)
{
	register int i;
	for(i=0; i <MAX_CLIENT; i++){
			g_client[i].cli_sd = -1;
	}
}


void init_server(int svr_port)
{
	struct sockaddr_in svr_addr;

	if((g_svr_sd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
			fprintf(stderr, "socket : Can't open socket\n");
			exit(0);
	}

	memset(&svr_addr, 0, sizeof(svr_addr));

	svr_addr.sin_family = PF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(svr_port);

	int nSocketOpt = 1;

	if(setsockopt(g_svr_sd, SOL_SOCKET, SO_REUSEADDR, &nSocketOpt, sizeof(nSocketOpt)) < 0){
			fprintf(stderr, "setsockopt : Can't set reuse address\n");
			close(g_svr_sd);
			exit(0);
	}

	if(bind(g_svr_sd, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0){
			fprintf(stderr, "bind : Can't bind server\n");
			close(g_svr_sd);
			exit(0);
	}

	listen(g_svr_sd, 15);
}


void epoll_init(void)
{
	struct epoll_event events;

	g_epoll_fd = epoll_create(MAX_EVENTS);

	if(g_epoll_fd < 0){
			fprintf(stderr, "epoll_create : Epoll Create Failed\n");
			close(g_svr_sd);
			exit(0);
	}

	events.events = EPOLLIN;
	events.data.fd = g_svr_sd;

	if(epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, g_svr_sd, &events) < 0){
			fprintf(stderr, "epoll_ctl : epoll control failed\n");
			close(g_svr_sd);
			close(g_epoll_fd);
			exit(0);
	}


}



void epoll_cli_add(int cli_fd)
{
	struct epoll_event events;

	events.events = EPOLLIN;
	events.data.fd = cli_fd;

	if(epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, cli_fd, &events) < 0){
			fprintf(stderr, "epoll_ctl : add client failed\n");
	}
}


void userpool_add(int cli_fd, char *cli_ip)
{
	register int i;
	for(i = 0; i < MAX_CLIENT; i++){
			if(g_client[i].cli_sd == -1) break;
	}

	if(i >= MAX_CLIENT) close(cli_fd);

	g_client[i].cli_sd = cli_fd;
	memset(&g_client[i].cli_ip[0], 0, 20);
	strcpy(&g_client[i].cli_ip[0], cli_ip);
}


void userpool_delete(int cli_fd)
{
	register int i;
	for(i = 0; i < MAX_CLIENT; i++){
			if(g_client[i].cli_sd == cli_fd)
			{
				g_client[i].cli_sd = -1;
				break;
			}
	}
}


void userpool_send(char *buffer)
{
	register int i;
	int len;

	len = strlen(buffer);


	for(i = 0; i < MAX_CLIENT; i++)
	{
		if(g_client[i].cli_sd != -1){
				len = send(g_client[i].cli_sd, buffer, len, 0);
		}
	}
}



void client_recv(int event_fd)
{
	char r_buffer[1024];
	int len;

	memset(r_buffer, 0, 1024);
	len = recv(event_fd, r_buffer, 1024, 0);

	if(len < 0 || len == 0)
	{
		userpool_delete(event_fd);
		close(event_fd);
		return;
	}

	printf("%s", r_buffer);

	userpool_send(r_buffer);
}


void server_process(void)
{
	struct sockaddr_in cli_addr;
	int i, nfds;
	int cli_sd;
	int cli_len = sizeof(cli_addr);

	nfds = epoll_wait(g_epoll_fd, g_events, MAX_EVENTS, 100);
	
	if(nfds == 0) return;

	if(nfds < 0){
			fprintf(stderr, "epoll wait error\n");
			return;
	}


	for(i = 0; i < nfds; i++)
	{
			if(g_events[i].data.fd == g_svr_sd){
					cli_sd = accept(g_svr_sd, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_len);
					if(cli_sd < 0){
					} else {
							userpool_add(cli_sd, inet_ntoa(cli_addr.sin_addr));
							epoll_cli_add(cli_sd);
					}
					continue;
			}

			client_recv(g_events[i].data.fd);
	}

}


void end_server(int sig)
{
	close(g_svr_sd);
	exit(0);
}


int main(int argc, const char *argv[])
{
	if(argc < 3) g_svr_port = DEFAULT_PORT;
	else {
			if(strcmp("-p", argv[1]) == 0){
					g_svr_port = atoi(argv[2]);
					if(g_svr_port < 1024){
							fprintf(stderr, "port number invalid\n");
							exit(0);
					}
			}
	}

	init_data();

	init_server(g_svr_port);
	epoll_init();

	while(1)
	{
		server_process();
	}

}
