#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include "tlpi_hdr.h"

#define SERVER_FIFO "/tmp/seqnum_sv"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"
#define CLIENT_FIFO_NAME_LEN 100

struct request{
	pid_t pid;
	int seqLen;
};

struct response{
	int seqNum;
};