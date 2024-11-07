# address
```cpp
template<network::socket_domain DOMAIN>
class address: public addressBase;
```
Class representing a socket address belonging to a particular address family (`DOMAIN`).  
As addresses don't store the same data across address families, the template instantiations of this class may vary considerably from one another. Then again, as this proto-library only supports IPv4 networking so far, it doesn't matter that much at all.
# IPv4 address
```cpp
class address<socket_domain::ipv4>: public addressBase;
```
Class representing an Internet Protocol v4 address.
## Methods
### Constructors
```cpp
address<socket_domain::ipv4>::address()                                  // 1
address<socket_domain::ipv4>::address(in_port_t port, uint32_t address); // 2
```
1. Constructs an undetermined IPv4 address.
2. Constructs an IPv4 address with address number `address` and port number `port`.
### Conversion operators
```cpp
address<socket_domain::ipv4>::operator std::string() const; // 1
```
1. Convert this address to its string representation.
### Accessors
#### address
```cpp
uint32_t
address<socket_domain::ipv4>::address() const noexcept;
```
Return the IPv4 address.
#### port
```cpp
in_port_t
address<socket_domain::ipv4>::port() const noexcept;
```
Return the port.

