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
	address(in_port_t, uint32_t = INADDR_ANY);

	template<typename C>
	operator std::basic_string<C>() const;

	uint32_t	host() const noexcept;
	in_port_t	port() const noexcept;

	sockaddr const*		raw() const noexcept;
	static socklen_t	size();

private:
	template<socket_domain D, socket_type>
	friend class socket;
	friend class acceptor_socket<socket_domain::ipv4>;

	address(int);

	sockaddr*	raw() noexcept;
	
	sockaddr_in	_addr;
}; // class address<socket_domain::ipv4>

}; // namespace network

#endif // NETPP_ADDRESS_IPV4_HPP
