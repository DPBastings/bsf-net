#ifndef BSF_NET_ADDRESS_IPV4_HPP
# define BSF_NET_ADDRESS_IPV4_HPP

# include <libbsf/net/address/address.hpp> // net::address_storage
extern "C" {
# include <arpa/inet.h> // Internet
}

namespace bsf::net {

template<>
class address<domain::ipv4>: public address_base<sockaddr_in> {
public:
	static constexpr auto	domain = domain::ipv4;
	using host_t = uint32_t;
	using port_t = in_port_t;

	address() noexcept = default;
	address(host_t, port_t) noexcept;

	/// @brief Create an address with a wildcard host name.
	[[nodiscard]] static address	any(port_t) noexcept;

	/// @brief Create an address from its string representation.
	[[nodiscard]] static std::optional<address>	from_string(char const*) noexcept;
	/// @brief Get this address' string representation.
	[[nodiscard]] std::string					to_string() const;

	/// @brief Get this address' hostname component.
	[[nodiscard]] host_t	host() const noexcept;
	/// @brief Get this address' port component.
	[[nodiscard]] port_t	port() const noexcept;
private:
	using Base = address_base<sockaddr_in>;
}; // class address

}; // namespace bsf::net

#endif // BSF_NET_ADDRESS_IPV4_HPP