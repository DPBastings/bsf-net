#ifndef BSF_NET_ADDRESS_HPP
# define BSF_NET_ADDRESS_HPP

# include <libbsf/net/domain.hpp> // net::domain::*
# include <libbsf/net/utility/utility.hpp> // net::void_t
# include <concepts> // std::same_as
# include <cstdint> // *int*_t
# include <optional> // std::optional
# include <string> // std::string
extern "C" {
# include <arpa/inet.h> // Internet
# include <sys/un.h> // Unix
}

namespace bsf::net::address {

template<
	typename Storage,
	typename Host,
	typename Port,
	Host DefaultHost
>
struct traits_base {
	using storage_t = Storage;
	using host_t = Host;
	using port_t = Port;

	static constexpr host_t	default_host = DefaultHost;
	static constexpr bool	has_port = !std::same_as<Port, void_t>;
}; // struct traits_base<...>

template<domain::domain D>
struct traits;

template<>
struct traits<domain::ipv4>:
	traits_base<sockaddr_in, uint32_t, in_port_t, INADDR_ANY> {};
template<>
struct traits<domain::ipv6>:
	traits_base<sockaddr_in6, in6_addr, in_port_t, IN6ADDR_ANY_INIT> {};
template<>
struct traits<domain::local>:
	traits_base<sockaddr_un, char const*, void_t, nullptr> {};

/**
 * @brief Address class.
 *
 * An address represents the identity of a communication endpoint, which is
 * unique within the network on which the communication takes place. Primarily,
 * an address is used to bind a socket (which is itself to be conceptualized as
 * a *handle* of a communication endpoint) in order to render it useable.
 * @tparam D The address' *domain*, or the *family* it belongs to. In many
 * cases, an address family directly corresponds with a particular
 * communications network (though with at least one particularly notable
 * exception: the Internet has two address families associated with it). 
 */
template<domain::domain D>
class address {
public:
	static constexpr auto	domain = D;
	using storage_t = traits<D>::storage_t;
	using host_t = traits<D>::host_t;
	using port_t = traits<D>::port_t;

	address() noexcept;
	/// @brief Construct an address from a host.
	address(host_t) noexcept
		requires (!traits<D>::has_port);
	/// @brief Construct an address from a host and a port.
	address(host_t, port_t) noexcept
		requires (traits<D>::has_port);

	/// @brief Create an address from its text representation.
	[[nodiscard]] static std::optional<address>	from_string(char const*) noexcept;
	/// @brief Convert an address to a text representation.
	[[nodiscard]] std::string					to_string() const;

	/// @brief Get the host part of the address.
	[[nodiscard]] host_t	host() const noexcept;
	/// @brief Get the port part of the address.
	[[nodiscard]] port_t	port() const noexcept;

	sockaddr*		raw_ptr() noexcept;
	sockaddr const*	raw_ptr() const noexcept;
	socklen_t*		size_ptr() noexcept;
	socklen_t		size() const noexcept;
private:
	static constexpr socklen_t	_max_size = sizeof(storage_t);

	storage_t	_raw;
	socklen_t	_size = sizeof(storage_t);
}; // class address

/// @brief Convert an address to a text representation.
template<domain::domain D>
[[nodiscard]] std::string
to_string(address<D> const&);

}; // namespace bsf::net::address

# include "./address.ipp"

#endif // BSF_NET_ADDRESS_HPP
