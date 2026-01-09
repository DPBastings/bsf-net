#include <libbsf/net/address/ipv4.hpp>
#include <sstream> // std::ostringstream

namespace bsf::net {

using Address = address<domain::ipv4>;

// Basic operations

Address::address(host_t host, port_t port) noexcept:
	Base{
		sockaddr_in{
			.sin_family = static_cast<sa_family_t>(domain),
			.sin_port = ::htons(port),
			.sin_addr{ .s_addr = ::htonl(host) },
			.sin_zero{},
		}
	} {}

std::optional<Address>
Address::from_string(char const* str) noexcept {
	Address	res;
	auto&	raw = res._data;
	if (::inet_aton(str, &raw.sin_addr) == 0) return (std::nullopt);
	raw.sin_family = static_cast<sa_family_t>(domain);
	raw.sin_port = 0;
	return (res);
}


Address
Address::any(port_t port) noexcept {
	return (Address{ INADDR_ANY, port });
}

// Public methods

Address::host_t
Address::host() const noexcept {
	return (::ntohl(_data.sin_addr.s_addr));
}

Address::port_t
Address::port() const noexcept {
	return (::ntohs(_data.sin_port));
}

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

}; // namespace bsf::net