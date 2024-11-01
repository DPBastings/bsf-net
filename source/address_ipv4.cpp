#include "address_ipv4.hpp"

#include <iostream>
#include <sstream>

using network::address;
using network::socket_domain;

// Basic operations

address<socket_domain::ipv4>::address(in_port_t port, uint32_t address) {
	_addr.sin_family = static_cast<int>(socket_domain::ipv4);
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = htonl(address);
}

// Public methods

address<socket_domain::ipv4>::port_type
address<socket_domain::ipv4>::port() const noexcept {
	return (ntohs(_addr.sin_port));
}

address<socket_domain::ipv4>::host_type
address<socket_domain::ipv4>::host() const noexcept {
	return (ntohl(_addr.sin_addr.s_addr));
}

sockaddr*
address<socket_domain::ipv4>::raw() noexcept {
	return (reinterpret_cast<sockaddr*>(&_addr));
}

sockaddr const*
address<socket_domain::ipv4>::raw() const noexcept {
	return (reinterpret_cast<sockaddr const*>(&_addr));
}

socklen_t
address<socket_domain::ipv4>::size() const noexcept {
	return (sizeof(sockaddr_in));
}
