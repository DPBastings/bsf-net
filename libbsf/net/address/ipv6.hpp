#ifndef BSF_NET_ADDRESS_IPV6_HPP
# define BSF_NET_ADDRESS_IPV6_HPP

# include <libbsf/net/address/address.hpp> // net::address_storage
extern "C" {
# include <arpa/inet.h> // Internet
}

namespace bsf::net {

template<>
class address<domain::ipv6>: public address_base<sockaddr_in6> {
public:
	static constexpr auto	domain = domain::ipv6;
	using host_t = in6_addr;
	using port_t = in_port_t;

	address() noexcept = default;
	address(host_t, port_t) noexcept;

	[[nodiscard]] static address	any(port_t) noexcept;

	[[nodiscard]] static std::optional<address>	from_string(char const*) noexcept;
	[[nodiscard]] std::string					to_string() const;

	[[nodiscard]] host_t	host() const noexcept;
	[[nodiscard]] port_t	port() const noexcept;
private:
	using Base = address_base<sockaddr_in6>;
}; // class address

}; // namespace bsf::net

#endif // BSF_NET_ADDRESS_IPV6_HPP