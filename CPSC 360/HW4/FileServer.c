#include "FileServer.h"
#include "NameServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

/* This was basically written by rpcgen and placed here by me. 
 * Please don't ask me what it means.
 */
void
fileserver_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		char *getfile_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case getFile:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) getfile_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

/* Function to do FileServer RPC server setup. 
 * This was basically written by rpcgen and placed here by me. 
 * Please don't ask me what it means.
 */
void initFileServer(){
	register SVCXPRT *transp;

	pmap_unset (FILESERVER, SIMP_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, FILESERVER, SIMP_VERSION, fileserver_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (FILESERVER, SIMP_VERSION, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, FILESERVER, SIMP_VERSION, fileserver_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (FILESERVER, SIMP_VERSION, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	return;
}

int main(int argc, char **argv){

	CLIENT *cl;
	register_operand regop;

	int *result;

	pid_t childPID;
	int status;

	/* Check our invocation. */
	if(argc != 3){
		printf("usage: %s <NameServer> <CommonName>", argv[0]);
		return 1;
	}

	struct addrinfo* selves = NULL;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;

	/* Get our hostname to send to the NameServer. Report error on failure.*/
    int s = getaddrinfo("localhost", NULL, &hints, &selves);
    if (s != 0) {
        fprintf(stderr, "Error getting hostname. Exiting: %s\n", gai_strerror(s));
    	exit(EXIT_FAILURE);
    }

    /* Fill out the structure we are going to send to the NameServer. */
    regop.name = strdup(argv[2]);
    regop.hostname = strdup(selves->ai_canonname);

    /* Create NameServer client for RPC call. */
    cl = clnt_create(argv[1],NAMESERVER,SIMP_VERSION,"tcp");
    if(cl == NULL){
    	fprintf(stderr, "Error creating client: %s\n", clnt_spcreateerror(argv[1]));	
    	exit(EXIT_FAILURE);
    }

    /* Register with the NameServer. */
    if((result = registerserver_1(&regop,cl)) == NULL){
    	printf("Error reigistering. Exiting: %s\n", clnt_sperror(cl,argv[1]));
    	exit(EXIT_FAILURE);
    } else if (*result == 0){
    	printf("Error reigistering on NameServer. Exiting.");
    	exit(EXIT_FAILURE);
    }

    /* Fork into child process to handle FileServer requests. */
    childPID = fork();
    if(childPID >= 0){
    	/* child process. */
    	if(childPID == 0){
			/* Start taking FileServer requests. */
        	initFileServer();   
        }
        /* parent process. */
        else{
        	/* Wait here until we are kille. */
        	wait(&status);
        }
    } else{
    	fprintf(stderr, "Error forking FileServer server. Exiting.\n");
    	if((result = removeserver_1(&(regop.name),cl)) == NULL){
    		printf("Error unregistering: %s\n", clnt_sperror(cl,argv[1]));
    	}
    	exit(EXIT_FAILURE);
    }

   	/* If the forked process ever exits we unregister with the NameServer. */
    if((result = removeserver_1(&(regop.name),cl)) == NULL){
    	printf("Error unregistering: %s\n", clnt_sperror(cl,argv[1]));
    	exit(EXIT_FAILURE);
    } else if (*result == 0){
    	printf("Error unregistering on NameServer. Exiting.");
    	exit(EXIT_FAILURE);
    }

	return EXIT_SUCCESS;
}