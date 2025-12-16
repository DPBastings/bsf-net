#ifndef BSF_NET_SOCKET_SOCKET_BASE_HPP
# define BSF_NET_SOCKET_SOCKET_BASE_HPP

# include <libbsf/net/domain.hpp> // net::domain::*
# include <libbsf/net/socket/type.hpp> // net::socket::type::*
# include <libbsf/net/address/address.hpp> // net::address::*
# include <libbsf/net/utility/handle.hpp> // net::handle
# include <libbsf/net/utility/expected.hpp> // bsf::expected

namespace bsf::net::socket {

struct config {
	bool	non_blocking = true;
	bool	close_on_exec = true;

	inline unsigned int	to_bitmask() const noexcept {
		return ((non_blocking ? SOCK_NONBLOCK : 0x00)
			| (close_on_exec ? SOCK_CLOEXEC : 0x00)
		);
	}
}; // struct config

enum class recv_flag: unsigned int {
	none = 0x00,
	close_on_exec = MSG_CMSG_CLOEXEC,
	dont_wait = MSG_DONTWAIT,
	error_queue = MSG_ERRQUEUE,
	out_of_band = MSG_OOB,
	peek = MSG_PEEK,
	truncate = MSG_TRUNC,
	wait_all = MSG_WAITALL,
}; // enum class recv_flag

enum class send_flag: unsigned int {
	none = 0x00,
	confirm = MSG_CONFIRM,
	dont_route = MSG_DONTROUTE,
	dont_wait = MSG_DONTWAIT,
	end_record = MSG_EOR,
	more = MSG_MORE,
	no_signal = MSG_NOSIGNAL,
	out_of_band = MSG_OOB,
	fast_open = MSG_FASTOPEN,
}; // enum class send_flag

enum class recv_error {
	uninitialized,
	would_block,
	connection_refused,
	interrupted,
	out_of_memory,
	not_connected,
}; // enum class recv_error

using recv_result = bsf::expected<std::size_t, recv_error>;

enum class send_error {
	uninitialized,
	access_violation,
	bad_address,
	would_block,
	connection_reset,
	interrupted,
	message_too_large,
	not_connected,
	out_of_memory,
	// ...
}; // enum class send_error

using send_result = bsf::expected<std::size_t, send_error>;

namespace detail {

recv_error	get_recv_error() noexcept;
send_error	get_send_error() noexcept;

};

namespace option {

enum option {
	is_listening = SO_ACCEPTCONN,
	// SO_ATTACH_FILTER
	// SO_ATTACH_BPF
	// SO_ATTACH_REUSEPORT_CBPF
	// SO_ATTACH_REUSEPORT_EBPF
	// SO_BINDTODEVICE
	// SO_BROADCAST
	// SO_BSDCOMPAT
	// SO_DEBUG
	// SO_DETACH_FILTER
	// SO_DETACH_BPF
	// SO_DOMAIN
	error = SO_ERROR,
	dont_route = SO_DONTROUTE,
	incoming_cpu = SO_INCOMING_CPU,
	// SO_INCOMING_NAPI_ID
	keep_alive = SO_KEEPALIVE,
	// SO_LINGER
	// SO_LOCK_FILTER
	// SO_MARK
	inline_oob = SO_OOBINLINE,
	// SO_PASSCRED
	// SO_PASSSEC
	// SO_PEEK_OFF
	// SO_PEERCRED
	// SO_PEERSEC
	priority = SO_PRIORITY,
	protocol = SO_PROTOCOL,
	recv_buffer_size = SO_RCVBUF,
	// SO_RCVBUFFORCE
	// SO_RCV_LOWAT
	// SO_SND_LOWAT
	// SO_RCVTIMEO,
	// SO_SNDTIMEO,
	reuse_address = SO_REUSEADDR,
	reuse_port = SO_REUSEPORT,
	// SO_RXQ_OVFL
	// SO_SELECT_ERR_QUEUE
	send_buffer_size = SO_SNDBUF,
	// SO_SNDBUF_FORCE
	// SO_TIMESTAMP
	// SO_TIMESTAMPNS
	// SO_TYPE
	// SO_BUSY_POLL
};

template<typename T, bool IsWriteable, bool IsReadable = true>
struct traits_base {
	using value_type = T;
	using api_type = std::conditional_t<std::is_same_v<T, bool>, int, T>;

	static constexpr bool	is_writeable = IsWriteable;
	static constexpr bool	is_readable = IsReadable;
};

template<option Opt>
struct traits;

template<>
struct traits<is_listening>: traits_base<bool, false> {};
template<>
struct traits<error>: traits_base<int, false> {};
template<>
struct traits<dont_route>: traits_base<bool, true> {};
template<>
struct traits<incoming_cpu>: traits_base<int, true /* OS-dependent */> {};
template<>
struct traits<keep_alive>: traits_base<bool, true> {};
template<>
struct traits<inline_oob>: traits_base<bool, true> {};
template<>
struct traits<priority>: traits_base<int, true> {};
template<>
struct traits<protocol>: traits_base<int, false> {};
template<>
struct traits<recv_buffer_size>: traits_base<int, true> {};
template<>
struct traits<reuse_address>: traits_base<bool, true> {};
template<>
struct traits<reuse_port>: traits_base<bool, true> {};
template<>
struct traits<send_buffer_size>: traits_base<int, true> {};

}; // namespace option

template<domain::domain D, type::type T>
class socket_base: public handle {
public:
	using address_t = address::address<D>;

	explicit socket_base() = default;

	static constexpr auto	domain() noexcept;
	static constexpr auto	type() noexcept;
	int						protocol() const;
	int						error() const;

	std::optional<address_t>	address() const;
	std::optional<address_t>	peer_address() const;

	template<option::option Opt, int Level = SOL_SOCKET>
	auto	option() const
		requires (option::traits<Opt>::is_readable);
	template<option::option Opt, int Level = SOL_SOCKET>
	void	option(option::traits<Opt>::value_type) const
		requires (option::traits<Opt>::is_writeable);

	void	bind(address_t const&) const;
	void	connect(address_t const&) const;

	recv_result	recv(void*, std::size_t) const;
	recv_result	recv(void*, std::size_t, recv_flag) const;
	send_result	send(void const*, std::size_t) const;
	send_result	send(void const*, std::size_t, send_flag) const;

	static std::optional<socket_base>			make(config conf);
	static std::pair<socket_base, socket_base>	make_pair(config conf)
		requires (D == domain::local);
private:
	friend class address::address<D>;

	socket_base(raw_t);
}; // class socket_base<D, T>

}; // namespace bsf::net::socket

# include "./socket_base.ipp"

#endif // BSF_NET_SOCKET_SOCKET_BASE_HPP
