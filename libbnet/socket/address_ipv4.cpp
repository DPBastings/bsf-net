#include <libbnet/socket/address.hpp>

using Address = bsf::net::basic_address<bsf::net::socket_domain::ipv4>;

// Basic operations

template<>
Address::basic_address(host_type host, port_type port) 
	requires (bsf::net::has_port<Address::domain>):
	_raw {
		.sin_family = static_cast<sa_family_t>(domain),
		.sin_port = ::htons(port),
		.sin_addr {
			.s_addr = ::htonl(host),
		}	
	} {}

template<>
Address::basic_address(std::string const& str):
	_raw {
		.sin_family = static_cast<sa_family_t>(domain),
		.sin_port = 0,
	} {
	if (::inet_aton(str.c_str(), &_raw.sin_addr) == 0) {
		throw (exception("inet_aton"));
	}
}

// Public methods

template<>
Address::host_type
Address::host() const noexcept {
	return (::ntohl(_raw.sin_addr.s_addr));
}

template<>
Address::port_type
Address::port() const noexcept 
	requires (bsf::net::has_port<Address::domain>) {
	return (::ntohs(_raw.sin_port));
}