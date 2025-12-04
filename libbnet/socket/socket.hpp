#ifndef BSF_NET_SOCKET_HPP
# define BSF_NET_SOCKET_HPP

# include <libbnet/domain.hpp> // net::domain::*
# include <libbnet/socket/type.hpp> // net::socket::type::*
# include <libbnet/address.hpp> // net::address::*
# include <libbnet/utility/handle.hpp> // net::handle
# include <concepts> // std::same_as
# include <utility>

namespace bsf::net::socket {

template<typename Type>
concept contiguous_container = requires(Type Type) {
	Type.data();
	Type.size();
};

template<typename Type>
concept iobuf = std::same_as<typename Type::value_type, char>
	&& contiguous_container<Type>;


enum class recv_flags: int {
	none = 0x00,
	close_on_exec = MSG_CMSG_CLOEXEC,
	dont_wait = MSG_DONTWAIT,
	error_queue = MSG_ERRQUEUE,
	out_of_band = MSG_OOB,
	peek = MSG_PEEK,
	truncate = MSG_TRUNC,
	wait_all = MSG_WAITALL,
}; // enum class recv_flags
enum class send_flags: int {
	none = 0x00,
	confirm = MSG_CONFIRM,
	dont_route = MSG_DONTROUTE,
	dont_wait = MSG_DONTWAIT,
	end_record = MSG_EOR,
	more = MSG_MORE,
	no_signal = MSG_NOSIGNAL,
	out_of_band = MSG_OOB,
	fast_open = MSG_FASTOPEN,
}; // enum class send_flags



template<domain::domain Domain, type::type Type>
class basic_socket: public handle<basic_socket<Domain, Type>> {
public:
	using address_type = basic_address<Domain>;
	using streamsize = ssize_t;
	using handle::raw_type;
	static constexpr auto	domain = Domain;
	static constexpr auto	type = Type;

	template<int O, typename V>
	class option_reference;

	template<int O>
	using bool_option = option_reference<O, bool>;
	template<int O>
	using int_option = option_reference<O, int>;
	template<int O>
	using time_option = option_reference<O, timeval>;

	explicit basic_socket() = default;

	int				protocol() const noexcept;
	address_type	address() const;
	address_type	peer_address() const;
	int				error() const;

	bool_option<SO_REUSEADDR>	reuse_address() const
		requires is_inet<Domain>;
	bool_option<SO_REUSEPORT>	reuse_port() const
		requires is_inet<Domain>;
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

	template<typename... Ts>
	static std::pair<basic_socket, basic_socket>	make_pair(Ts&&...)
		requires is_pairable<Domain>;
private:
	friend class basic_address<Domain>;

	template<int O, typename V>
	friend class option_reference;

	basic_socket(raw_type);

	static raw_type	make_handle(int);
}; // class template socket

template<domain::domain Domain, type::type Type>
template<int O, typename V>
class basic_socket<Domain, Type>::option_reference {
public:
	using enclosing = basic_socket<Domain, Type>;

	void operator=(V) const;

	operator V() const;
private:
	friend enclosing;
	friend class basic_acceptor_socket<Domain, Type>;

	option_reference(enclosing::raw_type);

	enclosing::raw_type	_raw_socket;
}; // class option_reference

}; // namespace bsf::net

# include "./socket.ipp"

#endif // BSF_NET_SOCKET_HPP
