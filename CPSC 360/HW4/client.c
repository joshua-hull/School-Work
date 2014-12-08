#include "NameServer.h"
#include "FileServer.h"
#include <stdio.h>

int main(int argc, char **argv){
	CLIENT *cl;
	char **hostname;
	char **filecontents;

	/* Check that our invocation is correct. */
	if(argc != 4){
		printf("usage: %s <NameServer> <CommonName> <file>", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Create NameServer client for RPC call. */
	cl = clnt_create(argv[1],NAMESERVER,SIMP_VERSION,"tcp");
    if(cl == NULL){
    	printf("Error createing NameServer client: %s\n", clnt_spcreateerror(argv[1]));	
    	exit(EXIT_FAILURE);
    }

    /* Get the hostname for the common name given on the command line. */
    if((hostname = getserverhostname_1(&(argv[2]),cl)) == NULL){
    	printf("Error getting server hostname: %s\n", clnt_sperror(cl,argv[1]));
    	exit(EXIT_FAILURE);
    }

    /* Create client for the FileServer using the hostname given to us from the NameServer. */
    cl = NULL;
    cl = clnt_create(*hostname,FILESERVER,SIMP_VERSION,"tcp");
    if(cl == NULL){
    	printf("Error createing FileServer client: %s\n", clnt_spcreateerror(argv[1]));	
    	exit(EXIT_FAILURE);
    }

    /* Get the file from the FileServer. */
    if((filecontents = getfile_1(&(argv[3]),cl)) == NULL){
    	printf("Error getting server hostname: %s\n", clnt_sperror(cl,argv[1]));
    	exit(EXIT_FAILURE);
    }

    /* Open the output file for writting. */
    FILE *fp = fopen(argv[3], "w");
    if (fp != NULL) {
    	/* Write the contents to the file. */
    	if(fwrite(*filecontents, sizeof(char), strlen(*filecontents), fp) != strlen(*filecontents)) {
    		perror("Error writing to file. Exiting: ");
    		exit(EXIT_FAILURE);
    	} else {
    		printf("Done writting file.\n");
    	}
    } else {
    	perror("Error opening file for writting. Exiting: ");
    	exit(EXIT_FAILURE);
    }

    /* All Done. */
    printf("File contents: %s\n", *filecontents);
	return EXIT_SUCCESS;
}