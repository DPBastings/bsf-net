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

address<socket_domain::ipv4>::host_type
address<socket_domain::ipv4>::host() const noexcept {
	return (ntohl(_addr.sin_addr.s_addr));
}

address<socket_domain::ipv4>::port_type
address<socket_domain::ipv4>::port() const noexcept {
	return (ntohs(_addr.sin_port));
}