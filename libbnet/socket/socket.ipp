#ifndef BSF_NET_SOCKET_TPP
# define BSF_NET_SOCKET_TPP

# ifndef BSF_NET_SOCKET_HPP
#  error "include in network/socket.hpp"
# endif // BSF_NET_SOCKET_HPP

extern "C" {
#include <unistd.h>
}

namespace bsf::net::socket {

template<domain Domain, type Type>
std::optional<basic_socket<Domain, Type>>
basic_socket<Domain, Type>::make(bool non_blocking, bool close_on_exec) {
	int const	opts = (non_blocking ? SOCK_NONBLOCK : 0x00)
		| (close_on_exec ? SOCK_CLOEXEC : 0x00);
	raw_type	raw = ::socket(
		static_cast<int>(D),
		static_cast<int>(T) | opts,
		0)
	;
	if (raw == -1) return (std::nullopt_t{});
	return (basic_socket{ raw});
}

template<domain Domain, type Type>
int
basic_socket<Domain, Type>::protocol() const noexcept {
	return (int_option<SO_PROTOCOL>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::address_type
basic_socket<Domain, Type>::address() const {
	return (address_type(*this));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::address_type
basic_socket<Domain, Type>::peer_address() const {
	return (address_type(*this, 0));
}

template<domain Domain, type Type>
int
basic_socket<Domain, Type>::error() const {
	return (int_option<SO_ERROR>(_raw));
}

// Configuration methods

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template bool_option<SO_REUSEADDR>
basic_socket<Domain, Type>::reuse_address() const requires is_inet<D> {
	return (bool_option<SO_REUSEADDR>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template bool_option<SO_REUSEPORT>
basic_socket<Domain, Type>::reuse_port() const requires is_inet<D> {
	return (bool_option<SO_REUSEPORT>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template bool_option<SO_DONTROUTE>
basic_socket<Domain, Type>::dont_route() const {
	return (bool_option<SO_DONTROUTE>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>:: template int_option<SO_INCOMING_CPU>
basic_socket<Domain, Type>::cpu_affinity() const {
	return (int_option<SO_INCOMING_CPU>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>:: template bool_option<SO_KEEPALIVE>
basic_socket<Domain, Type>::keep_alive() const {
	return (bool_option<SO_KEEPALIVE>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>:: template bool_option<SO_OOBINLINE>
basic_socket<Domain, Type>::inline_oob() const {
	return (bool_option<SO_OOBINLINE>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template int_option<SO_PRIORITY>
basic_socket<Domain, Type>::priority() const {
	return (int_option<SO_PRIORITY>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template int_option<SO_RCVBUF>
basic_socket<Domain, Type>::recv_buffer_size() const {
	return (int_option<SO_RCVBUF>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template int_option<SO_SNDBUF>
basic_socket<Domain, Type>::send_buffer_size() const {
	return (int_option<SO_SNDBUF>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template int_option<SO_RCVLOWAT>
basic_socket<Domain, Type>::recv_minimum() const {
	return (int_option<SO_RCVLOWAT>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template int_option<SO_SNDLOWAT>
basic_socket<Domain, Type>::send_minimum() const {
	return (int_option<SO_SNDLOWAT>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template time_option<SO_RCVTIMEO>
basic_socket<Domain, Type>::recv_timeout() const {
	return (time_option<SO_RCVTIMEO>(_raw));
}

template<domain Domain, type Type>
typename basic_socket<Domain, Type>::template time_option<SO_SNDTIMEO>
basic_socket<Domain, Type>::send_timeout() const {
	return (time_option<SO_SNDTIMEO>(_raw));
}

// I/O methods

template<domain Domain, type Type>
void
basic_socket<Domain, Type>::bind(address_type const& addr) const {
	if (::bind(_raw, addr.raw_ptr(), addr.size()) == -1) {
		throw (exception("bind"));
	}
}

template<domain Domain, type Type>
void
basic_socket<Domain, Type>::connect(address_type const& addr) const {
	if (::connect(_raw, addr.raw_ptr(), addr.size() == -1)) {
		throw (exception("connect"));
	}
}

template<domain Domain, type Type>
template<iobuf B>
typename basic_socket<Domain, Type>::streamsize
basic_socket<Domain, Type>::send(B const& buf, send_flags flags) const {
	streamsize const	len = ::send(_raw, buf.data(), buf.size(), flags);

	if (is_open() && len == -1) {
		throw (exception("send"));
	}
	return (len);
}

template<domain Domain, type Type>
template<iobuf B>
typename basic_socket<Domain, Type>::streamsize
basic_socket<Domain, Type>::recv(B& buf, recv_flags flags) const {
	streamsize const	len = ::recv(_raw, buf.data(), buf.size(), flags);

	if (is_open() && len == -1) {
		throw (exception("recv"));
	}
	buf._len = len;
	return (buf._len);
}

// Other methods

template<domain Domain, type Type>
std::pair<basic_socket<Domain, Type>,basic_socket<Domain, Type>>
basic_socket<Domain, Type>::make_pair(bool non_blocking, bool close_on_exec) requires is_pairable<D> {
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
	return (std::pair<basic_socket, basic_socket>(
		basic_socket(fds[0]), 
		basic_socket(fds[1])
	));
}

/* Socket option reference */

template<domain Domain, type Type>
template<int O, typename V>
basic_socket<Domain, Type>::option_reference<O, V>::option_reference(enclosing::raw_type raw_socket):
	_raw_socket(raw_socket) {}

template<domain Domain, type Type>
template<int O, typename V>
void
basic_socket<Domain, Type>::option_reference<O, V>::operator=(V optval) const {
	socklen_t	optlen = sizeof(V);

	if (::setsockopt(_raw_socket, SOL_SOCKET, O, &optval, &optlen) == -1) {
		throw (exception("setsockopt"));
	}
}

template<domain Domain, type Type>
template<int O, typename V>
basic_socket<Domain, Type>::option_reference<O, V>::operator V() const {
	V			optval;
	socklen_t	optlen = sizeof(V);

	if (::getsockopt(_raw_socket, SOL_SOCKET, O, &optval, &optlen) == -1) {
		throw (exception("getsockopt"));
	}
	return (optval);
}

}; // namespace bsf::net

#endif // BSF_NET_SOCKET_TPP
