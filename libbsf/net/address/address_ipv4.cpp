#include <libbsf/net/address/address.hpp>
#include <sstream> // std::ostringstream

using Address = bsf::net::address::address<bsf::net::domain::ipv4>;

// Basic operations

template<>
Address::address(host_t host, port_t port):
	_raw {
		.sin_family = static_cast<sa_family_t>(domain),
		.sin_port = ::htons(port),
		.sin_addr{ .s_addr = ::htonl(host) },
		.sin_zero{},
	} {}

template<>
std::optional<Address>
Address::from_string(char const* str) noexcept {
	Address	res;
	auto&	raw = res._raw;
	if (::inet_aton(str, &raw.sin_addr) == 0) return (std::nullopt);
	raw.sin_family = static_cast<sa_family_t>(domain);
	raw.sin_port = 0;
	return (res);
}

// Public methods

template<>
Address::host_t
Address::host() const noexcept {
	return (::ntohl(_raw.sin_addr.s_addr));
}

template<>
Address::port_t
Address::port() const noexcept {
	return (::ntohs(_raw.sin_port));
}

template<>
std::string
Address::to_string() const {
	std::ostringstream	oss;
	host_t const		hostnum = host();
	uint8_t const*		octets = reinterpret_cast<uint8_t const*>(&hostnum);

	for (size_t i = sizeof(host_t) - 1; i > 0; --i)
		oss << std::to_string(octets[i]) << '.';
	oss << std::to_string(octets[0]) << ':' << std::to_string(port());
	return (oss.str());
}