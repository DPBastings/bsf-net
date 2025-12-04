#ifndef BSF_NET_ACCEPTOR_SOCKET_TPP
# define BSF_NET_ACCEPTOR_SOCKET_TPP

# ifndef BSF_NET_ACCEPTOR_SOCKET_HPP
#  error "include in acceptor_socket.hpp"
# endif // BSF_NET_ACCEPTOR_SOCKET_HPP

namespace bsf::net {

// Basic operations

template<domain::domain D, type::type T>
basic_acceptor_socket<D, T>::basic_acceptor_socket(address_type const& addr, bool non_blocking, bool close_on_exec):
	super(non_blocking, close_on_exec) {
	super::bind(addr);
}

template<domain::domain D, type::type T>
basic_acceptor_socket<D, T>::basic_acceptor_socket(int backlog, address_type const& addr, bool non_blocking, bool close_on_exec):
	super(non_blocking, close_on_exec) {
	super::bind(addr);
	super::listen(backlog);
}

// Accessor methods

template<domain::domain D, type::type T>
bool
basic_acceptor_socket<D, T>::is_listening() const {
	using option_type = typename super::template bool_option<SO_ACCEPTCONN>;

	return (option_type(this->_raw));
}

// I/O methods

template<domain::domain D, type::type T>
void
basic_acceptor_socket<D, T>::listen(int backlog) const {
	if (::listen(this->_raw, backlog) == -1) {
		throw (exception("listen"));
	}
}

template<domain::domain D, type::type T>
typename basic_acceptor_socket<D, T>::super
basic_acceptor_socket<D, T>::accept(bool non_blocking, bool close_on_exec) const {
	address_type	addr;

	return (accept(addr, non_blocking, close_on_exec));
}

template<domain::domain D, type::type T>
typename basic_acceptor_socket<D, T>::super
basic_acceptor_socket<D, T>::accept(address_type& addr, bool non_blocking, bool close_on_exec) const {
	socklen_t			size = addr.size();
	handle::raw_type 	raw_handle = ::accept4(
		this->_raw,
		addr.raw(),
		&size,
		(non_blocking ? SOCK_NONBLOCK : 0) | (close_on_exec ? SOCK_CLOEXEC : 0));

	if (raw_handle == handle::null)
		throw (exception("accept"));
	return (super(raw_handle));
}

}; // namespace bsf::net

#endif // BSF_NET_ACCEPTOR_SOCKET_TPP
