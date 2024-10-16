#ifndef NETPP_ACCEPTOR_SOCKET_HPP
# define NETPP_ACCEPTOR_SOCKET_HPP

# include "network.hpp"
# include "./stream_socket.hpp"

namespace network {

template<socket_domain D>
class acceptor_socket: public stream_socket<D> {
public:
	using super = stream_socket<D>;
	using typename super::address;
	using typename super::option;

	acceptor_socket(option = option::none, char const* = nullptr);
	acceptor_socket(address const&, option = option::none, char const* = nullptr);
	acceptor_socket(int, address const&, option = option::none, char const* = nullptr);

	void				listen(int) const;
	stream_socket<D>	accept() const;
	stream_socket<D>	accept(address&) const;

	template<size_t BSIZE>
	void			read(Buffer<BSIZE> const&, int) = delete;
	template<size_t BSIZE>
	size_t			write(Buffer<BSIZE>&, int) = delete;
}; // class template acceptor_socket<socket_D>

}; // namespace network

# include "./acceptor_socket.tpp"

#endif // NETPP_ACCEPTOR_SOCKET_HPP
