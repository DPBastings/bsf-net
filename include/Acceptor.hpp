#ifndef NETPP_ACCEPTOR_SOCKET_HPP
# define NETPP_ACCEPTOR_SOCKET_HPP

# include "network.hpp"
# include "StreamSocket.hpp"

namespace network {

template<Domain DOMAIN>
class AcceptorSocket: public StreamSocket<DOMAIN> {
public:
	using super = StreamSocket<DOMAIN>;
	using typename super::Address;
	using typename super::OptionList;

	AcceptorSocket(OptionList = {}, char const* = nullptr);
	AcceptorSocket(Address const&, OptionList = {}, char const* = nullptr);
	AcceptorSocket(int, Address const&, OptionList = {}, char const* = nullptr);

	void					listen(int) const;
	StreamSocket<DOMAIN>	accept() const;
	StreamSocket<DOMAIN>	accept(typename StreamSocket<DOMAIN>::Address&) const;

	template<size_t BSIZE>
	void			read(Buffer<BSIZE> const&, int) = delete;
	template<size_t BSIZE>
	size_t			write(Buffer<BSIZE>&, int) = delete;
}; // class template AcceptorSocket<Domain>

}; // namespace network

# include "AcceptorSocket.tpp"

#endif // NETPP_ACCEPTOR_SOCKET_HPP
