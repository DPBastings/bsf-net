#include <libbsf/net/async/poller.hpp>
#include <cassert> // assert
#include <cerrno> // errno
extern "C" {
# include <fcntl.h> // FD_CLOEXEC
}

namespace bsf::net::async {

// Basic operations

poller::poller(bool cloexec):
	handle(::epoll_create1(cloexec ? FD_CLOEXEC : 0)) {}


/**
 * @brief Add a handle to the poller's interest list.
 */
poller::add_result
poller::add(handle const& h, event::type on, mode m) const noexcept {
	using Error = bsf::unexpected<add_error>;

	RawEvent	ev;
	ev.events = static_cast<uint32_t>(on) | static_cast<uint32_t>(m);
	ev.data.fd = h.raw(); // Might change this to a pointer to the handle.

	if (::epoll_ctl(raw(), EPOLL_CTL_ADD, h.raw(), &ev) == -1) {
		return (Error(detail::get_poller_add_error()));
	}
	return (registration(*this, h.raw()));
}

/**
 * @brief Modify the registration of a polled handle.
 */
bool
poller::modify(registration const& reg, event::type on, mode m) const noexcept {
	assert(reg._owner == this);

	RawEvent	ev;
	ev.events = static_cast<uint32_t>(on) | static_cast<uint32_t>(m);
	ev.data.fd = reg._registered_handle;

	return (::epoll_ctl(
		raw(),
		EPOLL_CTL_MOD,
		reg._registered_handle,
		&ev
	) != -1);
}

/**
 * @brief Remove a handle from the poller's interest list.
 * @param reg The registration of the handle that is to be removed.
 */
bool
poller::remove(registration& reg) const noexcept {
	bool const	success = do_remove(reg);
	if (success) {
		reg._owner = nullptr;
		reg._registered_handle = handle::null;
	}
	return (success);
}

bool
poller::do_remove(registration const& reg) const noexcept {
	assert(reg._owner == this);

	RawEvent	ev; // Cf. man 2 epoll_ctl -> BUGS.

	return (::epoll_ctl(
		raw(),
		EPOLL_CTL_DEL,
		reg._registered_handle,
		&ev
	) != -1);
}

poller::wait_result
poller::wait(event* events, std::size_t max, unsigned int timeout) noexcept {
	using Error = bsf::unexpected<wait_error>;

	_raw_events.resize(max);

	int const n = ::epoll_wait(
		raw(), _raw_events.data(), max, timeout);

	if (n == -1) return (Error(detail::get_poller_wait_error()));
	_raw_events.resize(n);
	for (auto const& ev: _raw_events) {
		*events = event{ static_cast<event::type>(ev.events), ev.data.fd };
		++events;
	}
	return (static_cast<std::size_t>(n));
}



poller::registration::~registration() noexcept {
	if (!_owner) return;
	_owner->do_remove(*this);
}



namespace detail {

poller::add_error
get_poller_add_error() noexcept {
	switch (errno) {
	case EBADF: return (poller::add_error::uninitialized);
	case EEXIST: return (poller::add_error::exists);
	case EINVAL: return (poller::add_error::bad_value);
	case ELOOP: return (poller::add_error::loop);
	case ENOMEM:
	case ENOSPC: return (poller::add_error::out_of_resources);
	case EPERM: return (poller::add_error::unpollable);
	default: __builtin_unreachable();
	}
}

poller::wait_error
get_poller_wait_error() noexcept {
	switch (errno) {
	case EBADF: return (poller::wait_error::uninitialized);
	// EFAULT
	case EINTR: return (poller::wait_error::interrupted);
	case EINVAL: return (poller::wait_error::uninitialized);
	default: __builtin_unreachable();
	}
}

}; // namespace detail

}; // namespace bsf::net::async