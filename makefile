CC = g++
OBJS_SERVER = server.o connection.o message.o
OBJS_CLIENT = client.o connection.o message.o
DEBUG_FLAG = -DNDEBUG
# COMP_FLAG = -std=c++20
COMP_FLAG = -std=c++20 -Wall -pedantic-errors -Werror

BUILD_PATH = build/

OBJS_SERVER_TARGET = $(addprefix $(BUILD_PATH), $(OBJS_SERVER))
OBJS_CLIENT_TARGET = $(addprefix $(BUILD_PATH), $(OBJS_CLIENT))

run: build
	build/server &
	build/client &

build: build_server build_client

build_server: $(OBJS_SERVER)
	$(CC) $(DEBUG_FLAG) $(OBJS_SERVER_TARGET) -o build/server

build_client: $(OBJS_CLIENT)
	$(CC) $(DEBUG_FLAG) $(OBJS_CLIENT_TARGET) -o build/client

server.o: src/server.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/server.cpp -o build/server.o

client.o: src/client.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/client.cpp -o build/client.o

connection.o: src/connection.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/connection.cpp -o build/connection.o

message.o: src/message.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/message.cpp -o build/message.o