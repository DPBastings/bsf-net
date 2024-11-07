#ifndef NETPP_ADDRESS_INFO_HPP
# define NETPP_ADDRESS_INFO_HPP

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
class basic_address_info {
public:
	enum class flags {
		none = 0,
		passive = AI_PASSIVE,
		canonical_name = AI_CANONNAME,
	}; // enum class flags
	class iterator;
	using const_iterator = iterator;
	using address_type = basic_address<D>;

	basic_address_info();
	~basic_address_info();
	basic_address_info(basic_address_info const&) = delete;
	basic_address_info(basic_address_info&&);
	basic_address_info& operator=(basic_address_info const&) = delete;
	basic_address_info& operator=(basic_address_info&&);
	//constructors with numeric host / address
	basic_address_info(std::string const&);
	// basic_address_info(std::string const&, bool)
		// requires requires { D == socket_domain::ipv6; };
	basic_address_info(std::string const&, std::string const&);
	basic_address_info(std::string const&, std::string const&, bool)
		requires requires { D == socket_domain::ipv6; };
	
	iterator	begin() const noexcept;
	iterator	end() const noexcept;
	iterator	cbegin() const noexcept;
	iterator	cend() const noexcept;

	static constexpr socket_domain	domain = D;
	static constexpr socket_type	type = T;

private:
	struct hint_t {
		hint_t(int = 0);

		addrinfo	data;
	}; // struct hint_type

	addrinfo*	_raw;
}; // class basic_address_info<D, T>

template<socket_domain D, socket_type T>
class basic_address_info<D, T>::iterator {
public:
	using enclosing = basic_address_info<D, T>;

	bool	operator==(iterator) const noexcept;

	iterator&	operator++() noexcept;
	iterator	operator++(int) noexcept;

	address_type	operator*() const noexcept;
private:
	friend enclosing;
	iterator(addrinfo const*);

	addrinfo const*	_ptr;
}; // class basic_address_info<D, T>::const_iterator;

}; // namespace network

# include "./address_info.ipp"

#endif // NETPP_ADDRESS_INFO_HPP