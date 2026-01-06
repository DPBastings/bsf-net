#ifndef BSF_NET_SOCKET_ACCEPTOR_IPP
# define BSF_NET_SOCKET_ACCEPTOR_IPP

namespace bsf::net::socket {

// Basic operations

template<domain::domain D, type::type T>
acceptor<D, T>::acceptor(Base&& base) noexcept:
	socket_base<D, T>{ std::move(base) } {}

// Factories

/**
 * @param addr The address to which the new acceptor will be bound.
 * @param conf Configuration for the new acceptor.
 * @param backlog The number of pending connection requests that will be kept
 * in the acceptor's backlog (before they are `accept()`ed). If a connection
 * request comes in whilst the backlog is full, connection will be refused.
 * @return A useable acceptor socket, or nothing if an error occurred during
 * creation.
 */
template<domain::domain D, type::type T>
std::optional<acceptor<D, T>>
acceptor<D, T>::make(address_t const& addr, config conf, int backlog) noexcept {
	auto	base = make_socket<D, T>(addr, conf);

	if (!base) return (std::nullopt);
	acceptor	res{ std::move(*base) };
	res.listen(backlog);
	return (res);
}

// I/O methods

/**
 * @param conf The configuration parameters for the newly-created local socket.
 */
template<domain::domain D, type::type T>
acceptor<D, T>::accept_result
acceptor<D, T>::accept(config conf) const {
	address_t	addr;

	return (accept(addr, conf));
}

/**
 * @param addr The address of the peer socket.
 * @param conf The configuration parameters for the newly-created local socket.
 */
template<domain::domain D, type::type T>
acceptor<D, T>::accept_result
acceptor<D, T>::accept(address_t& addr, config conf) const {
	socklen_t			size = addr.size();
	handle::raw_t const	raw = ::accept4(
		this->_raw,
		addr.raw(),
		&size,
		conf.to_bitmask());

	if (raw == handle::null) return (detail::get_accept_error());
	return (socket_t{ raw });
}

template<domain::domain D, type::type T>
bool
acceptor<D, T>::listen(int backlog) const noexcept {
	return (::listen(raw(), backlog) != -1);
}

// Non-member functions

/**
 * @note Equivalent to calling the static `make()` member of `acceptor<D, T>`.
 */
template<domain::domain D, type::type T>
std::optional<acceptor<D, T>>
make_acceptor(address::address<D> const& addr, config conf, int backlog) noexcept {
	using Acceptor = acceptor<D, T>;

	return (Acceptor::make(addr, conf, backlog));
}

}; // namespace bsf::net::socket

#endif // BSF_NET_SOCKET_ACCEPTOR_IPP
