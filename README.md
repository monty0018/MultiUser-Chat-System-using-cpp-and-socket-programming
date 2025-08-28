# Multi-Client Chat Application

A real-time multi-client chat application built with C++ and Windows Sockets (Winsock).

## Features

- ✅ Real-time messaging between multiple clients
- ✅ Multi-threaded server handling concurrent connections
- ✅ User identification with chat names
- ✅ Clean connection/disconnection handling
- ✅ Cross-client message broadcasting

## Prerequisites

- Windows OS
- C++ Compiler (GCC/MinGW or Visual Studio)
- Winsock library (included with Windows)

## Building the Application

### Method 1: Using GCC/MinGW

```bash
# Build server
g++ -o server Server/server.cpp -lws2_32 -std=c++11

# Build client
g++ -o client Client/client.cpp -lws2_32 -std=c++11
