CC = g++
OBJS_SERVER = serverApplication.o server.o connection.o message.o
OBJS_CLIENT = clientApplication.o client.o connection.o message.o
DEBUG_FLAG = -DNDEBUG
COMP_FLAG = -std=c++20 -Wall -pedantic-errors -Werror

BUILD_PATH = build/
RELEASE_PATH = release/

OBJS_SERVER_TARGET = $(addprefix $(BUILD_PATH), $(OBJS_SERVER))
OBJS_CLIENT_TARGET = $(addprefix $(BUILD_PATH), $(OBJS_CLIENT))


.PHONY: clean

release: build
	$(info $(shell mkdir -p $(RELEASE_PATH)))
	cp -f $(BUILD_PATH)client $(RELEASE_PATH)'chat-cli'; cp -f $(BUILD_PATH)server $(RELEASE_PATH)'chat-ser';

clean:
	rm -rf build release

build: build_server build_client

build_server: $(OBJS_SERVER)
	$(CC) $(DEBUG_FLAG) $(OBJS_SERVER_TARGET) -o build/server

build_client: $(OBJS_CLIENT)
	$(CC) $(DEBUG_FLAG) $(OBJS_CLIENT_TARGET) -o build/client

serverApplication.o: src/serverApplication.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/serverApplication.cpp -o build/serverApplication.o

clientApplication.o: src/clientApplication.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/clientApplication.cpp -o build/clientApplication.o

server.o: src/server.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/server.cpp -o build/server.o

client.o: src/client.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/client.cpp -o build/client.o

connection.o: src/connection.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/connection.cpp -o build/connection.o

message.o: src/message.cpp
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) src/message.cpp -o build/message.o

$(info $(shell mkdir -p $(BUILD_PATH)))