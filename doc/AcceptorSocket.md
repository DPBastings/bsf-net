# acceptor_socket
```cpp
template<bsf::net::socket_domain DOMAIN>
class bsf::net::acceptor_socket: public bsf::net::stream_socket<DOMAIN>;
```
Class representing a socket that can listen for incoming connections.
## Member types
- `address`
Inherited from `stream_socket`.
- `OptionList`
Inherited from `stream_socket`.
## Methods
### Constructors
```cpp
acceptor_socket<DOMAIN>::acceptor_socket(OptionList opts, char const* prot);                                   // 1
acceptor_socket<DOMAIN>::acceptor_socket(address const& addr, OptionList opts, char const* prot);              // 2
acceptor_socket<DOMAIN>::acceptor_socket(int backlog, address const& addr, OptionList opts, char const* prot); // 3
```
1. Opens a socket.
2. Opens a socket and binds it to `addr`.
3. Opens a socket, binds it to addr and starts listening with a backlog size of `backlog`.
### Utility
#### listen
```cpp
void
acceptor_socket<DOMAIN>::listen(int backlog) const;
```
Listen for incoming connections with a maximum queue size of `backlog`.
##### exceptions
- `socketexception` if `listen()` fails.
#### accept
```cpp
stream_socket<DOMAIN>
acceptor_socket<DOMAIN>::accept() const;                                    // 1
stream_socket<DOMAIN>
acceptor_socket<DOMAIN>::accept(stream_socket<DOMAIN>::address& addr) const; // 2
```
1. Accept an incoming connection request and create a new stream socket for it. Unless `socketOption::nonblock` was set for this acceptor, the calling thread is blocked until a connection request is received (an exception is thrown otherwise).
2. As 1, but also stores the address of the new socket in `addr`, potentially saving a call to `stream_socket<DOMAIN>::address()` later.
##### exceptions
- `socketexception` if `accept()` fails.