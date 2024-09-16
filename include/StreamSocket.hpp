#ifndef NETPP_STREAM_SOCKET_HPP
# define NETPP_STREAM_SOCKET_HPP

# include "network.hpp"
# include "Socket.hpp"

namespace network {

template<Domain DOMAIN>
class StreamSocket: public Socket<DOMAIN, Type::stream> {
public:
	using super = Socket<DOMAIN, Type::stream>;
	using typename super::Raw;
	using typename super::Address;
	using typename super::OptionList;

	StreamSocket(OptionList = {}, char const* = nullptr);
	StreamSocket(Address const&, OptionList = {}, char const* = nullptr);

protected:
	friend class Acceptor<DOMAIN>;
	friend class SocketPair<StreamSocket>;

	StreamSocket(Raw);
}; // class template StreamSocket<Domain>

}; // namespace network

# include "StreamSocket.tpp"

#endif // NETPP_STREAM_SOCKET_HPP
