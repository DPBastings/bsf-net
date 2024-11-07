#ifndef NETPP_HPP
# define NETPP_HPP

# include <cstddef>
# include <stdexcept>

extern "C" {
# include <sys/socket.h>
# include <sys/types.h>
}

namespace network {

enum class socket_domain {
	local = AF_LOCAL,
	ipv4 = AF_INET,
	ipv6 = AF_INET6,
	unspecified = AF_UNSPEC,
}; // enum class socket_domain

template<socket_domain D>
concept internet_domain = (D == socket_domain::ipv4 || D == socket_domain::ipv6);

enum class socket_type {
	stream = SOCK_STREAM,
	datagram = SOCK_DGRAM,
	seq_packet = SOCK_SEQPACKET,
	raw = SOCK_RAW,
	rdm = SOCK_RDM,
}; // enum class socket_type

class handle;
	
template<socket_domain, socket_type>
class basic_socket;

template<socket_domain>
class basic_address;

template<socket_domain, socket_type>
class basic_acceptor_socket;

class basic_socketbuf;

class poller;

class exception: public std::exception {
public:
	exception(char const* = "") noexcept;

	char const*	what() const noexcept;
	int			error() const noexcept;

private:
	char const*	_msg;
	int			_errno;
}; // class exception

}; // namespace network

#endif // NETPP_HPP
