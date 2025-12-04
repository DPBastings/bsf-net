#ifndef BSF_NET_BASIC_SOCKET_ADDRESS_HPP
# define BSF_NET_BASIC_SOCKET_ADDRESS_HPP

# include <libbnet/domain.hpp>
# include <cstdint>
# include <string>
# include <type_traits>
extern "C" {
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/un.h>
}

namespace bsf::net::address {

template<typename Domain>
struct traits;

template<typename Domain>
concept has_port = !is_void<typename traits<D>::port_type>;

template<typename Domain>
concept has_subclass = (D == socket_domain::local);

template<typename Domain>
concept has_dynamic_size = (D == socket_domain::local);

template<>
struct traits<socket_domain::ipv4> {
	using storage_type = sockaddr_in;
	using host_type = uint32_t;
	using port_type = in_port_t;
	// using subclass_type = void_type;

	static constexpr host_type	default_host = INADDR_ANY;
}; // struct traits<socket_domain::ipv4>

template<>
struct traits<socket_domain::ipv6> {
	using storage_type = sockaddr_in6;
	using host_type = in6_addr;
	using port_type = in_port_t;
	// using subclass_type = void_type;

	static constexpr host_type	default_host = IN6ADDR_ANY_INIT;
}; // struct traits<socket_domain::ipv6>

template<>
struct traits<socket_domain::local> {
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
}; // struct traits<socket_domain::local>

template<typename Domain>
class basic_address: public traits<D> {
public:
	using typename traits<D>::storage_type;
	using typename traits<D>::host_type;
	using typename traits<D>::port_type;
	using typename traits<D>::subclass_type;

	basic_address(host_type = traits<D>::default_host)
		requires (!has_port<D>);
	basic_address(host_type = traits<D>::default_host, port_type = 0)
		requires (has_port<D>);
	basic_address(std::string const&);
	template<socket_type T>
	basic_address(basic_socket<D, T> const&);
	template<socket_type T>
	basic_address(basic_socket<D, T> const&, int);

	host_type		host() const noexcept;
	port_type		port() const noexcept
		requires (has_port<D>);
	subclass_type	subclass() const noexcept
		requires (has_subclass<D>);

	static constexpr socket_domain	domain = D;

private:
	template<socket_domain, socket_type T>
	friend class basic_socket;
	template<socket_domain, socket_type T>
	friend class basic_acceptor_socket;

	sockaddr*		raw_ptr() noexcept;
	sockaddr const*	raw_ptr() const noexcept;
	socklen_t*		size_ptr() noexcept;
	socklen_t		size() const noexcept;

	static constexpr socklen_t	_max_size = sizeof(storage_type);

	storage_type	_raw;
	socklen_t		_size;
}; // class basic_address

}; // namespace bsf::net::address

# include "./address.ipp"

#endif // BSF_NET_BASIC_SOCKET_ADDRESS_HPP
