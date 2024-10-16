#ifndef NETPP_HPP
# define NETPP_HPP

# include <cstddef>
# include <stdexcept>

# include <sys/socket.h>
# include <sys/types.h>

namespace network {

enum class socket_domain {
	local = AF_LOCAL,
	ipv4 = AF_INET,
	// ipv6 = AF_INET6,
	// bluetooth = AF_BLUETOOTH,
}; // enum class socket_domain

enum class socket_type {
	stream = SOCK_STREAM,
	// datagram = SOCK_DGRAM,
	// seq_packet = SOCK_SEQPACKET,
	// raw = SOCK_RAW,
	// rdm = SOCK_RDM,
}; // enum class socket_type

class handle;
	
template<socket_domain, socket_type>
class socket;

template<socket_domain>
class address;

template<socket_domain>
class stream_socket;

template<socket_domain>
class acceptor_socket;

template<std::size_t>
class StaticBuffer;

class DynamicBuffer;

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
