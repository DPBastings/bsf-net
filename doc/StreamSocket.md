# stream_socket
```cpp
template<network::socket_domain DOMAIN>
class network::stream_socket: public network::socket<DOMAIN, type::stream>;
```
Partial specialization of the base socket.
This is the socket type that will be created by an acceptor_socket's `accept()` method.