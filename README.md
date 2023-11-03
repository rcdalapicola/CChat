# CChat

This is a simple server-client chat, where multiple clients can connect to the chat, and the message sent by one of the clients is forwarded to all of the others.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
  - [Build](#build)
  - [Run](#run)
- [Remarks](#remarks)
- [Contact](#contact)

## Prerequisites

- Unix system: this project was designed for Unix systems, thus it is not supported on Windows.

## Getting Started

The first step to run the chat is to have the full repository in your machine. The repository of this project can be obtained on github.com/rcdalapicola/CChat. Once you have the repository, you the follow the Build and Run steps to get your chat application running.

### Build

You can build the project with the makefile on the root folder.

With 'make build', the compiled files and executables will be created on the "build/" folder.
With 'make' or 'make release', the "release/" folder will be created, and the two executables (one for the client, and one for the server) will be copied.

### Run

To run the chat, you should first start the server (chat-ser). The server executable can take a port as an input, which will be the port the server will be listening to. In case no port is given when calling chat-ser, the default port (8080) will be chosen.

Example:
```bash
release/chat-ser 8080
```

Once the server is running, you can start as many clients as wanted. The client should be started with the following parameters: `chat-cli <username> <server-ip>:<server-port>`. 

Example:
```bash
release/chat-cli TestUser42 127.0.0.1:8080
```

After the user is connected, you can start sending messages, and the server will forward them to the other users.

## Remarks

- Messages received from other clients will show their username and the message.
  Example: `TestUser42: Hello, world!`.

- The server forwards the incoming messages to all the clients, but the client author of the message does not display it, since the message will still be visible on the author's terminal.

    Example:
```
#Terminal from user 'Mario'
Hello, world!               #Message typed by the user
Luigi: Hi, Mario!           #Message received by the server

```

```
#Terminal from user 'Luigi'
Mario: Hello, world!        #Message received by the server
Hi, Mario!                  #Message typed by the user

```

```
#Terminal from user 'Peach'
Mario: Hello, world!        #Message received by the server
Luigi: Hi, Mario!           #Message received by the server

```

- The server will also print all the messages coming from the clients. 

    Example:
    ```
    #Terminal running the server
    Mario: Hello, world!        #Message received by client 'Mario'
    Luigi: Hi, Mario!           #Message received by client 'Luigi'

    ```

## Contact

    Rodolfo Coelho Dalapicola
    Email: rodolfo.dalapicola@gmail.com
    Github: github.com/rcdalapicola/

