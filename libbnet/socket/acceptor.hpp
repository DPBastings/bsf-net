#ifndef BSF_NET_ACCEPTOR_SOCKET_HPP
# define BSF_NET_ACCEPTOR_SOCKET_HPP

# include "network.hpp"
# include "socket.hpp"

namespace bsf::net {

template<type::type T>
concept is_connection_based = (T == type::type::stream || T == type::type::seq_packet);

template<domain::domain D, type::type T>
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

}; // namespace bsf::net

# include "./acceptor_socket.ipp"

#endif // BSF_NET_ACCEPTOR_SOCKET_HPP
