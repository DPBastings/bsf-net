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

template<domain::domain Domain, type::type Type>
class socket_base: public handle {
public:
	using address_t = address::address<Domain>;
	template<int O, typename V>
	class option_reference;

	template<int O>
	using bool_option = option_reference<O, bool>;
	template<int O>
	using int_option = option_reference<O, int>;
	template<int O>
	using time_option = option_reference<O, timeval>;

	explicit socket_base() = default;

	static constexpr auto	domain() noexcept;
	static constexpr auto	type() noexcept;
	int						protocol() const noexcept;

	address_t	address() const;
	address_t	peer_address() const;
	int			error() const;

	bool_option<SO_REUSEADDR>	reuse_address() const
		requires (domain::traits<Domain>::is_inet);
	bool_option<SO_REUSEPORT>	reuse_port() const
		requires (domain::traits<Domain>::is_inet);
	bool_option<SO_DONTROUTE>	dont_route() const;
	int_option<SO_INCOMING_CPU>	cpu_affinity() const;
	bool_option<SO_KEEPALIVE>	keep_alive() const;
	bool_option<SO_OOBINLINE>	inline_oob() const;
	int_option<SO_PRIORITY>		priority() const;
	int_option<SO_RCVBUF>		recv_buffer_size() const;
	int_option<SO_SNDBUF>		send_buffer_size() const;
	int_option<SO_RCVLOWAT>		recv_minimum() const;
	int_option<SO_SNDLOWAT>		send_minimum() const;
	time_option<SO_RCVTIMEO>	recv_timeout() const;
	time_option<SO_SNDTIMEO>	send_timeout() const;

	void	bind(address_t const&) const;
	void	connect(address_t const&) const;

	recv_result	recv(void*, std::size_t) const;
	recv_result	recv(void*, std::size_t, recv_flag) const;
	send_result	send(void const*, std::size_t) const;
	send_result	send(void const*, std::size_t, send_flag) const;

	static std::optional<socket_base>			make(config conf);
	static std::pair<socket_base, socket_base>	make_pair(config conf)
		requires (Domain == domain::local);
private:
	friend class address::address<Domain>;
	template<int O, typename V>
	friend class option_reference;

	socket_base(raw_t);
}; // class socket_base<Domain, Type>

template<domain::domain Domain, type::type Type>
template<int O, typename V>
class socket_base<Domain, Type>::option_reference {
public:
	using enclosing = socket_base<Domain, Type>;

	void operator=(V) const;

	operator V() const;
private:
	friend enclosing;

	option_reference(enclosing::raw_t);

	enclosing::raw_t	_raw_socket;
}; // class option_reference

}; // namespace bsf::net::socket

# include "./socket_base.ipp"

#endif // BSF_NET_SOCKET_SOCKET_BASE_HPP
