# stream_socket
```cpp
template<bsf::net::socket_domain DOMAIN>
class bsf::net::stream_socket: public bsf::net::socket<DOMAIN, type::stream>;
```
Partial specialization of the base socket.
This is the socket type that will be created by an acceptor_socket's `accept()` method.