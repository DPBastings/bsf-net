#ifndef NETPP_ADDRESS_INFO_IPP
# define NETPP_ADDRESS_INFO_IPP

# ifndef NETPP_ADDRESS_INFO_HPP
#  error include in address_info.hpp
# endif // NETPP_ADDRESS_INFO_HPP

namespace network {

// Basic operations

template<socket_domain D, socket_type T>
basic_resolver<D, T>::basic_resolver():
	_raw(nullptr) {}


template<socket_domain D, socket_type T>
basic_resolver<D, T>::~basic_resolver() {
	::freeaddrinfo(_raw);
}

template<socket_domain D, socket_type T>
basic_resolver<D, T>::basic_resolver(basic_resolver<D, T>&& that):
	_raw(that._raw) {
	that._raw = nullptr;
}

template<socket_domain D, socket_type T>
basic_resolver<D, T>&
basic_resolver<D, T>::operator=(basic_resolver&& that) {
	if (this == &that) {
		return (*this);
	}
	_raw = that._raw;
	that._raw = nullptr;
	return (*this);
}

template<socket_domain D, socket_type T>
basic_resolver<D, T>::basic_resolver(std::string const& node)
	requires requires { D == socket_domain::ipv4; } {
	hint_t	hint;
	int		err = ::getaddrinfo(node.c_str(), nullptr, &hint.data, &_raw);

	if (err != 0) {
		throw (exception(::gai_strerror(err)));
	}
}

template<socket_domain D, socket_type T>
basic_resolver<D, T>::basic_resolver(std::string const& node, bool map_ipv4)
	requires requires { D == socket_domain::ipv6; } {
	hint_t	hint {map_ipv4 ? AI_V4MAPPED : 0};
	int		err = ::getaddrinfo(node.c_str(), nullptr, &hint.data, &_raw);

	if (err != 0) {
		throw (exception(::gai_strerror(err)));
	}
}

template<socket_domain D, socket_type T>
basic_resolver<D, T>::basic_resolver(std::string const& node, std::string const& service)
	requires requires { D == socket_domain::ipv4; } {
	hint_t	hint;
	int		err = ::getaddrinfo(node.c_str(), service.c_str(), &hint.data, &_raw);

	if (err != 0) {
		throw (exception(::gai_strerror(err)));
	}
}

template<socket_domain D, socket_type T>
basic_resolver<D, T>::basic_resolver(std::string const& node, std::string const& service, bool map_ipv4)
	requires requires { D == socket_domain::ipv6; } {
	hint_t	hint {map_ipv4 ? AI_V4MAPPED : 0};
	int		err = ::getaddrinfo(node.c_str(), service.c_str(), &hint.data, &_raw);

	if (err != 0) {
		throw (exception(::gai_strerror(err)));
	}
}

// Iterator methods

template<socket_domain D, socket_type T>
typename basic_resolver<D, T>::iterator
basic_resolver<D, T>::begin() const noexcept {
	return (iterator(_raw));
}

template<socket_domain D, socket_type T>
typename basic_resolver<D, T>::iterator
basic_resolver<D, T>::end() const noexcept {
	return (iterator(nullptr));
}

template<socket_domain D, socket_type T>
typename basic_resolver<D, T>::iterator
basic_resolver<D, T>::cbegin() const noexcept {
	return (begin());
}

template<socket_domain D, socket_type T>
typename basic_resolver<D, T>::iterator
basic_resolver<D, T>::cend() const noexcept {
	return (end());
}

/* Dependent types */

template<socket_domain D, socket_type T>
basic_resolver<D, T>::hint_t::hint_t(int flags):
	data {
		.ai_flags = flags | AI_CANONNAME,
		.ai_family = static_cast<int>(D),
		.ai_socktype = static_cast<int>(T),
		.ai_protocol = 0,
		.ai_addrlen = 0,
		.ai_addr = nullptr,
		.ai_canonname = nullptr,
		.ai_next = nullptr,
	} {}

template<socket_domain D, socket_type T>
basic_resolver<D, T>::iterator::iterator(addrinfo const* ptr):
	_ptr(ptr) {}

template<socket_domain D, socket_type T>
bool
basic_resolver<D, T>::iterator::operator==(iterator that) const noexcept {
	return (_ptr == that._ptr);
}

template<socket_domain D, socket_type T>
typename basic_resolver<D, T>::iterator&
basic_resolver<D, T>::iterator::operator++() noexcept {
	_ptr = _ptr->ai_next;
	return (*this);
}

template<socket_domain D, socket_type T>
typename basic_resolver<D, T>::iterator
basic_resolver<D, T>::iterator::operator++(int) noexcept {
	const_iterator	ret(*this);

	_ptr = _ptr->ai_next;
	return (ret);
}

template<socket_domain D, socket_type T>
typename basic_resolver<D,T>::result_type
basic_resolver<D, T>::iterator::operator*() const noexcept {
	return (result_type {
		reinterpret_cast<typename address_type::storage_type const*>(_ptr->ai_addr),
		_ptr->ai_canonname != nullptr ? _ptr->ai_canonname : ""
	});
}

template<socket_domain D, socket_type T>
void
basic_resolver<D, T>::clear() noexcept {
	::freeaddrinfo(_raw);
	_raw = nullptr;
}

// Non-member functions

template<socket_domain D, socket_type T>
basic_resolver<D, T>::flag
operator|(basic_resolver<D, T>::flag lhs, basic_resolver<D, T>::flag rhs) {
	using flag = typename basic_resolver<D, T>::flag;

	return (static_cast<flag>(static_cast<int>(lhs) | static_cast<int>(rhs)));
}

}; // namespace network

#endif // NETPP_ADDRESS_INFO_IPP