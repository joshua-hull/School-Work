CC=clang
RPC=rpcgen

CFLAGS=-g -Wall
DFLAGS=-DRPC_SVC_FG
LFLAGS=-lrpcsvc

NAME_SOURCES=$(wildcard NameServer_*.c)
NAME_OBJECTS=$(NAME_SOURCES:.c=.o)

FILE_SOURCES=$(wildcard FileServer_*.c)
FILE_OBJECTS=$(FILE_SOURCES:.c=.o)


all: client NameServer FileServer

client: NameServer.h FileServer.h
	@echo -e "--Compiling Client Object Files--"
	@make -s client.o
	@make -s NameServer_clnt.o FileServer_clnt.o NameServer_xdr.o
	@echo -e "--Linking Client Object Files--"
	@echo -n -e "\t"
	$(CC) $(CFLAGS) -o client client.o NameServer_clnt.o NameServer_xdr.o FileServer_clnt.o

NameServer: NameServer.h
	@echo -e "--Compiling NameServer Object Files--"
	@make -s NameServer_svc.o
	@echo -e "--Linking NameServer Object Files--"
	@echo -n -e "\t"
	$(CC) $(CFLAGS) $(LFLAGS) -o NameServer NameServer_Service.o NameServer_svc.o NameServer_xdr.o

FileServer: NameServer.h FileServer.h FileServer.o
	@echo -e "--Compiling FileServer Object Files--"
	@echo -e "--Linking FileServer Object Files--"
	@echo -n -e "\t"
	$(CC) $(CFLAGS) $(LFLAGS) -o FileServer FileServer.o FileServer_Service.o NameServer_clnt.o NameServer_xdr.o

NameServer.h: NameServer.x 
	@echo -e "--Running rpcgen on NameServer.x--"
	@echo -n -e "\t"
	rpcgen -C NameServer.x
	@echo -e "--Compiling NameServer Object Files--"
	@make -s $(NAME_OBJECTS)

FileServer.h: FileServer.x
	@echo -e "--Running rpcgen on FileServer.x--"
	@echo -n -e "\t"
	rpcgen -l -o FileServer_clnt.c FileServer.x
	rpcgen -h -o FileServer.h FileServer.x
	@echo -e "--Compiling FileServer Object Files--"
	@make -s $(FILE_OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

clean:
	@rm -f *.o *_clnt.c *_svc.c *_xdr.c FileServer.h NameServer.h client NameServer FileServer