#include "address_ipv6.hpp"

#include <iostream>
#include <sstream>

using network::address;
using network::socket_domain;

// Basic operations

address<socket_domain::ipv6>::address(port_type port, host_type host):
	_addr {
		.sin6_family = static_cast<int>(socket_domain::ipv6),
		.sin6_port = htons(port),
		.sin6_flowinfo = 0,
		.sin6_addr = host,
		.sin6_scope_id = 0,
	} {}

// Public methods

address<socket_domain::ipv6>::port_type
address<socket_domain::ipv6>::port() const noexcept {
	return (ntohs(_addr.sin6_port));
}

address<socket_domain::ipv6>::host_type
address<socket_domain::ipv6>::host() const noexcept {
	return (_addr.sin6_addr);
}

sockaddr*
address<socket_domain::ipv6>::raw() noexcept {
	return (reinterpret_cast<sockaddr*>(&_addr));
}

sockaddr const*
address<socket_domain::ipv6>::raw() const noexcept {
	return (reinterpret_cast<sockaddr const*>(&_addr));
}

socklen_t
address<socket_domain::ipv6>::size() const noexcept {
	return (sizeof(sockaddr_in6));
}
