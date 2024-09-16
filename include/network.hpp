#ifndef NETPP_NETPP_HPP
# define NETPP_NETPP_HPP

# include <cstddef>
# include <stdexcept>

extern "C" {
# include <sys/socket.h>
# include <sys/types.h>
}

namespace network {
using std::size_t;

enum class Domain {
	unix = AF_UNIX,
	local = AF_LOCAL,
	ipv4 = AF_INET,
	ipv6 = AF_INET6,
	bluetooth = AF_BLUETOOTH,
}; // enum class Domain

enum class Type {
	stream = SOCK_STREAM,
	datagram = SOCK_DGRAM,
	seq_packet = SOCK_SEQPACHET,
	raw = SOCK_RAW,
	rdm = SOCK_RDM,
}; // enum class Type

class Handle;
	
template<Domain, Type>
class Socket;

template<Domain>
class Address;

template<Domain>
class StreamSocket;

template<Domain>
class AcceptorSocket;

template<size_t>
class StaticBuffer;

class DynamicBuffer;

class Poller;

class Exception {
public:
	Exception(char const*);

	int	errno() const noexcept;

private:
	char const*	_msg;
	int			_errno;
}; // class Exception
}; // namespace network

#endif // NETPP_NETPP_HPP
