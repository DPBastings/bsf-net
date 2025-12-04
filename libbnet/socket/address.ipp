#ifndef BSF_NET_BASIC_ADDRESS_IPP
# define BSF_NET_BASIC_ADDRESS_IPP

# include <sstream>

// todo: make the _size member static constexpr for socket address

namespace bsf::net {

// Basic operations

template<socket_domain D>
template<socket_type T>
basic_address<D>::basic_address(basic_socket<D, T> const& s):
	_size(_max_size) {
	if (::getsockname(s.raw(), raw_ptr(), &_size) == -1) {
		throw (exception("getsockname"));
	}
}

template<socket_domain D>
template<socket_type T>
basic_address<D>::basic_address(basic_socket<D, T> const& s, int):
	_size(_max_size) {
	if (::getpeername(s.raw(), raw_ptr(), &_size) == -1) {
		throw (exception("getpeername"));
	}
}

// Conversions

// Class-level specializations

template<>
template<typename C>
basic_address<socket_domain::ipv4>::operator std::basic_string<C>() const {
	std::ostringstream	oss;
	host_type const		hostnum = host();
	uint8_t const*		octets = reinterpret_cast<uint8_t const*>(&hostnum);

	for (size_t i = sizeof(host_type) - 1; i > 0; --i)
		oss << std::to_string(octets[i]) << '.';
	oss << std::to_string(octets[0]) << ':' << std::to_string(port());
	return (oss.str());
}

template<>
template<typename C>
basic_address<socket_domain::ipv6>::operator std::basic_string<C>() const {
	std::ostringstream	oss;
	host_type			hostnum = host();
	uint16_t const*		groups = reinterpret_cast<uint16_t const*>(&hostnum);

	oss.setf(std::ios::hex);
	oss << '[';
	for (size_t i = sizeof(host_type) / sizeof(uint16_t) - 1; i > 0; --i)
		oss << groups[i] << ':';
	oss << groups[0] << "]:" << std::to_string(port());
	return (oss.str());
}

template<>
template<typename C>
basic_address<socket_domain::local>::operator std::basic_string<C>() const {
	return ("local socket"); // todo
}

// Accessors

template<socket_domain D>
sockaddr*
basic_address<D>::raw_ptr() noexcept {
	return (reinterpret_cast<sockaddr*>(&_raw));
}

template<socket_domain D>
sockaddr const*
basic_address<D>::raw_ptr() const noexcept {
	return (reinterpret_cast<sockaddr const*>(&_raw));
}

template<socket_domain D>
socklen_t*
basic_address<D>::size_ptr() noexcept {
	return (&_size);
}

template<socket_domain D>
socklen_t
basic_address<D>::size() const noexcept {
	if constexpr (has_dynamic_size<D>)
		return (_size);
	return (_max_size);
}

}; // namespace bsf::net

#endif // BSF_NET_BASIC_ADDRESS_IPP