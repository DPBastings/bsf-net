#ifndef NETPP_BASIC_SOCKET_ADDRESS_HPP
# define NETPP_BASIC_SOCKET_ADDRESS_HPP

# include "network.hpp"

extern "C" {
# include <sys/socket.h>
}

namespace network {

template<typename T>
class basic_address {
public:
	template<typename C>
	operator std::basic_string<C>() const {
		return (std::basic_string<C>(static_cast<T const&>(*this)));
	};

	auto	host() const noexcept {
		return (static_cast<T const*>(this)->host());
	};
}; // class basic_address

}; // namespace network

#endif // NETPP_BASIC_SOCKET_ADDRESS_HPP
