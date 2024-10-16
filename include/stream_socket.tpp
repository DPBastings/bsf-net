#ifndef NETPP_STREAM_SOCKET_TPP
# define NETPP_STREAM_SOCKET_TPP

# ifndef NETPP_STREAM_SOCKET_HPP
#  error "include in network/stream_socket.hpp"
# endif // NETPP_STREAM_SOCKET_HPP

namespace network {

template<socket_domain DOMAIN>
stream_socket<DOMAIN>::stream_socket(raw_type raw):
	socket<DOMAIN, type::stream>(raw) {}

template<socket_domain DOMAIN>
stream_socket<DOMAIN>::stream_socket(OptionList opts, char const* prot):
	socket<DOMAIN, type::stream>(opts, prot) {}

template<socket_domain DOMAIN>
stream_socket<DOMAIN>::stream_socket(address const& addr, OptionList opts, char const* prot):
	socket<DOMAIN, type::stream>(addr, opts, prot) {}
	
}; // namespace network

#endif // NETPP_STREAM_SOCKET_TPP
