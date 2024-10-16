#ifndef NETPP_SOCKET_HPP
# define NETPP_SOCKET_HPP

# include <initializer_list>
# include <stdexcept>
# include <utility>

# include "network.hpp"
# include "./handle.hpp"

namespace network {

template<socket_domain D, socket_type T>
class socket: public handle {
public:
	using address_type = network::address<D>;
	using streamsize = std::ssize_t;
	using handle::raw_type;

	enum class option: int {
		none = 0;
		nonblock = SOCK_NONBLOCK,
		cloexec = SOCK_CLOEXEC,
	}; // enum class option
	enum class recv_flags: int {
		none = 0;
		close_on_exec = MSG_CMSG_CLOEXEC,
		dont_wait = MSG_DONTWAIT,
		error_queue = MSG_ERRQUEUE,
		out_of_band = MSG_OOB,
		peek = MSG_PEEK,
		truncate = MSG_TRUNC,
		wait_all = MSG_WAITALL,
	}; // enum class recv_flags
	enum class send_flags: int {
		none = 0;
		confirm = MSG_CONFIRM,
		dont_route = MSG_DONTROUTE,
		dont_wait = MSG_DONTWAIT,
		end_record = MSG_EOR,
		more = MSG_MORE,
		no_signal = MSG_NOSIGNAL,
		out_of_band = MSG_OOB,
		fast_open = MSG_FASTOPEN,
	}; // enum class send_flags

	static constexpr socket_domain	domain = D;
	static constexpr socket_type	type = T;

	socket(option = option::none, char const* = "");
	socket(address_type const&, option, char const* = "");

	void	bind(address_type const&);

	static std::pair<socket, socket>	make_pair();

	template<contiguous_byte_container C>
	streamsize	send(BaseBuffer<C> const&, send_flags = send_flags::none) const;
	template<contiguous_byte_container C>
	streamsize	recv(Buffer<C>&, recv = recv_flags::none) const;

	address_type	address() const noexcept;
	bool			listens() const noexcept;
	int				protocol() const noexcept;

protected:
	socket(raw_type);

private:
	static raw_type	make_handle(option, char const*) noexcept;
	static int		get_protocol(char const*) noexcept;
}; // class template socket

}; // namespace network

# include "socket.tpp"

#endif // NETPP_SOCKET_HPP
