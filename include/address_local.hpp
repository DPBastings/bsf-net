#ifndef NETPP_ADDRESS_LOCAL_HPP
# define NETPP_ADDRESS_LOCAL_HPP

# include "network.hpp"
# include "./basic_address.hpp"

# include <string_view>
# include <sys/un.h>

namespace network {

class address<socket_domain::local>: public basic_address<address<socket_domain::local>> {
public:
	using host_type = std::basic_string_view<char>;
	enum class subtype {
		path,
		anonymous,
# ifdef linux
		abstract,
# endif // linux
	}; // enum class subtype
	
	address(char const* = "");

	template<typename C>
	operator std::basic_string<C>() const;

	host_type	host() const noexcept;
	subtype		subtype() const noexcept;

	sockaddr*		raw() noexcept;
	sockaddr const*	raw() const noexcept;
	socklen_t*		size() noexcept;
	socklen_t		size() const noexcept;
private:
	template<socket_type T>
	friend class socket<socket_domain::local, T>;

	static socklen_t	_max_size = 
# ifdef linux
		108;
# endif // linux

	sockaddr_un	_addr;
	socklen_t	_size;
}; // class address_local

}; // namespace network

#endif // NETPP_ADDRESS_LOCAL_HPP