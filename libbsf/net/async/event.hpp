#ifndef BSF_NET_EVENT_HPP
# define BSF_NET_EVENT_HPP

# include <libbsf/net/utility/handle.hpp>
# include <cstdint> // *int*_t
extern "C" {
# include <sys/epoll.h> // EPOLL*, epoll_event
}

namespace bsf::net::async {

class event {
public:
	enum type: uint32_t {
		none = 0x00,
		read = EPOLLIN,
		write = EPOLLOUT,
		closed = EPOLLRDHUP,
		exception = EPOLLPRI,
		error = EPOLLERR,
		hangup = EPOLLHUP,
		any = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLPRI | EPOLLERR | EPOLLHUP,
	}; // enum type

	bool	concerns(handle const&) const noexcept;
	bool	is(type) const noexcept;
	operator bool() const noexcept;
private:
	friend class poller;

	event(type, handle::raw_t) noexcept;

	type			_mask = type::none;
	handle::raw_t	_ref = handle::null;
}; // class event

}; // namespace bsf::net::event

#endif // BSF_NET_EVENT_HPP