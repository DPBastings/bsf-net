#ifndef NETPP_SOCKET_TPP
# define NETPP_SOCKET_TPP

# ifndef NETPP_SOCKET_HPP
#  error "include in network/socket.hpp"
# endif // NETPP_SOCKET_HPP

# include "utils.tpp"

extern "C" {
#include <netdb.h>
#include <unistd.h>
}

namespace network {

// Basic operations

template<socket_domain D, socket_type T>
socket<D, T>::socket(raw_type raw):
	handle(raw) {}

template<socket_domain D, socket_type T>
socket<D, T>::socket(option opts, char const* prot):
	handle(make_handle(opts, prot)) {}

template<socket_domain D, socket_type T>
socket<D, T>::socket(address const& addr, option opts, char const* prot):
	handle(make_handle(opts, prot)) {
	bind(addr);
}

// Accessor methods

template<socket_domain D, socket_type T>
typename socket<D, T>::address
socket<D, T>::address() const noexcept {
	return address(_raw);
}

// Networking methods

template<socket_domain D, socket_type T>
void
socket<D, T>::bind(address const& addr) {
	if (::bind(_raw, addr._raw, addr.size()) == -1) {
		throw (socketexception("bind"));
	}
}

// I/O methods

template<socket_domain D, socket_type T>
template<size_t BSIZE>
socket<D, T>::streamsize
socket<D, T>::write(Buffer<BSIZE> const& buf, int flags) const {
	streamsize const	len = ::send(raw(), buf.data(), buf._len, flags);

	if (is_open() && len == -1) {
		throw (socketexception("send"));
	}
	return (len);
}

template<socket_domain D, socket_type T>
template<size_t BSIZE>
socket<D, T>::streamsize
socket<D, T>::read(Buffer<BSIZE>& buf, int flags) const {
	streamsize const	len = ::recv(raw(), buf.data(), BSIZE, flags);

	if (is_open() && len == -1) {
		throw (socketexception("recv"));
	}
	buf._len = len;
	return (buf._len);
}

// Private and protected methods

template<socket_domain D, socket_type T>
typename socket<D, T>::raw_type
socket<D, T>::make_handle(option opts, char const* prot) noexcept {
	return (::socket(
		static_cast<int>(D),
		static_cast<int>(T) | static_cast<int>(opts),
		get_protocol(prot))
	);
}

template<socket_domain D, socket_type T>
int
socket<D, T>::get_protocol(char const* prot) noexcept {
	if (!prot || prot[0] == '\0')
		return (0);
	protoent const*	const	entry = ::getprotobyname(prot);

	//if (entry == nullptr)
	//	throw (socketexception("getprotobyname"));
	return (entry->p_proto);
}

// Non-member functions

template<socket_domain D, socket_type T>
constexpr socket<D, T>::option
operator|(socket<D, T>::option lhs, socket<D, T>::option rhs) {
	return (static_cast<socket<D, T>::option>(
		static_cast<int>(lhs) | static_cast<int>(rhs)
	));
}

}; // namespace network

#endif // NETPP_SOCKET_TPP
