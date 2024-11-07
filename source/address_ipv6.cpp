#include "address.hpp"

using address = network::basic_address<network::socket_domain::ipv6>;

// Basic operations

template<>
address::basic_address(host_type host, port_type port) 
	requires (network::has_port<address::domain>):
	_raw {
		.sin6_family = static_cast<sa_family_t>(domain),
		.sin6_port = htons(port),
		.sin6_flowinfo = 0,
		.sin6_addr = host,
		.sin6_scope_id = 0,
	} {}

// Public methods

template<>
address::port_type
address::port() const noexcept
	requires (network::has_port<address::domain>) {
	return (ntohs(_raw.sin6_port));
}

template<>
address::host_type
address::host() const noexcept {
	return (_raw.sin6_addr);
}