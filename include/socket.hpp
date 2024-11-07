#ifndef NETPP_SOCKET_HPP
# define NETPP_SOCKET_HPP

# include <concepts>
# include <utility>

# include "network.hpp"
# include "./handle.hpp"

namespace network {

template<socket_domain D>
concept is_pairable = (D == socket_domain::local /* || D == socket_domain::tipc */);

template<typename T>
concept contiguous_container = requires(T t) {
	t.data();
	t.size();
};

template<typename T>
concept iobuf = std::is_same_v<typename T::value_type, char>
	&& contiguous_container<T>;

template<socket_domain D, socket_type T>
class basic_socket: public handle {
public:
	using address_type = network::basic_address<D>;
	using streamsize = ssize_t;
	using handle::raw_type;

	template<int O, typename V>
	class option_reference;

	template<int O>
	using bool_option = option_reference<O, bool>;
	template<int O>
	using int_option = option_reference<O, int>;
	template<int O>
	using time_option = option_reference<O, timeval>;

	enum class recv_flags: int {
		none = 0,
		close_on_exec = MSG_CMSG_CLOEXEC,
		dont_wait = MSG_DONTWAIT,
		error_queue = MSG_ERRQUEUE,
		out_of_band = MSG_OOB,
		peek = MSG_PEEK,
		truncate = MSG_TRUNC,
		wait_all = MSG_WAITALL,
	}; // enum class recv_flags
	enum class send_flags: int {
		none = 0,
		confirm = MSG_CONFIRM,
		dont_route = MSG_DONTROUTE,
		dont_wait = MSG_DONTWAIT,
		end_record = MSG_EOR,
		more = MSG_MORE,
		no_signal = MSG_NOSIGNAL,
		out_of_band = MSG_OOB,
		fast_open = MSG_FASTOPEN,
	}; // enum class send_flags

	basic_socket(bool = false, bool = true);

	socket_domain	domain() const noexcept;
	socket_type		type() const noexcept;
	int				protocol() const noexcept;
	address_type	address() const;
	address_type	peer_address() const;
	int				error() const;

	bool_option<SO_REUSEADDR>	reuse_address() const
		requires is_inet<D>;
	bool_option<SO_REUSEPORT>	reuse_port() const
		requires is_inet<D>;
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

	void		bind(address_type const&) const;
	void		connect(address_type const&) const;
	template<iobuf B>
	streamsize	send(B const&, send_flags = send_flags::none) const;
	template<iobuf B>
	streamsize	recv(B&, recv_flags = recv_flags::none) const;

	static std::pair<basic_socket, basic_socket>	make_pair(bool = false, bool = true) requires is_pairable<D>;

private:
	friend class basic_address<D>;

	template<int O, typename V>
	friend class option_reference;

	basic_socket(raw_type);

	static raw_type	make_handle(int);

	raw_type&	raw() noexcept;
	raw_type	raw() const noexcept;
}; // class template socket

template<socket_domain D, socket_type T>
template<int O, typename V>
class basic_socket<D, T>::option_reference {
public:
	using enclosing = basic_socket<D, T>;

	void operator=(V) const;

	operator V() const;
private:
	friend enclosing;
	friend class basic_acceptor_socket<D, T>;

	option_reference(enclosing::raw_type);

	enclosing::raw_type	_raw_socket;
}; // class option_reference

using ipv4_tcp_socket = basic_socket<socket_domain::ipv4, socket_type::stream>;
using ipv4_udp_socket = basic_socket<socket_domain::ipv4, socket_type::datagram>;
using ipv6_tcp_socket = basic_socket<socket_domain::ipv6, socket_type::stream>;
using ipv6_udp_socket = basic_socket<socket_domain::ipv6, socket_type::datagram>;

}; // namespace network

# include "./socket.tpp"

#endif // NETPP_SOCKET_HPP
