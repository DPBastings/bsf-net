#include <libbsf/net/address/ipv6.hpp>
#include <sstream> // std::ostringstream

namespace bsf::net {

using Address = address<domain::ipv6>;

// Basic operations

Address::address(host_t host, port_t port, scope_t scope) noexcept:
	Base{
		sockaddr_in6{
			.sin6_family = static_cast<sa_family_t>(domain),
			.sin6_port = ::htons(port),
			.sin6_flowinfo = 0,
			.sin6_addr = host,
			.sin6_scope_id = scope,
		}
	} {}


Address
Address::any(port_t port, scope_t scope) noexcept {
	return (Address{ IN6ADDR_ANY_INIT, port, scope });
}

// Public methods

Address::host_t
Address::host() const noexcept {
	return (_data.sin6_addr);
}

Address::port_t
Address::port() const noexcept {
	return (::ntohs(_data.sin6_port));
}

Address::scope_t
Address::scope() const noexcept {
	return (_data.sin6_scope_id);
}

std::string
Address::to_string() const {
	std::ostringstream	oss;
	host_t				hostnum = host();
	uint16_t const*		groups = reinterpret_cast<uint16_t const*>(&hostnum);

	oss.setf(std::ios::hex);
	oss << '[';
	for (size_t i = sizeof(host_t) / sizeof(uint16_t) - 1; i > 0; --i)
		oss << groups[i] << ':';
	oss << groups[0] << "]:" << std::to_string(port());
	return (oss.str());
}

}; // namespace bsf::net