
SERVER_OBJECTS = utilities.o uksu_server.o socket_helper.o
CLIENT_OBJECTS = utilities.o uksu_client.o socket_helper.o
CC=gcc
CFLAGS = -O2 -Wall -pedantic 

all: clean server client
	
server: $(SERVER_OBJECTS)
	$(info ************  Building server ************)
	$(CC) $(CFLAGS) $(SERVER_OBJECTS) -o UKSU_server -lpthread

client: $(CLIENT_OBJECTS)
	$(info ************  Building client ************)
	$(CC) $(CFLAGS) $(CLIENT_OBJECTS) -o UKSU_client

utilities.o: utilities.h utilities.c
	$(CC) $(CFLAGS) -c utilities.c

server.o: uksu_server.c utilities.h socket_helper.h
	$(CC) $(CFLAGS) -c uksu_server.c

client.o: uksu_client.c utilities.h socket_helper.h
	$(CC) $(CFLAGS) -c uksu_client.c

socket_helper.o: socket_helper.c socket_helper.h
	$(CC) $(CFLAGS) -c socket_helper.c

clean: 
	$(info ************  Cleaning project ************)
	-rm *.o 
	-rm UKSU_server
	-rm UKSU_client