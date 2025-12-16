#include <libbsf/net/socket/socket_base.hpp>
#include <cerrno> // errno

namespace bsf::net::socket::detail {

recv_error
get_recv_error() noexcept {
	switch (errno) {
	// case EAGAIN: // ***TODO*** EAGAIN and EWOULDBLOCK can be the same. Find a way to suppress compiler warnings.
	case EWOULDBLOCK: return (recv_error::would_block);
	case EBADF: return (recv_error::uninitialized);
	case ECONNREFUSED: return (recv_error::connection_refused);
	// EFAULT
	case EINTR: return (recv_error::interrupted);
	// EINVAL
	case ENOMEM: return (recv_error::out_of_memory);
	case ENOTCONN: return (recv_error::not_connected);
	case ENOTSOCK: return (recv_error::uninitialized);
	default: __builtin_unreachable();
	}
}

send_error
get_send_error() noexcept {
	switch (errno) {
	case EACCES: return (send_error::access_violation);
	// case EAGAIN: // Has a slightly different meaning for Internet UDP sockets.
	case EWOULDBLOCK: return (send_error::would_block);
	// EALREADY
	case EBADF: return (send_error::uninitialized);
	case ECONNRESET: return (send_error::connection_reset);
	case EFAULT: return (send_error::bad_address);
	case EINTR: return (send_error::interrupted);
	// EISCONN
	case EMSGSIZE: return (send_error::message_too_large);
	// ENOBUFS
	case ENOMEM: return (send_error::out_of_memory);
	case ENOTCONN: return (send_error::not_connected);
	case ENOTSOCK: return (send_error::uninitialized);
	// EOPNOTSUPP (will be enforced at compile time)
	case EPIPE: return (send_error::uninitialized);
	default: __builtin_unreachable();
	}
}

};