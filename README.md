# FT_IRC - Custom IRC Server Project

## Introduction
`ft_irc` is a programming project aimed at creating a custom IRC (Internet Relay Chat) server from scratch, using C++ 98.
This server will allow multiple clients to connect, communicate in real-time through text-based messages, either publicly or privately, and join group channels.

## Key Features
- **Real-Time Communication**: Support for multiple clients to chat in real-time.
- **Custom Server**: Built using C++ 98, adhering to fundamental programming practices.
- **Channel and Private Messaging**: Users can join channels or send private messages.
- **Connection Authentication**: Secure connections with a password.
- **User Modes and Channel Operators**: Implementation of standard IRC features.

## Technical Specifications
- **Non-Blocking I/O**: The server uses non-blocking I/O operations.
- **Polling Mechanism**: Utilizes `poll()` (or similar functions like `select()`, `kqueue()`, `epoll()`) for handling operations.
- **Stability**: Designed to be robust against crashes or unexpected quits.
- **Compiling**: Use the provided Makefile for compiling.
- **No External Libraries**: Purely built with standard C++ libraries, no external or Boost libraries used.

## Usage
To run the server:
```bash
./ircserv <port> <password>
```
`<port>`: Port number for the IRC server.  
`<password>`: Connection password for clients.

## Client Compatibility
Compatible with standard IRC clients.  
Ensure smooth operation similar to conventional IRC servers.

## Contributions and Development

This project was brought to life by a team of three 42 students !

<p align="center"><br />
<a href="http://github.com/n1kito" alt="n1kito github profile"><img src="https://github.com/n1kito.png" width="60px style="border-radius:50%"/></a>
<a href="http://github.com/vrigaudy" alt="jhparkkkk github profile"><img src="https://github.com/jhparkkkk.png" width="60px style="border-radius:50%"/></a>
<a href="http://github.com/torgaa" alt="torgaa github profile"><img src="https://github.com/torgaa.png" width="60px style="border-radius:50%"/></a>

</p>

Contributions were managed via Git, ensuring a collaborative and iterative approach to development, making good use of the `Issues` system.
