#ifndef BSF_NET_POLLER_HPP
# define BSF_NET_POLLER_HPP

# include "network.hpp"
# include "./handle.hpp"

# include <cstdint>
# include <initializer_list>
# include <unordered_map>
# include <unordered_set>

extern "C" {
# include <sys/epoll.h>
}

namespace bsf::net {
	
class poller: public handle {
public:
	class event;
	enum class mode: uint32_t {
		none = 0,
		edge_triggered = EPOLLET,
		one_shot = EPOLLONESHOT,
		wake_up = EPOLLWAKEUP,
		exclusive = EPOLLEXCLUSIVE,
	}; // enum class mode
	enum class event_type: uint32_t {
		read = EPOLLIN,
		write = EPOLLOUT,
		closed = EPOLLRDHUP,
		exception = EPOLLPRI,
		error = EPOLLERR,
		hangup = EPOLLHUP,
		all = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLPRI | EPOLLERR | EPOLLHUP,
	}; // enum class event_type
	using event_list = std::unordered_map<shared_handle, event>;

	poller(bool = true);
	~poller() = default;
	poller(poller const&) = delete;
	poller(poller&&);
	poller&	operator=(poller const&) = delete;
	poller&	operator=(poller&&);

	template<typename T>
	shared_handle	add(T&&, event_type, mode = mode::none);
	void			add(shared_handle const&, event_type, mode = mode::none);
	template<std::size_t MAX_EVENTS>
	event_list		wait(unsigned int) const;
	void			modify(shared_handle const&, event_type, mode);
	void			remove(shared_handle const&);
	size_t			size() const noexcept;

private:
	using handles = std::unordered_set<shared_handle>;

	handles	_handles;
}; // class poller

class poller::event {
public:
	bool	expire(event_type) noexcept;
	bool	happened(event_type) const noexcept;

private:
	friend class poller;
	using raw_type = epoll_event;

	event(raw_type);

	raw_type	_raw;
}; // class event

constexpr poller::mode			operator|(poller::mode, poller::mode);
constexpr poller::event_type	operator|(poller::event_type, poller::event_type);

}; // namespace bsf::net

# include "./poller.ipp"

#endif // BSF_NET_POLLER_HPP
