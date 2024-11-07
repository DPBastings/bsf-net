#ifndef NETPP_ADDRESS_HPP
# define NETPP_ADDRESS_HPP

# include "network.hpp"

# include <concepts>
# include <stdint.h>
# include <string>
# include <type_traits>

extern "C" {
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/un.h>
}

namespace network {

struct void_type { };

template<socket_domain D>
struct address_traits {
	using storage_type = void_type;
	using host_type = void_type;
	using port_type = void_type;
	using subtype = void_type;
}; // struct address_traits

template<typename T>
concept is_void = std::is_same_v<T, void_type>;

template<socket_domain D>
concept has_port = !is_void<typename address_traits<D>::port_type>;

template<socket_domain D>
concept has_subclass = (D == socket_domain::local);

template<socket_domain D>
concept has_dynamic_size = (D == socket_domain::local);

template<socket_domain D>
class basic_address: public address_traits<D> {
public:
	using typename address_traits<D>::storage_type;
	using typename address_traits<D>::host_type;
	using typename address_traits<D>::port_type;
	using typename address_traits<D>::subclass_type;

	basic_address(host_type = address_traits<D>::default_host)
		requires (!has_port<D>);
	basic_address(host_type = address_traits<D>::default_host, port_type = 0)
		requires (has_port<D>);
	basic_address(std::string const&);
	template<socket_type T>
	basic_address(basic_socket<D, T> const&);
	template<socket_type T>
	basic_address(basic_socket<D, T> const&, int);

	template<typename C>
	operator std::basic_string<C>() const;

	host_type		host() const noexcept;
	port_type		port() const noexcept
		requires (has_port<D>);
	subclass_type	subclass() const noexcept
		requires (has_subclass<D>);

	static constexpr socket_domain	domain = D;

private:
	template<socket_domain, socket_type T>
	friend class basic_address_info;
	template<socket_domain, socket_type T>
	friend class basic_socket;
	template<socket_domain, socket_type T>
	friend class basic_acceptor_socket;

	basic_address(storage_type const*)
		requires internet_domain<D>;

	sockaddr*		raw_ptr() noexcept;
	sockaddr const*	raw_ptr() const noexcept;
	socklen_t*		size_ptr() noexcept;
	socklen_t		size() const noexcept;

	static constexpr socklen_t	_max_size = sizeof(storage_type);

	storage_type	_raw;
	socklen_t		_size;
}; // class basic_address

template<>
struct address_traits<socket_domain::ipv4> {
	using storage_type = sockaddr_in;
	using host_type = uint32_t;
	using port_type = in_port_t;
	using subclass_type = void_type;

	static constexpr host_type	default_host = INADDR_ANY;
	static constexpr host_type	loopback_host = INADDR_LOOPBACK;
}; // struct address_traits<socket_domain::ipv4>

template<>
struct address_traits<socket_domain::ipv6> {
	using storage_type = sockaddr_in6;
	using host_type = in6_addr;
	using port_type = in_port_t;
	using subclass_type = void_type;

	static constexpr host_type	default_host = IN6ADDR_ANY_INIT;
	static constexpr host_type	loopback_host = IN6ADDR_LOOPBACK_INIT;
}; // struct address_traits<socket_domain::ipv6>

template<>
struct address_traits<socket_domain::local> {
	using storage_type = sockaddr_un;
	using host_type = std::string_view;
	using port_type = void_type;
	enum class subclass_type {
		path,
		anonymous,
# ifdef __linux__
		abstract,
# endif // __linux__
	}; // enum class subclass_type

	static constexpr host_type	default_host = "";
}; // struct address_traits<socket_domain::local>

}; // namespace network

# include "./address.ipp"

#endif // NETPP_ADDRESS_HPP
