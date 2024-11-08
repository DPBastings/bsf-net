#ifndef NETPP_RESOLVER_HPP
# define NETPP_RESOLVER_HPP

# include "network.hpp"

# include <concepts>
# include <string>

extern "C" {
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
}

namespace network {

template<socket_domain D, socket_type T>
class basic_resolver: public resolver_traits<D> {
public:
	struct result_type;
	class iterator;
	using const_iterator = iterator;
	using address_type = basic_address<D>;
	using resolver_traits::flag;
	// query type!

	basic_resolver();
	~basic_resolver();
	basic_resolver(basic_resolver const&) = delete;
	basic_resolver(basic_resolver&&);
	basic_resolver& operator=(basic_resolver const&) = delete;
	basic_resolver& operator=(basic_resolver&&);
	basic_resolver(std::string const&, flags);
	basic_resolver(std::string const&, flag = flag::none);
	basic_resolver(std::string const&, std::string const&);
	basic_resolver(std::string const&, std::string const&, flag = flag::none);
	
	iterator	resolve(std::string const&, std::string const&, bool = false);
	iterator	resolve_host(std::string const&, bool = false);
	iterator	resolve_port(std::string const&, bool = false);
	iterator	begin() const noexcept;
	iterator	end() const noexcept;
	iterator	cbegin() const noexcept;
	iterator	cend() const noexcept;

	static constexpr socket_domain	domain = D;
	static constexpr socket_type	type = T;

private:
	void	clear() noexcept;

	struct hint_t {
		hint_t(int = 0);

		addrinfo	data;
	}; // struct hint_type

	addrinfo*	_raw;
}; // class basic_resolver<D, T>

template<socket_domain D>
struct resolver_traits {
	enum class flag {
		none = 0;
	}; // enum class flag
}; // struct resolver_traits<D>

template<>
struct resolver_traits<socket_domain::ipv6> {
	enum class flag {
		none = 0,
		canonical_name = AI_CANONNAME,
	}; // enum class flag
}; // struct resolver_traits<socket_domain::ipv6>

template<>
struct resolver_traits<socket_domain::ipv6> {
	enum class flag {
		none = 0,
		canonical_name = AI_CANONNAME,
		map_ipv4 = AI_V4MAPPED,
	}; // enum class flag
}; // struct resolver_traits<socket_domain::ipv6>

template<socket_domain D, socket_type T>
struct basic_resolver<D, T>::result_type {
	address_type		address;
	std::string_view	canonical_name;
}; // struct basic_resolver<D, T>::result_type

template<socket_domain D, socket_type T>
class basic_resolver<D, T>::iterator {
public:
	using enclosing = basic_resolver<D, T>;

	bool	operator==(iterator) const noexcept;

	iterator&	operator++() noexcept;
	iterator	operator++(int) noexcept;

	result_type	operator*() const noexcept;
private:
	friend enclosing;
	iterator(addrinfo const*);

	addrinfo const*	_ptr;
}; // class basic_resolver<D, T>::const_iterator;

template<socket_domain D, socket_type T>
basic_resolver<D, T>::flag	operator|(basic_resolver<D, T>::flag, basic_resolver<D, T>::flag);

}; // namespace network

# include "./resolver.ipp"

#endif // NETPP_RESOLVER_HPP