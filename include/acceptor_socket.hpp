#ifndef NETPP_ACCEPTOR_SOCKET_HPP
# define NETPP_ACCEPTOR_SOCKET_HPP

# include "network.hpp"
# include "socket.hpp"

namespace network {

template<socket_type T>
concept is_connection_based = (T == socket_type::stream || T == socket_type::seq_packet);

template<socket_domain D, socket_type T>
class basic_acceptor_socket: public basic_socket<D, T> {
public:
	using super = basic_socket<D, T>;
	using typename super::address_type;

	basic_acceptor_socket(address_type const&, bool = false, bool = true);
	basic_acceptor_socket(int, address_type const&, bool = false, bool = true);

	void	listen(int) const;
	super	accept(bool = false, bool = true) const;
	super	accept(address_type&, bool = false, bool = true) const;

	bool	is_listening() const;

	template<typename C>
	typename super::streamsize	recv(C const&, typename super::recv_flags) = delete;
	template<typename C>
	typename super::streamsize	send(C&, typename super::send_flags) = delete;
}; // class template acceptor_socket<socket_D>

}; // namespace network

# include "./acceptor_socket.tpp"

#endif // NETPP_ACCEPTOR_SOCKET_HPP
