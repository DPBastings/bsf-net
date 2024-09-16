#ifndef NETPP_ACCEPTOR_SOCKET_TPP
# define NETPP_ACCEPTOR_SOCKET_TPP

# ifndef NETPP_ACCEPTOR_SOCKET_HPP
#  error "include in AcceptorSocket.hpp"
# endif // NETPP_ACCEPTOR_SOCKET_HPP

namespace network {

template<Domain DOMAIN>
AcceptorSocket<DOMAIN>::AcceptorSocket(OptionList opts, char const *prot):
	StreamSocket<DOMAIN>(opts, prot) {}

template<Domain DOMAIN>
AcceptorSocket<DOMAIN>::AcceptorSocket(Address const& addr, OptionList opts, char const *prot):
	StreamSocket<DOMAIN>(addr, opts, prot) {}

template<Domain DOMAIN>
AcceptorSocket<DOMAIN>::AcceptorSocket(int backlog, Address const& addr, OptionList opts, char const *prot):
	StreamSocket<DOMAIN>(addr, opts, prot) {
	listen(backlog);
}

template<Domain DOMAIN>
void
AcceptorSocket<DOMAIN>::listen(int backlog) const {
	if (::listen(this->raw(), backlog) == -1) {
		throw (SocketException("listen"));
	}
}

template<Domain DOMAIN>
StreamSocket<DOMAIN>
AcceptorSocket<DOMAIN>::accept() const {
	Handle::Raw const	raw_handle = ::accept(this->raw(), nullptr, nullptr);
	
	if (raw_handle == StreamSocket<DOMAIN>::_invalid_handle)
		throw (SocketException("accept"));
	return (StreamSocket<DOMAIN>(raw_handle));
}

template<Domain DOMAIN>
StreamSocket<DOMAIN>
AcceptorSocket<DOMAIN>::accept(typename StreamSocket<DOMAIN>::Address& addr) const {
	socklen_t			size = addr.size();
	Handle::Raw const	raw_handle = ::accept(this->raw(), addr.raw(), &size);

	if (raw_handle == StreamSocket<DOMAIN>::_invalid_handle)
		throw (SocketException("accept"));
	if (size != addr.size())
		throw (network::Exception("address", "accept"));
	return (StreamSocket<DOMAIN>(raw_handle));
}

}; // namespace network

#endif // NETPP_ACCEPTOR_SOCKET_TPP
