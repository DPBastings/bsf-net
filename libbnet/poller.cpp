#include "poller.hpp"

extern "C" {
# include <fcntl.h>
}

using bsf::net::poller;

// Basic operations

poller::poller(bool cloexec):
	handle(::epoll_create1(cloexec ? FD_CLOEXEC : 0)) {}

poller::poller(poller&& that):
	handle(static_cast<handle&&>(std::move(that))),
	_handles(that._handles) {}

poller&
poller::operator=(poller&& that) {
	handle::operator=(static_cast<handle&&>(std::move(that)));
	return (*this);
}

// Modifier methods

void
poller::add(shared_handle const& handle, event_type events, mode modes) {
	event::raw_type	rev;

	rev.events = static_cast<int>(events) | static_cast<int>(modes);
	rev.data.fd = handle->_raw;
	if (epoll_ctl(_raw, EPOLL_CTL_ADD, rev.data.fd, &rev) == -1) {
		switch (errno) {
		case EEXIST:	// attempt to add a handle twice
			break;
		default:
			throw (exception("epoll_ctl"));
		}
	}
	_handles.emplace(handle);
}

void
poller::modify(shared_handle const& handle, event_type events, mode modes) {
	event::raw_type	rev;

	rev.events = static_cast<int>(events) | static_cast<int>(modes);
	rev.data.fd = handle->_raw;
	if (epoll_ctl(_raw, EPOLL_CTL_MOD, handle->_raw, &rev) == -1) {
		switch (errno) {
		case ENOENT:	// attempt to modify an unregistered handle
			return;
		default:
			throw (exception("epoll_ctl"));
		}
	}
}

void
poller::remove(shared_handle const& handle) {
	event::raw_type	rev; // man epoll_ctl -> BUGS

	if (epoll_ctl(_raw, EPOLL_CTL_DEL, handle->_raw, &rev) == -1)
		throw (exception("epoll_ctl"));
	
	auto	it = std::find(_handles.begin(), _handles.end(), handle);

	_handles.erase(it);
}

size_t
poller::size() const noexcept {
	return (_handles.size());
}

/* poller::event */

poller::event::event(event::raw_type raw):
	_raw(raw) {}

bool
poller::event::happened(event_type type) const noexcept {
	return (_raw.events & static_cast<uint32_t>(type));
}

bool
poller::event::expire(event_type type) noexcept {
	bool	did_happen = happened(type);

	_raw.events &= ~static_cast<uint32_t>(type);
	return (did_happen);
}