#ifndef NETPP_STREAM_SOCKET_HPP
# define NETPP_STREAM_SOCKET_HPP

# include "network.hpp"
# include "socket.hpp"

namespace network {

template<socket_domain DOMAIN>
class stream_socket: public socket<DOMAIN, type::stream> {
public:
	using super = socket<DOMAIN, type::stream>;
	using typename super::raw_type;
	using typename super::address;
	using typename super::OptionList;

	stream_socket(OptionList = {}, char const* = nullptr);
	stream_socket(address const&, OptionList = {}, char const* = nullptr);

protected:
	friend class acceptor_socket<DOMAIN>;
	friend class socket_pair<stream_socket>;

	stream_socket(raw_type);
}; // class template stream_socket<socket_domain>

}; // namespace network

# include "stream_socket.tpp"

#endif // NETPP_STREAM_SOCKET_HPP
