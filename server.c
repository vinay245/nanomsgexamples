#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

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
node0(const char *url)
{
        int sz_date = strlen(DATE) + 1; // '\0' too
        int sock;
        int rv;

        if ((sock = nn_socket(AF_SP, NN_REP)) < 0) {
                fatal("nn_socket");
        }
          if ((rv = nn_bind(sock, url)) < 0) {
                fatal("nn_bind");
        }
        for (;;) {
                char *buf = NULL;
                int bytes;
                if ((bytes = nn_recv(sock, &buf, NN_MSG, 0)) < 0) {
                        fatal("nn_recv");
                }
                if ((bytes == (strlen(DATE) + 1)) && (strcmp(DATE, buf) == 0)) {
                        printf("NODE0: RECEIVED DATE REQUEST\n");
                        char *d = date();
                        int sz_d = strlen(d) + 1; // '\0' too
                        printf("NODE0: SENDING DATE %s\n", d);
                        if ((bytes = nn_send(sock, d, sz_d, 0)) < 0) {
                                fatal("nn_send");
                        }
                }
                nn_freemsg(buf);
        }
}

int
main(const int argc, const char **argv)
{
        return node0(PATH);

      fprintf(stderr, "Usage: reqrep %s|%s <URL> ...\n", NODE0, NODE1);
      return (1);
}
