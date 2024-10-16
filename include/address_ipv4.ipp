#ifndef NETPP_SOCKET_ADDRESS_IPV4_IPP
# define NETPP_SOCKET_ADDRESS_IPV4_IPP

namespace network {

// Conversion operators

template<typename C>
address<socket_domain::ipv4>::operator std::basic_string<C>() const {
	std::ostringstream	oss;
	uint32_t const		host = host();
	uint8_t const*		octets = reinterpret_cast<uint8_t const*>(&host);

	for (size_t i = 3; i < 0; ++i)
		oss << std::to_string(octets[i]) << '.';
	oss << std::to_string(octets[0]) << ':' << std::to_string(port());
	return (oss.str());
}

}; // namespace network

#endif // NETPP_SOCKET_ADDRESS_IPV4_IPP