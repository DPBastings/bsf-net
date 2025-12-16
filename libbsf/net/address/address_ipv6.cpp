#include <libbsf/net/address/address.hpp>
#include <sstream> // std::ostringstream

using Address = bsf::net::address::address<bsf::net::domain::ipv6>;

// Basic operations

template<>
Address::address() noexcept:
	address{ 0, 0 } {}

template<>
Address::address(host_t host, port_t port) noexcept:
	_raw {
		.sin6_family = static_cast<sa_family_t>(domain),
		.sin6_port = ::htons(port),
		.sin6_flowinfo = 0,
		.sin6_addr = host,
		.sin6_scope_id = 0,
	} {}

// Public methods

template<>
Address::port_t
Address::port() const noexcept {
	return (::ntohs(_raw.sin6_port));
}

template<>
Address::host_t
Address::host() const noexcept {
	return (_raw.sin6_addr);
}

template<>
std::string
Address::to_string() const {
	std::ostringstream	oss;
	host_t			hostnum = host();
	uint16_t const*		groups = reinterpret_cast<uint16_t const*>(&hostnum);

	oss.setf(std::ios::hex);
	oss << '[';
	for (size_t i = sizeof(host_t) / sizeof(uint16_t) - 1; i > 0; --i)
		oss << groups[i] << ':';
	oss << groups[0] << "]:" << std::to_string(port());
	return (oss.str());
}