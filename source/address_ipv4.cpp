#include "address_ipv4.hpp"

using network::address;
using network::socket_domain;

// Basic operations

address<socket_domain::ipv4>::address(port_type port, host_type host):
	_addr {
		.sin_family = static_cast<sa_family_t>(socket_domain::ipv4),
		.sin_port = htons(port),
		.sin_addr {
			.s_addr = htonl(host),
		}	
	} {}

address<socket_domain::ipv4>::address(char const* str, port_type port):
	_addr {
		.sin_family = static_cast<sa_family_t>(socket_domain::ipv4),
		.sin_port = htons(port),
	} {
	if (::inet_aton(str, &_addr.sin_addr) == 0) {
		throw (exception("inet_aton"));
	}
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
