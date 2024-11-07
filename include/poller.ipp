#ifndef NETPP_POLLER_METHOD_TPP
# define NETPP_POLLER_METHOD_TPP

# ifndef NETPP_POLLER_HPP
#  error "include in network/poller.hpp"
# endif // NETPP_POLLER_HPP

# include <algorithm>

namespace network {

template<typename T> // Add concept: requires a handle-derived type.
shared_handle
poller::add(T&& handle, event_type events, mode modes) {
	shared_handle	shared_handle = std::make_shared<T>(std::move(handle));
	
	add(shared_handle, events, modes);
	return (shared_handle);
}

template<size_t MAX_EVENTS>
poller::event_list
poller::wait(unsigned int timeout) const {
	event_list		evs;
	event::raw_type	revs[MAX_EVENTS];
	int				n = epoll_wait(_raw, revs, MAX_EVENTS, timeout);

	if (n == -1)
		throw (exception("epoll_wait"));
	for (int i = 0; i < n; ++i) {
		auto	is_match = [fd = revs[i].data.fd](shared_handle const& shandle) {
			return (shandle->_raw == fd);
		};
		auto	it = std::find_if(_handles.begin(), _handles.end(), is_match);

		if (it == _handles.end())
			throw (exception("polled handle not found in list"));
		evs.insert({*it, revs[i]});
	}
	return (evs);
}

constexpr poller::mode
operator|(poller::mode lhs, poller::mode rhs) {
	return (static_cast<poller::mode>(
		static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs)
	));
}

constexpr poller::event_type
operator|(poller::event_type lhs, poller::event_type rhs) {
	return (static_cast<poller::event_type>(
		static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs)
	));
}

}; // namespace network

#endif // NETPP_POLLER_METHOD_TPP