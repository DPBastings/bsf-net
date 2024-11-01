#ifndef NETPP_ADDRESS_IPV4_HPP
# define NETPP_ADDRESS_IPV4_HPP

# include "network.hpp"
# include "./basic_address.hpp"

extern "C" {
# include <arpa/inet.h>
}

namespace network {

template<>
class address<socket_domain::ipv4>: public basic_address<address<socket_domain::ipv4>> {
public:
	using host_type = uint32_t;
	using port_type = in_port_t;

	address(port_type = 0, host_type = INADDR_ANY);

	template<typename C>
	operator std::basic_string<C>() const;

	host_type	host() const noexcept;
	port_type	port() const noexcept;

	sockaddr*		raw() noexcept;
	sockaddr const*	raw() const noexcept;
	socklen_t		size() const noexcept;

private:
	sockaddr_in	_addr;
}; // class address<socket_domain::ipv4>

}; // namespace network

# include "./address_ipv4.ipp"

#endif // NETPP_ADDRESS_IPV4_HPP
