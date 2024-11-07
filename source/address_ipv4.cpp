#include "address.hpp"

using address = network::basic_address<network::socket_domain::ipv4>;

// Basic operations

template<>
address::basic_address(host_type host, port_type port) 
	requires (network::has_port<address::domain>):
	_raw {
		.sin_family = static_cast<sa_family_t>(domain),
		.sin_port = htons(port),
		.sin_addr {
			.s_addr = htonl(host),
		}	
	} {}

template<>
address::basic_address(std::string const& str):
	_raw {
		.sin_family = static_cast<sa_family_t>(domain),
		.sin_port = 0,
	} {
	if (::inet_aton(str.c_str(), &_raw.sin_addr) == 0) {
		throw (exception("inet_aton"));
	}
}

// template<>
// address::basic_address(storage_type const* raw)
// 	requires internet_domain<address::domain>:
// 	_raw {
// 		.sin_family = raw->sin_family,
// 		.sin_port = raw->sin_port,
// 		.sin_addr {
// 			.s_addr = raw->sin_addr.s_addr;
// 		}
// 	} {}

// Public methods

template<>
address::host_type
address::host() const noexcept {
	return (ntohl(_raw.sin_addr.s_addr));
}

template<>
address::port_type
address::port() const noexcept 
	requires (network::has_port<address::domain>) {
	return (ntohs(_raw.sin_port));
}