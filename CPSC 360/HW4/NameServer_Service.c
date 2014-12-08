#include <stdio.h>
#include <string.h>
#include "NameServer.h"

int *registerserver_1_svc(register_operand *argp, struct svc_req *rqstp) {
	static int result = 0;

	printf("Got a connection: %s %s\n",argp->name, argp->hostname);

	FILE *fp = fopen("config","a");
	if(fp == NULL){
		perror("Error opening config file: ");
		fclose(fp);
		return (&result);
	}
	
	if(fprintf(fp, "%s %s\n",argp->name,argp->hostname) < 0){
		perror("Error writing to config file: ");
		fclose(fp);
		return(&result);
	}

	fclose(fp);
	result = 1;
	return (&result);
}

int *removeserver_1_svc(char **argp, struct svc_req *rqstp){
	static int result = 0;

	if(rename("config","config_tmp") < 0){
		perror("Error renaming config file: ");
		return (&result);
	}

	FILE *fp = fopen("config","a");
	FILE *old = fopen("config_tmp","r");
	if(fp == NULL || old == NULL){
		perror("Error opening config file: ");
		if(rename("config_tmp","config") < 0) perror("Error restoring config file: ");
		fclose(fp);
		fclose(old);
		return (&result);
	}	

	char name[80];
	char hostname[80];
	while(fscanf(old,"%s %s\n",name,hostname) != EOF){
		printf("name: %s hostname: %s\n",name,hostname);
		if(strcmp(name,*argp) != 0){
			if(fprintf(fp, "%s %s\n",name,hostname) < 0){
				perror("Error writing to config file: ");
				return(&result);
			}
		}
	}
	result = 1;
	return (&result);
}

char **getserverhostname_1_svc(char **argp, struct svc_req *rqstp){
	static char *result = "";

	printf("Opening config file.\n");
	FILE *fp = fopen("config","r");
	if(fp == NULL){
		perror("Error opening config file: ");
		fclose(fp);
		return (&result);
	}

	char name[80];
	char hostname[80];
	printf("Searching config file.\n");
	while(fscanf(fp,"%s %s\n",name,hostname) != EOF){
		printf("name: %s hostname: %s\n",name,hostname);
		if(strcmp(name,*argp) == 0){
			printf("\t^^match!^^\n");
			result = strdup(hostname);
			return(&result);
		}
	}
	printf("~~no match~~\n");
	return (&result);
}