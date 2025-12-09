#ifndef BSF_NET_SOCKET_ACCEPTOR_HPP
# define BSF_NET_SOCKET_ACCEPTOR_HPP

# include <libbsf/net/socket/type.hpp> // net::socket::type::*
# include <libbsf/net/socket/socket_base.hpp> // net::socket::socket_base

namespace bsf::net::socket {

template<domain::domain D, type::type T>
	requires (type::traits<T>::is_connection_based)
class acceptor: public socket_base<D, T> {
public:
	using super = socket_base<D, T>;
	using typename super::address_type;

	static acceptor	from_address(address_type const&, config);
	static acceptor	from_address(address_type const&, int, config);

	void	listen(int) const;
	super	accept(config) const;
	super	accept(address_type&, config) const;

	bool	is_listening() const;

	template<typename C>
	typename super::streamsize	recv(C const&, typename super::recv_flags) = delete;
	template<typename C>
	typename super::streamsize	send(C&, typename super::send_flags) = delete;
}; // class acceptor<D, T>

}; // namespace bsf::net::socket

# include "./acceptor.ipp"

#endif // BSF_NET_SOCKET_ACCEPTOR_HPP
