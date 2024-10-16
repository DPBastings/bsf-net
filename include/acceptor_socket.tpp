#ifndef NETPP_ACCEPTOR_SOCKET_TPP
# define NETPP_ACCEPTOR_SOCKET_TPP

# ifndef NETPP_ACCEPTOR_SOCKET_HPP
#  error "include in acceptor_socket.hpp"
# endif // NETPP_ACCEPTOR_SOCKET_HPP

namespace network {

template<socket_domain D>
acceptor_socket<D>::acceptor_socket(OptionList opts, char const *prot):
	stream_socket<D>(opts, prot) {}

template<socket_domain D>
acceptor_socket<D>::acceptor_socket(address const& addr, OptionList opts, char const *prot):
	stream_socket<D>(addr, opts, prot) {}

template<socket_domain D>
acceptor_socket<D>::acceptor_socket(int backlog, address const& addr, OptionList opts, char const *prot):
	stream_socket<D>(addr, opts, prot) {
	listen(backlog);
}

template<socket_domain D>
void
acceptor_socket<D>::listen(int backlog) const {
	if (::listen(this->raw(), backlog) == -1) {
		throw (socketexception("listen"));
	}
}

template<socket_domain D>
stream_socket<D>
acceptor_socket<D>::accept() const {
	handle::raw_type const	raw_handle = ::accept(this->raw(), nullptr, nullptr);
	
	if (raw_handle == stream_socket<D>::_invalid_handle)
		throw (socketexception("accept"));
	return (stream_socket<D>(raw_handle));
}

template<socket_domain D>
stream_socket<D>
acceptor_socket<D>::accept(typename stream_socket<D>::address& addr) const {
	socklen_t			size = addr.size();
	handle::raw_type const	raw_handle = ::accept(this->raw(), addr.raw(), &size);

	if (raw_handle == stream_socket<D>::_invalid_handle)
		throw (socketexception("accept"));
	if (size != addr.size())
		throw (network::exception("address", "accept"));
	return (stream_socket<D>(raw_handle));
}

}; // namespace network

#endif // NETPP_ACCEPTOR_SOCKET_TPP
