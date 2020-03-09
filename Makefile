
SERVER_OBJECTS = utilities.o uksu_server.o 
CLIENT_OBJECTS = utilities.o uksu_client.o

all: clean server client
	
server: $(SERVER_OBJECTS)
	$(info ************  Building server ************)
	cc $(SERVER_OBJECTS) -o UKSU_server

client: $(CLIENT_OBJECTS)
	$(info ************  Building client ************)
	cc $(CLIENT_OBJECTS) -o UKSU_client

utilities.o: utilities.h utilities.c
	cc -c utilities.c

server.o: uksu_server.c utilities.h
	cc -c uksu_server.c

client.o: uksu_client.c utilities.h
	cc -c uksu_client.c

clean: 
	$(info ************  Cleaning project ************)
	-rm *.o 
	-rm UKSU_server
	-rm UKSU_client