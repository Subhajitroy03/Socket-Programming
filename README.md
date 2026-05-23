Here is the complete, production-ready `README.md` file layout for your repository. It includes clear formatting, a logical architectural flow, and structural corrections for modern POSIX compliance.

# Linux Socket Programming Reference Guide

A structured reference guide detailing the essential system calls and network architectures required for TCP/IP socket programming in C/C++.

---

## Architecture Overview

Socket programming follows a strict structural workflow depending on whether a component operates as a Server (listening for incoming traffic) or a Client (initiating outbound traffic).

```
   SERVER                   CLIENT
   socket()                 socket()
      │                        │
   bind()                      │
      │                        │
   listen()                    │
      │                        │
   accept() <══════════════ connect()
      │                        │
   recv()   <══════════════ send()
      │                        │
   send()   ══════════════> recv()
      │                        │
   close()                  close()

```

---

## 1. Core System Calls

### 1.1 `socket(domain, type, protocol)`

Creates an un-bound network file descriptor.

* **`domain`**: Specifies the communication protocol family. `AF_INET` is used to target IPv4 internet protocols.
* **`type`**: Defines the communication semantics. `SOCK_STREAM` specifies two-way, reliable, and sequenced connection-based byte streams (TCP).
* **`protocol`**: Set to `0` to let the operating system automatically discover and bind the default protocol matched to the requested type.

```c
int sd = socket(AF_INET, SOCK_STREAM, 0);

```

---

### Intermediate Struct Initialization

Before binding a server or connecting a client, the structural endpoint address payload must be configured using `sockaddr_in`.

```c
struct sockaddr_in sad;
sad.sin_family = AF_INET;
sad.sin_port = htons(9995);                  // Converts host port to Network Byte Order
sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // Converts string IP to Network Byte Order

```

---

### 1.2 `bind(sockfd, addr, addrlen)` `[Server Only]`

Assigns a local network address and unique port number to an active socket descriptor.

* **`sockfd`**: The target socket file descriptor integer returned by the system setup call.
* **`addr`**: A pointer to a generic `sockaddr` structure containing the target interface network allocation data.
* **`addrlen`**: The absolute memory footprint size of the specific address structure in bytes (`sizeof(sad)`).

```c
if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    // Handle assignment/binding errors
}

```

---

### 1.3 `listen(sockfd, backlog)` `[Server Only]`

Puts the target socket into passive listening mode to monitor incoming connection handshakes.

* **`sockfd`**: The bound server socket file descriptor.
* **`backlog`**: The maximum length to which the internal queue of pending connections for `sockfd` may grow. A standard fallback value is `5`.

```c
listen(sd, 5);

```

---

### 1.4 `accept(sockfd, addr, addrlen)` `[Server Only]`

Extracts the first connection request on the queue of pending connections, creating and returning a distinct, newly bound data file descriptor.

* **`sockfd`**: The listening server socket identifier.
* **`addr`**: A pointer to a blank, unallocated `sockaddr_in` struct that the kernel fills with the connecting peer's remote IP and source port profile.
* **`addrlen`**: A pointer to a `socklen_t` integer tracking the precise capacity boundaries of the passed client struct.

```c
struct sockaddr_in cad;
socklen_t cadl = sizeof(cad);
int cd = accept(sd, (struct sockaddr *)&cad, &cadl);

```

---

### 1.5 `connect(sockfd, addr, addrlen)` `[Client Only]`

Establishes an active point-to-point network tunnel between a client socket descriptor and a remote host peer.

* **`sockfd`**: The local client file descriptor used to route the connection.
* **`addr`**: A structure pointer initialized with the remote server's listening port and IP string configuration.
* **`addrlen`**: The evaluated byte size profile of the structural destination address.

```c
connect(sd, (struct sockaddr *)&sad, sizeof(sad));

```

---

## 2. Data Transmission

### 2.1 `send(sockfd, buf, len, flags)`

Transmits raw byte sequences through a connected data pipeline.

* **`sockfd`**: The active connected communication data socket identifier.
* **`buf`**: A pointer referencing the application memory buffer containing the message payload data to be sent.
* **`len`**: The exact byte footprint length of the active sequence being transmitted.
* **`flags`**: Structural signal routing flags (almost always set to `0` for normal operation).

```c
char msg[] = "Hello Server";
// FIXED: Using strlen() instead of sizeof() to prevent garbage transmission
send(cd, msg, strlen(msg), 0);

```

---

### 2.2 `recv(sockfd, buf, len, flags)`

Reads incoming network stream content out of system memory buffers.

* **`sockfd`**: The connected target data descriptor to read from.
* **`buf`**: The destination character array where the kernel dumps incoming packet bytes.
* **`len`**: The absolute headroom ceiling boundary of your storage array.
* **`flags`**: Control flags (set to `0` for basic read requests).

```c
char str[1024];
// FIXED: Passing 'sizeof(str) - 1' keeps an open slot to append a safe null terminator
int bytes_received = recv(cd, str, sizeof(str) - 1, 0);
if (bytes_received > 0) {
    str[bytes_received] = '\0'; // Properly null-terminate string data
}

```

---

## 3. Resource Cleanup

### `close(sockfd)`

Closes active descriptors, invalidates the integer handle, drops routing pipelines, and returns kernel resources to the operating system.

```c
close(cd); // Close active client-server data channel
close(sd); // Close main server listening socket 

```

---
