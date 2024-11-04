#ifndef NETPP_SOCKET_ADDRESS_LOCAL_IPP
# define NETPP_SOCKET_ADDRESS_LOCAL_IPP

# include <sstream>

namespace network {

// Conversion operators

template<typename C>
address<socket_domain::local>::operator std::basic_string<C>() const {
	return (host);
}

}; // namespace network

#endif // NETPP_SOCKET_ADDRESS_LOCAL_IPP