#ifndef BSF_NET_SOCKET_SOCKET_BASE_IPP
# define BSF_NET_SOCKET_SOCKET_BASE_IPP

# include <libbsf/net/utility/except.hpp>
extern "C" {
# include <unistd.h>
}

namespace bsf::net::socket {

/**
 * @brief Create a socket from a raw file descriptor.
 * @param raw A raw socket handle.
 * @pre The file descriptor must refer to a valid socket of the appropriate
 * configuration.
 */ 
template<domain::domain D, type::type T>
socket_base<D, T>::socket_base(raw_t raw):
	handle{ raw } {}

/// @brief Create a configured socket.
template<domain::domain D, type::type T>
std::optional<socket_base<D, T>>
socket_base<D, T>::make(config conf) {
	raw_t	raw = ::socket(
		static_cast<int>(D),
		static_cast<int>(T) | conf.to_bitmask(),
		0 // ***TODO*** Support different protocols.
	);
	if (raw == -1) return (std::nullopt);
	return (socket_base{ raw });
}

/// @brief Create a pair of local sockets.
template<domain::domain D, type::type T>
std::pair<socket_base<D, T>, socket_base<D, T>>
socket_base<D, T>::make_pair(config conf)
	requires (D == domain::unix) {
	raw_t	fds[2];

	if (::socketpair(
		static_cast<int>(D),
		static_cast<int>(T) | conf.to_bitmask(),
		0, // ***TODO*** Support different protocols.
		fds
	) == -1) throw (exception("socketpair"));
	return (std::make_pair(
		socket_base{ fds[0] },
		socket_base{ fds[1] }
	));
}


template<domain::domain D, type::type T>
constexpr auto
socket_base<D, T>::domain() noexcept {
	return (D);
}

template<domain::domain D, type::type T>
constexpr auto
socket_base<D, T>::type() noexcept {
	return (T);
}

template<domain::domain D, type::type T>
int
socket_base<D, T>::protocol() const {
	return (option<option::protocol>());
}



template<domain::domain D, type::type T>
std::optional<typename socket_base<D, T>::address_t>
socket_base<D, T>::local_address() const {
	address_t	addr;
	if (::getsockname(raw(), addr.data(), addr.size()) == -1) {
		return (std::nullopt);
	}
	return (addr);
}

template<domain::domain D, type::type T>
std::optional<typename socket_base<D, T>::address_t>
socket_base<D, T>::peer_address() const {
	address_t	addr;
	if (::getpeername(raw(), addr.data(), addr.size()) == -1) {
		return (std::nullopt);
	}
	return (addr);
}

template<domain::domain D, type::type T>
int
socket_base<D, T>::error() const {
	return (option<option::error>());
}

// Options

template<domain::domain D, type::type T>
template<option::option Opt, int Level>
auto
socket_base<D, T>::option() const
	requires (option::traits<Opt>::is_readable) {
	using ApiType = option::traits<Opt>::api_type;

	socklen_t	len = sizeof(ApiType);
	ApiType		value;

	if (::getsockopt(raw(), Level, Opt, &value, &len) == -1) {
		throw (exception("getsockopt"));
	}
	return (static_cast<option::traits<Opt>::value_type>(value));
}


template<domain::domain D, type::type T>
template<option::option Opt, int Level>
void
socket_base<D, T>::option(option::traits<Opt>::value_type value) const
	requires (option::traits<Opt>::is_writeable) {
	using ApiType = option::traits<Opt>::api_type;

	socklen_t	len = sizeof(ApiType);
	auto		api_value = static_cast<ApiType>(value);

	if (::setsockopt(raw(), Level, Opt, &api_value, &len) == -1) {
		throw (exception("setsockopt"));
	}
}

// Peers

template<domain::domain D, type::type T>
bind_result
socket_base<D, T>::bind(address_t const& addr) const {
	if (::bind(raw(), addr.data(), addr.size()) == -1) {
		return (bsf::unexpected<bind_error>(detail::get_bind_error()));
	}
	return (true);
}

template<domain::domain D, type::type T>
connect_result
socket_base<D, T>::connect(address_t const& addr) const {
	if (::connect(raw(), addr.data(), addr.size() == -1)) {
		return (bsf::unexpected<connect_error>(detail::get_connect_error()));
	}
	return (true);
}

template<domain::domain D, type::type T>
bool
socket_base<D, T>::listen(int backlog) const noexcept
	requires (type::traits<T>::is_connection_based) {
	return (::listen(raw(), backlog) != -1);
}

// I/O operations

/**
 * @param addr Address from which data is expected.
 * @param data The memory in which the received data should be stored.
 * @param length Number of bytes that should be received at the most.
 * @param flag Receive flag bitmask.
 */
template<domain::domain D, type::type T>
recv_result
socket_base<D, T>::recv(void* data, std::size_t length, recv_flag flag) const {
	auto const	byte_count
		= ::recv(raw(), data, length, static_cast<int>(flag));

	if (byte_count == -1) {
		return (bsf::unexpected<recv_error>(detail::get_recv_error()));
	}
	return (static_cast<std::size_t>(byte_count));
}

/**
 * @param data The memory in which the received data should be stored.
 * @param length Number of bytes that should be received at the most.
 * @param flag Receive flag bitmask.
 */
template<domain::domain D, type::type T>
recv_result
socket_base<D, T>::recv(
	address_t const& addr, void* data, std::size_t length, recv_flag flag
) const requires (!type::traits<T>::is_connection_based) {
	auto const	byte_count
		= ::recv(raw(), data, length, static_cast<int>(flag), addr.ptr(), addr.size());

	if (byte_count == -1) {
		return (bsf::unexpected<recv_error>(detail::get_recv_error()));
	}
	return (static_cast<std::size_t>(byte_count));
}

/**
 * @param data Pointer to the data to be transmitted.
 * @param length Length of the data.
 * @param flag Send flag bitmask.
 */
template<domain::domain D, type::type T>
send_result
socket_base<D, T>::send(void const* data, std::size_t length, send_flag flag) const {
	auto const	byte_count
		= ::send(raw(), data, length, static_cast<int>(flag));

	if (byte_count == -1) {
		return (bsf::unexpected<send_error>(detail::get_send_error()));
	}
	return (static_cast<std::size_t>(byte_count));
}

/**
 * @param addr Address to deliver the data to.
 * @param data Pointer to the data to be transmitted.
 * @param length Length of the data.
 * @param flag Send flag bitmask.
 */
template<domain::domain D, type::type T>
send_result
socket_base<D, T>::send(
	address_t const& addr, void const* data, std::size_t length, send_flag flag
) const
	requires (!type::traits<T>::is_connection_based) {
	auto const	byte_count
		= ::sendto(raw(), data, length, static_cast<int>(flag), addr.ptr(), addr.size());

	if (byte_count == -1) {
		return (bsf::unexpected<send_error>(detail::get_send_error()));
	}
	return (static_cast<std::size_t>(byte_count));
}



template<domain::domain D, type::type T>
std::optional<socket_base<D, T>>
make_socket(address<D> const& addr, config conf) noexcept {
	auto	res = socket_base<D, T>::make(conf);

	if (!res) return (std::nullopt);
	REQUIRE(res->bind(addr));
	return (res);
}

}; // namespace bsf::net::socket

#endif // BSF_NET_SOCKET_SOCKET_BASE_IPP
