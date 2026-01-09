#ifndef BSF_NET_ASYNC_POLLER_HPP
# define BSF_NET_ASYNC_POLLER_HPP

# include <libbsf/net/async/event.hpp> // net::async::event
# include <libbsf/net/utility/expected.hpp> // bsf::expected
# include <libbsf/net/utility/handle.hpp>
# include <vector> // std::vector

extern "C" {
# include <sys/epoll.h> // Linux `epoll`
}

namespace bsf::net::async {

class poller: public handle {
public:
	class registration {
	public:
		registration() = default;
		~registration() noexcept;
		registration(registration&&) noexcept;
		registration&	operator=(registration&&) noexcept;
		registration(registration const&) = delete;
		registration&	operator=(registration const&) = delete;
	private:
		friend class poller;

		registration(poller const&, handle::raw_t) noexcept;

		poller const*	_owner = nullptr;
		handle::raw_t	_registered_handle = handle::null;
	}; // class registration

	enum class add_error {
		uninitialized,
		exists,
		bad_value,
		loop,
		out_of_resources,
		unpollable,
	}; // enum class add_error

	enum class wait_error {
		uninitialized,
		interrupted,
	}; // enum class wait_error

	enum mode: uint32_t {
		none = 0x00,
		edge_triggered = EPOLLET,
		one_shot = EPOLLONESHOT,
		wake_up = EPOLLWAKEUP,
		exclusive = EPOLLEXCLUSIVE,
	}; // enum class mode

	poller(bool = true);
	~poller() = default;
	poller(poller&&) = delete;
	poller&	operator=(poller&&) = delete;
	poller(poller const&) = delete;
	poller&	operator=(poller const&) = delete;

	using add_result = bsf::expected<registration, add_error>;
	add_result	add(handle const&, event::type, mode) const noexcept;
	bool		modify(registration const&, event::type, mode) const noexcept;
	bool		remove(registration&) const noexcept;

	using wait_result = bsf::expected<std::size_t, wait_error>;
	wait_result	wait(event*, std::size_t, unsigned int) noexcept;
private:
	using RawEvent = epoll_event;
	using EventBuffer = std::vector<RawEvent>;

	bool	do_remove(registration const&) const noexcept;

	EventBuffer	_raw_events{};
}; // class poller

namespace detail {

poller::add_error	get_poller_add_error() noexcept;
poller::wait_error	get_poller_wait_error() noexcept;

}; // namespace detail

}; // namespace bsf::net::async

#endif // BSF_NET_ASYNC_POLLER_HPP
