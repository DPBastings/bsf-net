#ifndef BSF_NET_SOCKET_SOCKET_BASE_IPP
# define BSF_NET_SOCKET_SOCKET_BASE_IPP

# include <libbsf/net/utility/except.hpp>
extern "C" {
# include <unistd.h>
}

namespace bsf::net::socket {

/// @brief Create a configured socket.
template<domain::domain D, type::type T>
std::optional<socket_base<D, T>>
socket_base<D, T>::make(config conf) {
	raw_t	raw = ::socket(
		static_cast<int>(D),
		static_cast<int>(T) | conf.to_bitmask(),
		0)
	;
	if (raw == -1) return (std::nullopt);
	return (socket_base{ raw });
}

/// @brief Create a pair of local sockets.
template<domain::domain D, type::type T>
std::pair<socket_base<D, T>,socket_base<D, T>>
socket_base<D, T>::make_pair(config conf)
	requires (D == domain::local) {
	raw_t	fds[2];

	if (::socketpair(
		static_cast<int>(D),
		static_cast<int>(T) | conf.to_bitmask(),
		0, // ***TODO*** Support different protocols.
		fds
	) == -1) throw (exception("socketpair"));
	return (std::make_pair(
		socket_base(fds[0]),
		socket_base(fds[1])
	));
}


template<domain::domain D, type::type T>
constexpr auto
socket_base<D, T>::domain() noexcept {
	return (D);
}

template<domain::domain D, type::type T>
constexpr auto
socket_base<D, T>::type() noexcept {
	return (T);
}

template<domain::domain D, type::type T>
int
socket_base<D, T>::protocol() const {
	return (int_option<SO_PROTOCOL>(_raw));
}



template<domain::domain D, type::type T>
typename socket_base<D, T>::address_type
socket_base<D, T>::address() const {
	return (address_type::from_socket(*this));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::address_type
socket_base<D, T>::peer_address() const {
	return (address_type::from_peer(*this));
}

template<domain::domain D, type::type T>
int
socket_base<D, T>::error() const {
	return (int_option<SO_ERROR>(_raw));
}

// Configuration methods

template<domain::domain D, type::type T>
typename socket_base<D, T>::template bool_option<SO_REUSEADDR>
socket_base<D, T>::reuse_address() const
	requires (domain::traits<D>::is_inet) {
	return (bool_option<SO_REUSEADDR>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template bool_option<SO_REUSEPORT>
socket_base<D, T>::reuse_port() const
	requires (domain::traits<D>::is_inet) {
	return (bool_option<SO_REUSEPORT>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template bool_option<SO_DONTROUTE>
socket_base<D, T>::dont_route() const {
	return (bool_option<SO_DONTROUTE>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>:: template int_option<SO_INCOMING_CPU>
socket_base<D, T>::cpu_affinity() const {
	return (int_option<SO_INCOMING_CPU>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>:: template bool_option<SO_KEEPALIVE>
socket_base<D, T>::keep_alive() const {
	return (bool_option<SO_KEEPALIVE>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>:: template bool_option<SO_OOBINLINE>
socket_base<D, T>::inline_oob() const {
	return (bool_option<SO_OOBINLINE>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template int_option<SO_PRIORITY>
socket_base<D, T>::priority() const {
	return (int_option<SO_PRIORITY>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template int_option<SO_RCVBUF>
socket_base<D, T>::recv_buffer_size() const {
	return (int_option<SO_RCVBUF>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template int_option<SO_SNDBUF>
socket_base<D, T>::send_buffer_size() const {
	return (int_option<SO_SNDBUF>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template int_option<SO_RCVLOWAT>
socket_base<D, T>::recv_minimum() const {
	return (int_option<SO_RCVLOWAT>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template int_option<SO_SNDLOWAT>
socket_base<D, T>::send_minimum() const {
	return (int_option<SO_SNDLOWAT>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template time_option<SO_RCVTIMEO>
socket_base<D, T>::recv_timeout() const {
	return (time_option<SO_RCVTIMEO>(_raw));
}

template<domain::domain D, type::type T>
typename socket_base<D, T>::template time_option<SO_SNDTIMEO>
socket_base<D, T>::send_timeout() const {
	return (time_option<SO_SNDTIMEO>(_raw));
}

// Peers

template<domain::domain D, type::type T>
void
socket_base<D, T>::bind(address_type const& addr) const {
	if (::bind(_raw, addr.raw_ptr(), addr.size()) == -1) {
		throw (exception("bind"));
	}
}

template<domain::domain D, type::type T>
void
socket_base<D, T>::connect(address_type const& addr) const {
	if (::connect(_raw, addr.raw_ptr(), addr.size() == -1)) {
		throw (exception("connect"));
	}
}

// I/O operations

template<domain::domain D, type::type T>
recv_result
socket_base<D, T>::recv(void* data, std::size_t length) const {
	return (recv(data, length, recv_flag::none))
}

template<domain::domain D, type::type T>
recv_result
socket_base<D, T>::recv(void* data, std::size_t length, recv_flag flag) const {
	ssize_t const	len = ::recv(_raw, buf.data(), buf.size(), flag);

	if (len == -1) return (bsf::unexpected(get_recv_error()));
	buf._len = len;
	return (buf._len);
}

template<domain::domain D, type::type T>
send_result
socket_base<D, T>::send(void const* data, std::size_t length) const {
	return (send(data, length, send_flag::none));
}


template<domain::domain D, type::type T>
send_result
socket_base<D, T>::send(void const* data, std::size_t length, send_flag flag) const {
	ssize_t const	len = ::send(_raw, buf.data(), buf.size(), flag);

	if (len == -1) return (bsf::unexpected(get_send_error()));
	return (len);
}

/* Socket option reference */

template<domain::domain D, type::type T>
template<int O, typename V>
socket_base<D, T>::option_reference<O, V>::option_reference(enclosing::raw_t raw_socket):
	_raw_socket{ raw_socket } {}

template<domain::domain D, type::type T>
template<int O, typename V>
void
socket_base<D, T>::option_reference<O, V>::operator=(V optval) const {
	socklen_t	optlen = sizeof(V);

	if (::setsockopt(_raw_socket, SOL_SOCKET, O, &optval, &optlen) == -1) {
		throw (exception("setsockopt"));
	}
}

template<domain::domain D, type::type T>
template<int O, typename V>
socket_base<D, T>::option_reference<O, V>::operator V() const {
	V			optval;
	socklen_t	optlen = sizeof(V);

	if (::getsockopt(_raw_socket, SOL_SOCKET, O, &optval, &optlen) == -1) {
		throw (exception("getsockopt"));
	}
	return (optval);
}

}; // namespace bsf::net

#endif // BSF_NET_SOCKET_SOCKET_BASE_IPP
