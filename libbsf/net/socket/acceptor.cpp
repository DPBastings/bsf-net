#include <libbsf/net/socket/acceptor.hpp>
#include <cerrno> // errno

namespace bsf::net::socket::detail {

accept_error
get_accept_error() noexcept {
	switch (errno) {
	// case EAGAIN: // ***TODO*** EAGAIN and EWOULDBLOCK can be the same. Find a way to suppress compiler warnings.
	case EWOULDBLOCK: return (accept_error::would_block);
	case EBADF: return (accept_error::uninitialized);
	case ECONNABORTED: return (accept_error::connection_aborted);
	// EFAULT
	case EINTR: return (accept_error::interrupted);
	case EINVAL: return (accept_error::uninitialized);
	case EMFILE:
	case ENFILE: return (accept_error::out_of_files);
	case ENOBUFS:
	case ENOMEM: return (accept_error::out_of_memory);
	case ENOTSOCK: return (accept_error::uninitialized);
	// EOPNOTSUPP
	case EPERM: return (accept_error::firewall);
	case EPROTO: return (accept_error::protocol);
	default: __builtin_unreachable();
	}
}

}; // namespace bsf::net::socket::detail