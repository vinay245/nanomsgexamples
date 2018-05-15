#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

#include <unistd.h>

#define NODE0 "node0"
#define NODE1 "node1"
#define DATE "DATE"

#define PATH "ipc:///tmp/reqrep.ipc"

void
fatal(const char *func)
{
        fprintf(stderr, "%s: %s\n", func, nn_strerror(nn_errno()));
        exit(1);
}

char *
date(void)
{
        time_t now = time(&now);
        struct tm *info = localtime(&now);
        char *text = asctime(info);
        text[strlen(text)-1] = '\0'; // remove '\n'
        return (text);
}

int
node1(const char *url)
{
        int sz_date = strlen(DATE) + 1; // '\0' too
        char *buf = NULL;
        int bytes = -1;
        int sock;
        int rv;

        if ((sock = nn_socket(AF_SP, NN_REQ)) < 0) {
                fatal("nn_socket");
        }
        if ((rv = nn_connect (sock, url)) < 0) {
                fatal("nn_connect");
        }
        for(;;)
        {
			sleep(5);
        printf("Client2: SENDING DATE REQUEST %s\n", DATE);
        if ((bytes = nn_send(sock, DATE, sz_date, 0)) < 0) {
                fatal("nn_send");
        }
        if ((bytes = nn_recv(sock, &buf, NN_MSG, 0)) < 0) {
                fatal("nn_recv");
        }
        printf("Client2: RECEIVED DATE %s\n", buf);  
        nn_freemsg(buf);
	}
        return (nn_shutdown(sock, 0));
}

int
main(const int argc, const char **argv)
{
	
                return (node1(PATH));

      fprintf(stderr, "Usage: reqrep %s|%s <URL> ...\n", NODE0, NODE1);
      return (1);
}
