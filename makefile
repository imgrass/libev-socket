##===============================================
# which complier
CC = gcc
DIR_ROOT = .
DIR_BIN = $(DIR_ROOT)/bin
DIR_SRC = $(DIR_ROOT)/src
CFLAGS = -g -Wall 

# model
.c.o:
	$(CC) $(CFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)
##===============================================

all: client server
	echo "mv client, server to bin"

#----------------------
# bin-client
dep_client = client.o
dep_dir_client = $(DIR_SRC)/$(dep_client)
client: $(dep_dir_client)
	$(CC) $(CFLAGS) -o $(DIR_BIN)/client $(dep_dir_client)

#----------------------
# bin-server
dep_server = server.o
dep_dir_server = $(DIR_SRC)/$(dep_server)
server: $(dep_dir_server)
	$(CC) $(CFLAGS) -o $(DIR_BIN)/server $(dep_dir_server)


client.o: $(DIR_SRC)/client.c

server.o: $(DIR_SRC)/server.c

clean:
	rm -r *.o 
