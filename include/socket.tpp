#ifndef NETPP_SOCKET_TPP
# define NETPP_SOCKET_TPP

# ifndef NETPP_SOCKET_HPP
#  error "include in network/socket.hpp"
# endif // NETPP_SOCKET_HPP

# include "utils.tpp"

extern "C" {
#include <unistd.h>
}

namespace network {

// Basic operations

template<socket_domain D, socket_type T>
socket<D, T>::socket(bool non_blocking, bool close_on_exec):
	handle(make_handle(
		(non_blocking ? SOCK_NONBLOCK : 0) | (close_on_exec ? SOCK_CLOEXEC : 0)
		)) {}

template<socket_domain D, socket_type T>
socket<D, T>::socket(raw_type raw):
	handle(raw) {}

// Accessor methods

template<socket_domain D, socket_type T>
socket_domain
socket<D, T>::domain() const noexcept {
	return (D);
}

template<socket_domain D, socket_type T>
socket_type
socket<D, T>::type() const noexcept {
	return (T);
}

template<socket_domain D, socket_type T>
int
socket<D, T>::protocol() const noexcept {
	return (int_option<SO_PROTOCOL>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::address_type
socket<D, T>::address() const {
	address_type	addr;
	socklen_t		asize = addr.size();

	if (::getsockname(_raw, addr.raw(), &asize) == -1) {
		throw (exception("getsockname"));
	}
	return (addr);
}

	// Specialization for local (UNIX) sockets
template<socket_type T>
typename socket<socket_domain::local, T>::address_type
socket<socket_domain::local, T>::address() const {
	address_type	addr;
	socklen_t&		asize = addr.size();

	if (::getsockname(_raw, addr.raw(), asize) == -1) {
		throw (exception("getsockname"));
	}
	return (addr);
}

// template<socket_domain D, socket_type T>
// typename socket<D, T>::address_type
// socket<D, T>::peer_address() const {
// 	address_type	addr;
// 	socklen_t		asize = addr.size();

// 	if (::getpeername(_raw, addr.raw(), asize) == -1) {
// 		throw (exception("getpeername"));
// 	}
// 	return (addr);
// }

// 	// Specialization for local (UNIX) sockets
// template<socket_type T>
// typename socket<socket_domain::local, T>::address_type
// socket<socket_domain::local, T>::peer_address() const {
// 	address_type	addr;
// 	socklen_t*		asize = addr.size();

// 	if (::getpeername(_raw, addr.raw(), &asize) == -1) {
// 		throw (exception("getpeername"));
// 	}
// 	return (addr);
// }

template<socket_domain D, socket_type T>
int
socket<D, T>::error() const {
	return (int_option<SO_ERROR>(_raw));
}

// Configuration methods

template<socket_domain D, socket_type T>
typename socket<D, T>::template bool_option<SO_REUSEADDR>
socket<D, T>::reuse_address() const requires is_inet<D> {
	return (bool_option<SO_REUSEADDR>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template bool_option<SO_REUSEPORT>
socket<D, T>::reuse_port() const requires is_inet<D> {
	return (bool_option<SO_REUSEPORT>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template bool_option<SO_DONTROUTE>
socket<D, T>::dont_route() const {
	return (bool_option<SO_DONTROUTE>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>:: template int_option<SO_INCOMING_CPU>
socket<D, T>::cpu_affinity() const {
	return (int_option<SO_INCOMING_CPU>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>:: template bool_option<SO_KEEPALIVE>
socket<D, T>::keep_alive() const {
	return (bool_option<SO_KEEPALIVE>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>:: template bool_option<SO_OOBINLINE>
socket<D, T>::inline_oob() const {
	return (bool_option<SO_OOBINLINE>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template int_option<SO_PRIORITY>
socket<D, T>::priority() const {
	return (int_option<SO_PRIORITY>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template int_option<SO_RCVBUF>
socket<D, T>::recv_buffer_size() const {
	return (int_option<SO_RCVBUF>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template int_option<SO_SNDBUF>
socket<D, T>::send_buffer_size() const {
	return (int_option<SO_SNDBUF>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template int_option<SO_RCVLOWAT>
socket<D, T>::recv_minimum() const {
	return (int_option<SO_RCVLOWAT>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template int_option<SO_SNDLOWAT>
socket<D, T>::send_minimum() const {
	return (int_option<SO_SNDLOWAT>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template time_option<SO_RCVTIMEO>
socket<D, T>::recv_timeout() const {
	return (time_option<SO_RCVTIMEO>(_raw));
}

template<socket_domain D, socket_type T>
typename socket<D, T>::template time_option<SO_SNDTIMEO>
socket<D, T>::send_timeout() const {
	return (time_option<SO_SNDTIMEO>(_raw));
}

// I/O methods

template<socket_domain D, socket_type T>
void
socket<D, T>::bind(address_type const& addr) const {
	if (::bind(_raw, addr.raw(), addr.size()) == -1) {
		throw (exception("bind"));
	}
}

template<socket_domain D, socket_type T>
void
socket<D, T>::connect(address_type const& addr) const {
	if (::connect(_raw, addr.raw(), addr.size() == -1)) {
		throw (exception("connect"));
	}
}

template<socket_domain D, socket_type T>
template<typename C>
typename socket<D, T>::streamsize
socket<D, T>::send(C const& buf, send_flags flags) const {
	streamsize const	len = ::send(_raw, buf.data(), buf.size(), flags);

	if (is_open() && len == -1) {
		throw (exception("send"));
	}
	return (len);
}

template<socket_domain D, socket_type T>
template<typename C>
typename socket<D, T>::streamsize
socket<D, T>::recv(C& buf, recv_flags flags) const {
	streamsize const	len = ::recv(_raw, buf.data(), buf.size(), flags);

	if (is_open() && len == -1) {
		throw (exception("recv"));
	}
	buf._len = len;
	return (buf._len);
}

// Other methods

template<socket_domain D, socket_type T>
std::pair<socket<D, T>, socket<D, T>>
socket<D, T>::make_pair(bool non_blocking, bool close_on_exec) requires is_pairable<D> {
	raw_type	fds[2];

	if (::socketpair(
			static_cast<int>(D),
			static_cast<int>(T)
				| (non_blocking ? SOCK_NONBLOCK : 0)
				| (close_on_exec ? SOCK_CLOEXEC : 0),
			0, // todo: support different protocols
			fds
		) == -1)
		throw (exception("socketpair"));
	return (std::pair<socket, socket>(socket(fds[0]), socket(fds[1])));
}

// Private methods

template<socket_domain D, socket_type T>
typename socket<D, T>::raw_type
socket<D, T>::make_handle(int opts) {
	raw_type	raw = ::socket(
		static_cast<int>(D),
		static_cast<int>(T) | opts,
		0)
	;
	if (raw == -1) {
		throw (exception("socket"));
	}
	return (raw);
}

/* Socket option reference */

template<socket_domain D, socket_type T>
template<int O, typename V>
socket<D, T>::option_reference<O, V>::option_reference(socket::raw_type raw_socket):
	_raw_socket(raw_socket) {}

template<socket_domain D, socket_type T>
template<int O, typename V>
void
socket<D, T>::option_reference<O, V>::operator=(V optval) const {
	socklen_t	optlen = sizeof(V);

	if (::setsockopt(_raw_socket, SOL_SOCKET, O, &optval, &optlen) == -1) {
		throw (exception("getsockopt"));
	}
}

template<socket_domain D, socket_type T>
template<int O, typename V>
socket<D, T>::option_reference<O, V>::operator V() const {
	V			optval;
	socklen_t	optlen = sizeof(V);

	if (::getsockopt(_raw_socket, SOL_SOCKET, O, &optval, &optlen) == -1) {
		throw (exception("getsockopt"));
	}
	return (optval);
}

}; // namespace network

#endif // NETPP_SOCKET_TPP
