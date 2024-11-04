#ifndef NETPP_SOCKET_ADDRESS_IPV6_HPP
# define NETPP_SOCKET_ADDRESS_IPV6_HPP

# include "network.hpp"
# include "./basic_address.hpp"

# include <array>

extern "C" {
# include <arpa/inet.h>
}

namespace network {

template<>
class address<socket_domain::ipv6>: public basic_address<address<socket_domain::ipv6>> {
public:
	using host_type = in6_addr;
	using port_type = in_port_t;

	address(port_type = 0, host_type = IN6ADDR_ANY_INIT);
	address(char const*);

	template<typename C>
	operator std::basic_string<C>() const;

	host_type	host() const noexcept;
	port_type	port() const noexcept;

private:
	template<socket_domain, socket_type>
	friend class socket;
	template<socket_domain, socket_type>
	friend class acceptor_socket;

	sockaddr_in6	_addr;
}; // class address<socket_domain::ipv6>

}; // namespace network

# include "./address_ipv6.ipp"

#endif // NETPP_SOCKET_ADDRESS_IPV6_HPP
