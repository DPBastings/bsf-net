#ifndef NETPP_ADDRESS_INFO_IPP
# define NETPP_ADDRESS_INFO_IPP

# ifndef NETPP_ADDRESS_INFO_HPP
#  error include in address_info.hpp
# endif // NETPP_ADDRESS_INFO_HPP

namespace network {

// Basic operations

template<socket_domain D, socket_type T>
basic_address_info<D, T>::basic_address_info():
	_raw(nullptr) {}


template<socket_domain D, socket_type T>
basic_address_info<D, T>::~basic_address_info() {
	::freeaddrinfo(_raw);
}

template<socket_domain D, socket_type T>
basic_address_info<D, T>::basic_address_info(basic_address_info<D, T>&& that):
	_raw(that._raw) {
	that._raw = nullptr;
}

template<socket_domain D, socket_type T>
basic_address_info<D, T>&
basic_address_info<D, T>::operator=(basic_address_info&& that) {
	if (this == &that) {
		return (*this);
	}
	_raw = that._raw;
	that._raw = nullptr;
	return (*this);
}

template<socket_domain D, socket_type T>
basic_address_info<D, T>::basic_address_info(std::string const& node) {
	hint_t	hint;
	int		err = ::getaddrinfo(node.c_str(), nullptr, &hint.data, &_raw);

	if (err != 0) {
		throw (exception(::gai_strerror(err)));
	}
}

template<socket_domain D, socket_type T>
basic_address_info<D, T>::basic_address_info(std::string const& node, std::string const& service) {
	hint_t	hint;
	int		err = ::getaddrinfo(node.c_str(), service.c_str(), &hint.data, &_raw);

	if (err != 0) {
		throw (exception(::gai_strerror(err)));
	}
}

template<socket_domain D, socket_type T>
basic_address_info<D, T>::basic_address_info(std::string const& node, std::string const& service, bool map_ipv4)
	requires requires { D == socket_domain::ipv6; } {
	hint_t	hint {map_ipv4 ? AI_V4MAPPED : 0};
	int		err = ::getaddrinfo(node.c_str(), service.c_str(), &hint.data, &_raw);

	if (err != 0) {
		throw (exception(::gai_strerror(err)));
	}
}

// Iterator methods

template<socket_domain D, socket_type T>
typename basic_address_info<D, T>::iterator
basic_address_info<D, T>::begin() const noexcept {
	return (iterator(_raw));
}

template<socket_domain D, socket_type T>
typename basic_address_info<D, T>::iterator
basic_address_info<D, T>::end() const noexcept {
	return (iterator(nullptr));
}

template<socket_domain D, socket_type T>
typename basic_address_info<D, T>::iterator
basic_address_info<D, T>::cbegin() const noexcept {
	return (begin());
}

template<socket_domain D, socket_type T>
typename basic_address_info<D, T>::iterator
basic_address_info<D, T>::cend() const noexcept {
	return (end());
}

/* Dependent types */

template<socket_domain D, socket_type T>
basic_address_info<D, T>::hint_t::hint_t(int flags):
	data {
		.ai_flags = flags,
		.ai_family = static_cast<int>(D),
		.ai_socktype = static_cast<int>(T),
		.ai_protocol = 0,
		.ai_addrlen = 0,
		.ai_addr = nullptr,
		.ai_canonname = nullptr,
		.ai_next = nullptr,
	} {}

template<socket_domain D, socket_type T>
basic_address_info<D, T>::iterator::iterator(addrinfo const* ptr):
	_ptr(ptr) {}

template<socket_domain D, socket_type T>
bool
basic_address_info<D, T>::iterator::operator==(iterator that) const noexcept {
	return (_ptr == that._ptr);
}

template<socket_domain D, socket_type T>
typename basic_address_info<D, T>::iterator&
basic_address_info<D, T>::iterator::operator++() noexcept {
	_ptr = _ptr->ai_next;
	return (*this);
}

template<socket_domain D, socket_type T>
typename basic_address_info<D, T>::iterator
basic_address_info<D, T>::iterator::operator++(int) noexcept {
	const_iterator	ret(*this);

	_ptr = _ptr->ai_next;
	return (ret);
}

template<socket_domain D, socket_type T>
typename basic_address_info<D,T>::address_type
basic_address_info<D, T>::iterator::operator*() const noexcept {
	return (address_type(reinterpret_cast<typename address_type::storage_type const*>(_ptr->ai_addr)));
}

} // namespace network

#endif // NETPP_ADDRESS_INFO_IPP