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

bind_error
get_bind_error() noexcept {
	switch (errno) {
	case EACCES: return (bind_error::access_violation);
	case EADDRINUSE: return (bind_error::address_in_use);
	case EBADF: return (bind_error::uninitialized);
	case EINVAL: return (bind_error::already_bound);
	// EINVAL for invalid address is compile-time eliminated
	case ENOTSOCK: return (bind_error::uninitialized);
	case EADDRNOTAVAIL:
	case ELOOP:
	case ENAMETOOLONG:
	case ENOENT:
	case ENOTDIR: return (bind_error::bad_address); // Unix sockets
	case ENOMEM: return (bind_error::out_of_resources);
	case EROFS: return (bind_error::access_violation);
	default: __builtin_unreachable();
	}
}

connect_error
get_connect_error() noexcept {
	switch (errno) {
	case EACCES: return (connect_error::access_violation);
	case EPERM: return (connect_error::access_violation);
	case EADDRINUSE: return (connect_error::address_in_use);
	case EADDRNOTAVAIL: return (connect_error::address_unavailable);
	// EAFNOSUPPORT (enforced at compile time)
	case EAGAIN: return (connect_error::would_block);
	case EALREADY: return (connect_error::busy);\
	case EBADF: return (connect_error::uninitialized);
	case ECONNREFUSED: return (connect_error::refused);
	// EFAULT
	case EINPROGRESS: return (connect_error::busy);
	case EINTR: return (connect_error::interrupted);
	case EISCONN: return (connect_error::already_connected);
	case ENOTSOCK: return (connect_error::uninitialized);
	// EPROTOTYPE (enforced at compile time)
	case ETIMEDOUT: return (connect_error::timeout);
	default: __builtin_unreachable();
	}
}

}; // namespace bsf::net::socket::detail