/*
 * (c) 2008-2011 Daniel Halperin <dhalperi@cs.washington.edu>
 */
#include "iwl_connector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#define MAX_PAYLOAD 2048
#define SLOW_MSG_CNT 1

int sock_fd = -1;							// the socket
FILE* out = NULL;

void check_usage(int argc, char** argv);

FILE* open_file(char* filename, char* spec);

void caught_signal(int sig);

void exit_program(int code);
void exit_program_err(int code, char* func);

int main(int argc, char** argv)
{
	/* Local variables */
	struct sockaddr_nl proc_addr, kern_addr;	// addrs for recv, send, bind
	struct cn_msg *cmsg;
	char buf[4096];
	int ret;
	unsigned short l, l2;
	int count = 0;

	/* Make sure usage is correct */
	check_usage(argc, argv);

	/* Open and check log file */
	out = open_file(argv[1], "w");

	/* Setup the socket */
	sock_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (sock_fd == -1)
		exit_program_err(-1, "socket");

	/* Initialize the address structs */
	memset(&proc_addr, 0, sizeof(struct sockaddr_nl));
	proc_addr.nl_family = AF_NETLINK;
	proc_addr.nl_pid = getpid();			// this process' PID
	proc_addr.nl_groups = CN_IDX_IWLAGN;
	memset(&kern_addr, 0, sizeof(struct sockaddr_nl));
	kern_addr.nl_family = AF_NETLINK;
	kern_addr.nl_pid = 0;					// kernel
	kern_addr.nl_groups = CN_IDX_IWLAGN;

	/* Now bind the socket */
	if (bind(sock_fd, (struct sockaddr *)&proc_addr, sizeof(struct sockaddr_nl)) == -1)
		exit_program_err(-1, "bind");

	/* And subscribe to netlink group */
	{
		int on = proc_addr.nl_groups;
		ret = setsockopt(sock_fd, 270, NETLINK_ADD_MEMBERSHIP, &on, sizeof(on));
		if (ret)
			exit_program_err(-1, "setsockopt");
	}

	/* Set up the "caught_signal" function as this program's sig handler */
	signal(SIGINT, caught_signal);

	/* Poll socket forever waiting for a message */
	while (1)
	{
		/* Receive from socket with infinite timeout */
		ret = recv(sock_fd, buf, sizeof(buf), 0);
		if (ret == -1)
			exit_program_err(-1, "recv");
		/* Pull out the message portion and print some stats */
		cmsg = NLMSG_DATA(buf);
		if (count % SLOW_MSG_CNT == 0);
			//printf("received %d bytes: id: %d val: %d seq: %d clen: %d\n", cmsg->len, cmsg->id.idx, cmsg->id.val, cmsg->seq, cmsg->len);
		/* Log the data to file */
		l = (unsigned short) cmsg->len;
		l2 = htons(l);
		fwrite(&l2, 1, sizeof(unsigned short), out);
//printf("%d,%d,%d\n", *(char*)(cmsg->data + 9), *(char*)(cmsg->data + 10), *(char*)(cmsg->data + 8));
		//printf("%d,%d,%d\n", *(char*)(cmsg->data + 11), *(char*)(cmsg->data + 12), *(char*)(cmsg->data + 13));		
		ret = fwrite(cmsg->data, 1, l, out);
		fflush(out);
		if (count % 1000 == 0)
			if (count == 0)
				printf("log starts!");
			else
				printf("%d\n", count);
			//printf("wrote %d bytes [msgcnt=%u]\n", ret, count);
		if (count == 2000)
			printf("Do It Now! Do NOT move until log finished!!!\n");
		if (count == 5000)
		{
			printf("log finished! There will be 2s before next round!\n");
			exit_program(0);
		}	
		++count;
		if (ret != l)
			exit_program_err(1, "fwrite");
	}

	exit_program(0);
	return 0;
}

void check_usage(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
		exit_program(1);
	}
}

FILE* open_file(char* filename, char* spec)
{
	FILE* fp = fopen(filename, spec);
	if (!fp)
	{
		perror("fopen");
		exit_program(1);
	}
	return fp;
}

void caught_signal(int sig)
{
	fprintf(stderr, "Caught signal %d\n", sig);
	exit_program(0);
}

void exit_program(int code)
{
	if (out)
	{
		fclose(out);
		out = NULL;
	}
	if (sock_fd != -1)
	{
		close(sock_fd);
		sock_fd = -1;
	}
	exit(code);
}

void exit_program_err(int code, char* func)
{
	perror(func);
	exit_program(code);
}
