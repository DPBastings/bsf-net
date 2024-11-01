#ifndef NETPP_ACCEPTOR_SOCKET_TPP
# define NETPP_ACCEPTOR_SOCKET_TPP

# ifndef NETPP_ACCEPTOR_SOCKET_HPP
#  error "include in acceptor_socket.hpp"
# endif // NETPP_ACCEPTOR_SOCKET_HPP

namespace network {

// Basic operations

template<socket_domain D, socket_type T, socket_protocol P>
acceptor_socket<D, T, P>::acceptor_socket(address_type const& addr, bool non_blocking, bool close_on_exec):
	super(non_blocking, close_on_exec) {
	this->bind(addr);
}

template<socket_domain D, socket_type T, socket_protocol P>
acceptor_socket<D, T, P>::acceptor_socket(int backlog, address_type const& addr, bool non_blocking, bool close_on_exec):
	super(non_blocking, close_on_exec) {
	this->bind(addr);
	this->listen(backlog);
}

// Accessor methods

template<socket_domain D, socket_type T, socket_protocol P>
bool
acceptor_socket<D, T, P>::is_listening() const noexcept {
	return (super::template int_option<SO_ACCEPTCONN>());
}

// I/O methods

template<socket_domain D, socket_type T, socket_protocol P>
void
acceptor_socket<D, T, P>::listen(int backlog) const {
	if (::listen(this->_raw, backlog) == -1) {
		throw (exception("listen"));
	}
}

template<socket_domain D, socket_type T, socket_protocol P>
typename acceptor_socket<D, T, P>::super
acceptor_socket<D, T, P>::accept(bool non_blocking, bool close_on_exec) const {
	address_type	addr;

	return (accept(addr, non_blocking, close_on_exec));
}

template<socket_domain D, socket_type T, socket_protocol P>
typename acceptor_socket<D, T, P>::super
acceptor_socket<D, T, P>::accept(address_type& addr, bool non_blocking, bool close_on_exec) const {
	socklen_t			size = addr.size();
	handle::raw_type 	raw_handle = ::accept4(
		this->_raw,
		addr.raw(),
		&size,
		(non_blocking ? SOCK_NONBLOCK : 0) | (close_on_exec ? SOCK_CLOEXEC : 0));

	if (raw_handle == handle::_invalid_handle)
		throw (exception("accept"));
	return (super(raw_handle));
}

}; // namespace network

#endif // NETPP_ACCEPTOR_SOCKET_TPP
