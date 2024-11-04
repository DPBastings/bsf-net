#ifndef NETPP_ADDRESS_LOCAL_HPP
# define NETPP_ADDRESS_LOCAL_HPP

# include "network.hpp"
# include "./basic_address.hpp"

# include <string_view>

extern "C" {
# include <sys/un.h>
}

namespace network {

template<>
class address<socket_domain::local>: public basic_address<address<socket_domain::local>> {
public:
	using host_type = std::basic_string_view<char>;
	enum class subtype {
		path,
		anonymous,
# ifdef __linux__
		abstract,
# endif // __linux__
	}; // enum class subtype
	
	address(char const* = "");

	template<typename C>
	operator std::basic_string<C>() const;

	host_type	host() const noexcept;
	subtype		subtype() const noexcept;

private:
	template<socket_domain, socket_type>
	friend class socket;
	template<socket_domain, socket_type>
	friend class acceptor_socket;

	static constexpr socklen_t	_max_size = 
# ifdef __linux__
		108;
# else // __linux__
#  error This platform does not support local sockets.
# endif // __linux__

	sockaddr_un	_addr;
	socklen_t	_size;
}; // class address_local

}; // namespace network

# include "./address_local.ipp"

#endif // NETPP_ADDRESS_LOCAL_HPP