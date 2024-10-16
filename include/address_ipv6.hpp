#ifndef NETPP_SOCKET_ADDRESS_IPV6_HPP
# define NETPP_SOCKET_ADDRESS_IPV6_HPP

# include "network.hpp"
# include "./basic_address.hpp"

extern "C" {
# include <arpa/inet.h>
}

namespace network {

template<>
class address<socket_domain::ipv6>: public basic_address<address<socket_domain::ipv6>> {
public:
	address(in_port_t, uint32_t = INADDR_ANY);

	template<typename C>
	operator std::basic_string<C>() const;

	uint32_t	host() const noexcept;
	in_port_t	port() const noexcept;

	sockaddr const*	pointer() const noexcept;
	sockaddr*		pointer() noexcept;
	socklen_t		size() const noexcept;

private:
	sockaddr_in6	_addr;
}; // class address<socket_domain::ipv6>

}; // namespace network

#endif // NETPP_SOCKET_ADDRESS_IPV6_HPP
