#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <sys/time.h>
#include <unistd.h>
#include "ejercicio7.h"

static tiempo time;
struct timeval t;

tiempo* reloj_1_svc(void * arg, struct svc_req *req)
{
	//hora del sistema
	gettimeofday(&t,NULL);
	
	time.sec = t.tv_sec;
	time.usec = t.tv_usec;
	
	return &time;
}

