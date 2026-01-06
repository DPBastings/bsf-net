#ifndef BSF_NET_SOCKET_ACCEPTOR_IPP
# define BSF_NET_SOCKET_ACCEPTOR_IPP

namespace bsf::net::socket {

// Basic operations

/// @brief Create an acceptor from a socket base.
/// @post The acceptor will retain the base's address binding and listening
/// status.
template<domain::domain D, type::type T>
acceptor<D, T>::acceptor(Base&& base) noexcept:
	socket_base<D, T>{ std::move(base) } {}

// Factories

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

template<domain::domain D, type::type T>
acceptor<D, T>::accept_result
acceptor<D, T>::accept(config conf) const {
	address_t	addr;

	return (accept(addr, conf));
}

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

// Non-member functions

template<domain::domain D, type::type T>
std::optional<acceptor<D, T>>
make_acceptor(address<D> const& addr, config conf, int backlog) noexcept {
	using Acceptor = acceptor<D, T>;

	return (Acceptor::make(addr, conf, backlog));
}

}; // namespace bsf::net::socket

#endif // BSF_NET_SOCKET_ACCEPTOR_IPP
