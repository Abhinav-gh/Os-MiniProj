CC = cc
# CFLAGS = -Wall -Wextra # Gives all possible warnings. 
SERVER_EXEC = server_exec
CLIENT_EXEC = client_exec

SERVER_SRC = Server/server.c Server/auth.c Functionalites/funcHandlerMain.c 
CLIENT_SRC = Client/client.c Client/clientMenu.c

all: server client

server: $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER_EXEC)

client: $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT_EXEC)

clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)
