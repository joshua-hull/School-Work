#include <stdio.h>
#include "FileServer.h"

/* FileServer service. Reads in contents of the requested file and returns it. */
char **getfile_1_svc(char **argp, struct svc_req *rqstp){
	static char *result = "";
	char *filename = *argp;
	long numbytes;

	/* Open file for reading. */
	FILE *fp = fopen(filename,"r");

	if (fp != NULL){
		/* Find out the size of the file. */
		fseek(fp,0L,SEEK_END);
		numbytes = ftell(fp);

		/* Rewind the tape. */
		fseek(fp,0L,SEEK_SET);

		/* Request memory to put the contents in. */
		result = (char *)calloc(numbytes,sizeof(char));
		if(result == NULL) {
			fprintf(stderr,"Error allocating memory for file. Returning empty string.\n");
		}
		else fread(result, sizeof(char),numbytes,fp);
		fclose(fp);
	} else printf("Error opening file or file does not exist. Returning empty string.\n");

	/* Return the contents. Defaults to an empty string on any kind of an error. */
	printf("File contents: %s\n", result);
	return (&result);
}