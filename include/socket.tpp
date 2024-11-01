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

template<socket_domain D, socket_type T, socket_protocol P>
socket<D, T, P>::socket(bool non_blocking, bool close_on_exec):
	handle(make_handle(
		(non_blocking ? SOCK_NONBLOCK : 0) | (close_on_exec ? SOCK_CLOEXEC : 0)
		)) {}

template<socket_domain D, socket_type T, socket_protocol P>
socket<D, T, P>::socket(raw_type raw):
	handle(raw) {}

// Accessor methods

template<socket_domain D, socket_type T, socket_protocol P>
socket_domain
socket<D, T, P>::domain() const noexcept {
	return (D);
}

template<socket_domain D, socket_type T, socket_protocol P>
socket_type
socket<D, T, P>::type() const noexcept {
	return (T);
}

template<socket_domain D, socket_type T, socket_protocol P>
socket_protocol
socket<D, T, P>::protocol() const noexcept {
	return (P);
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::address_type
socket<D, T, P>::address() const {
	address_type	addr;
	socklen_t		asize = addr.size();

	if (::getsockname(_raw, addr.raw(), &asize) == -1) {
		throw (exception("getsockname"));
	}
	return (addr);
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::address_type
socket<D, T, P>::peer_address() const {
	address_type	addr;
	socklen_t		asize = addr.size();

	if (::getpeername(_raw, addr.raw(), &asize) == -1) {
		throw (exception("getpeername"));
	}
	return (addr);
}

template<socket_domain D, socket_type T, socket_protocol P>
int
socket<D, T, P>::error() const noexcept {
	return (int_option<SO_ERROR>(_raw));
}

// Configuration methods

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template bool_option<SO_REUSEADDR>
socket<D, T, P>::reuse_address() const noexcept requires is_inet<D> {
	return (bool_option<SO_REUSEADDR>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template bool_option<SO_REUSEPORT>
socket<D, T, P>::reuse_port() const noexcept requires is_inet<D> {
	return (bool_option<SO_REUSEPORT>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template bool_option<SO_DONTROUTE>
socket<D, T, P>::dont_route() const noexcept {
	return (bool_option<SO_DONTROUTE>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>:: template int_option<SO_INCOMING_CPU>
socket<D, T, P>::cpu_affinity() const noexcept {
	return (int_option<SO_INCOMING_CPU>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>:: template bool_option<SO_KEEPALIVE>
socket<D, T, P>::keep_alive() const noexcept {
	return (bool_option<SO_KEEPALIVE>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>:: template bool_option<SO_OOBINLINE>
socket<D, T, P>::inline_oob() const noexcept {
	return (bool_option<SO_OOBINLINE>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template int_option<SO_PRIORITY>
socket<D, T, P>::priority() const noexcept {
	return (int_option<SO_PRIORITY>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template int_option<SO_RCVBUF>
socket<D, T, P>::recv_buffer_size() const noexcept {
	return (int_option<SO_RCVBUF>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template int_option<SO_SNDBUF>
socket<D, T, P>::send_buffer_size() const noexcept {
	return (int_option<SO_SNDBUF>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template int_option<SO_RCVLOWAT>
socket<D, T, P>::recv_minimum() const noexcept {
	return (int_option<SO_RCVLOWAT>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template int_option<SO_SNDLOWAT>
socket<D, T, P>::send_minimum() const noexcept {
	return (int_option<SO_SNDLOWAT>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template time_option<SO_RCVTIMEO>
socket<D, T, P>::recv_timeout() const noexcept {
	return (time_option<SO_RCVTIMEO>(_raw));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::template time_option<SO_SNDTIMEO>
socket<D, T, P>::send_timeout() const noexcept {
	return (time_option<SO_SNDTIMEO>(_raw));
}

// I/O methods

template<socket_domain D, socket_type T, socket_protocol P>
void
socket<D, T, P>::bind(address_type const& addr) const {
	if (::bind(_raw, addr.raw(), addr.size()) == -1) {
		throw (exception("bind"));
	}
}

template<socket_domain D, socket_type T, socket_protocol P>
void
socket<D, T, P>::connect(address_type const& addr) const {
	if (::connect(_raw, addr.raw(), addr.size() == -1)) {
		throw (exception("connect"));
	}
}

template<socket_domain D, socket_type T, socket_protocol P>
template<typename C>
typename socket<D, T, P>::streamsize
socket<D, T, P>::send(C const& buf, send_flags flags) const {
	streamsize const	len = ::send(_raw, buf.data(), buf.size(), flags);

	if (is_open() && len == -1) {
		throw (exception("send"));
	}
	return (len);
}

template<socket_domain D, socket_type T, socket_protocol P>
template<typename C>
typename socket<D, T, P>::streamsize
socket<D, T, P>::recv(C& buf, recv_flags flags) const {
	streamsize const	len = ::recv(_raw, buf.data(), buf.size(), flags);

	if (is_open() && len == -1) {
		throw (exception("recv"));
	}
	buf._len = len;
	return (buf._len);
}

// Other methods

template<socket_domain D, socket_type T, socket_protocol P>
std::pair<socket<D, T, P>, socket<D, T, P>>
socket<D, T, P>::make_pair(bool non_blocking, bool close_on_exec) requires is_pairable<D> {
	raw_type	fds[2];

	if (::socketpair(
			static_cast<int>(D),
			static_cast<int>(T)
				| (non_blocking ? SOCK_NONBLOCK : 0)
				| (close_on_exec ? SOCK_CLOEXEC : 0),
			static_cast<int>(P),
			fds
		) == -1)
		throw (exception("socketpair"));
	return (std::pair<socket, socket>(socket(fds[0]), socket(fds[1])));
}

// Private methods

template<socket_domain D, socket_type T, socket_protocol P>
typename socket<D, T, P>::raw_type
socket<D, T, P>::make_handle(int opts) {
	raw_type	raw = ::socket(
		static_cast<int>(D),
		static_cast<int>(T) | opts,
		static_cast<int>(P))
	;
	if (raw == -1) {
		throw (exception("socket"));
	}
	return (raw);
}

// Non-member functions

int
protocol_by_name(char const* prot) noexcept {
	if (!prot || prot[0] == '\0')
		return (0);
	protoent const*	const	entry = ::getprotobyname(prot);

	return (entry->p_proto);
}

/* Socket option reference */

template<socket_domain D, socket_type T, socket_protocol P>
template<int O, typename V>
socket<D, T, P>::option_reference<O, V>::option_reference(socket::raw_type raw_socket):
	_raw_socket(raw_socket) {}

template<socket_domain D, socket_type T, socket_protocol P>
template<int O, typename V>
void
socket<D, T, P>::option_reference<O, V>::operator=(V optval) const noexcept {
	socklen_t	optlen = sizeof(V);

	if (::setsockopt(_raw_socket, SOL_SOCKET, O, &optval, &optlen) == -1) {
		throw (exception("getsockopt"));
	}
}

template<socket_domain D, socket_type T, socket_protocol P>
template<int O, typename V>
socket<D, T, P>::option_reference<O, V>::operator V() const noexcept {
	V			optval;
	socklen_t	optlen = sizeof(V);

	if (::getsockopt(_raw_socket, SOL_SOCKET, O, &optval, &optlen) == -1) {
		throw (exception("getsockopt"));
	}
	return (optval);
}

}; // namespace network

#endif // NETPP_SOCKET_TPP
