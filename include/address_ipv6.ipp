#ifndef NETPP_SOCKET_ADDRESS_IPV6_IPP
# define NETPP_SOCKET_ADDRESS_IPV6_IPP

# include <sstream>

namespace network {

// Conversion operators

template<typename C>
address<socket_domain::ipv6>::operator std::basic_string<C>() const {
	std::ostringstream	oss;
	host_type			hostnum = host();
	uint16_t const*		groups = reinterpret_cast<uint16_t const*>(&hostnum);

	oss.setf(std::ios::hex);
	oss << '[';
	for (size_t i = 7; i > 0; --i)
		oss << groups[i] << ':';
	oss << groups[0] << "]:" << std::to_string(port());
	return (oss.str());
}

}; // namespace network

#endif // NETPP_SOCKET_ADDRESS_IPV6_IPP