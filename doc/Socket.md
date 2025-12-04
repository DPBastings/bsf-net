# socket
```cpp
template<bsf::net::socket_domain DOMAIN, bsf::net::socket_typeYPE>
class bsf::net::socket: public handle
```
Class representing a network socket belonging to a particular `DOMAIN` and conforming to a particular `TYPE`.

## Associated types
- `enum class socket_domain`
The socket's communication socket_domain. Possible values:
	- `ipv4`;
	- `local`;
- `enum class type`
The socket's type. Possible values:
	- `stream`;
- `enum class socketOption`
Miscellaneous options to be set on the socket's file descriptor. Possible values:
	- `nonblock`;
	- `cloexec`;
- `socketexception`	exception that can be thrown when a lower-level operation on the socket descriptor fails (e.g. where C-style `socket()` would return -1 on failure, the socket will throw a `socketexception`).

## Member types
- `address`
The address type associated with this kind of socket. Equates `bsf::net::address<DOMAIN>`.
- `OptionList`
Aggregate type containing an arbitrary number of `socketOption`s.

## Methods
### Constructors
```cpp
socket<DOMAIN, TYPE>::socket(OptionList opts, char const* prot = ""); // 1
```
1. Initialize the socket descriptor by way of a call to `socket()`. Note that `prot` is passed to `getprotobyname()` to retrieve the protocol number.
##### exceptions
1. 
	- `socket::exception` if `prot` does not denote a valid protocol.
	- `socket::exception` on failure of `socket()`.
### Manipulators
#### bind
```cpp
void	socket<DOMAIN, TYPE>::bind(address const& addr);
```
Bind this socket to local address `addr`.
##### exceptions
- `socketexception` on failure of `bind()`.
#### write
```cpp
template<size_t BSIZE>
size_t	socket<DOMAIN, TYPE>::write(Buffer<BSIZE> const& buf, int flags = 0);
```
Write the contents of `buf` to this socket.
##### exceptions
- `socketexception` on failure of `send()`.
#### read
```cpp
template<size_t BSIZE>
void	socket<DOMAIN, TYPE>::read(Buffer<BSIZE>& buf, int flags = 0);
```
Read from this socket into `buf`.
##### exceptions
- `socketexception` on failure of `recv()`.
### Accessors
#### address
```cpp
socket<DOMAIN, TYPE>::address	socket<DOMAIN, TYPE>::address() const noexcept;
```
Return the address to which this socket has been bound.
##### Notes
- If this socket has not in fact been bound to a local address (i.e. no prior call to `bind`() was made), the returned value is unspecified.