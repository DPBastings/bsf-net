#ifndef BSF_NET_SOCKET_ACCEPTOR_HPP
# define BSF_NET_SOCKET_ACCEPTOR_HPP

# include <libbsf/net/domain.hpp> // net::domain::*
# include <libbsf/net/utility/expected.hpp> // bsf::expected
# include <libbsf/net/socket/type.hpp> // net::socket::type::*
# include <libbsf/net/socket/socket_base.hpp> // net::socket::socket_base

namespace bsf::net::socket {

enum class accept_error {
	/// The socket is empty or badly configured.
	uninitialized,
	/// The socket is non-blocking and there are no connection requests.
	would_block,
	/// Connection aborted.
	connection_aborted,
	/// The call was interrupted by a signal handler.
	interrupted,
	/// A file limit was reached.
	out_of_files,
	/// Insufficient memory.
	out_of_memory,
	/// Firewall settings prevented the connection.
	firewall,
	/// Protocol error.
	protocol,
};
namespace detail {

accept_error	get_accept_error() noexcept;

}; // namespace detail

/**
 * @brief Acceptor socket class.
 *
 * An acceptor socket serves to establish a connection to any address by
 * accepting an incoming connection request from such an address. A common
 * use case of this pattern would be the Internet *server*: in most cases,
 * a server does not know to which remote machines it will be connecting, nor
 * does it initiate the connection (that is what the client would do).
 * @tparam D The acceptor's *domain* (address family). The documentation of
 * `socket_base` provides a more detailed explanation.
 * @tparam T The acceptor's *type* (communication mode). The documentation of
 * `socket_base` provides a more detailed explanation.
 */
template<domain::domain D, type::type T>
	requires (type::traits<T>::is_connection_based)
class acceptor: public socket_base<D, T> {
	using Base = socket_base<D, T>;
public:
	/// An `accept()`ed connection will create a socket of this type.
	using socket_t = socket_base<D, T>;
	using typename Base::address_t;

	acceptor() = default;

	/// @brief Create a new acceptor socket.
	[[nodiscard]] static std::optional<acceptor>	make(address_t const&, config, int) noexcept;

	/// @brief An expected socket base, or an error code in case of error.
	using accept_result = bsf::expected<socket_t, accept_error>;
	/// @brief Accept a new connection.
	[[nodiscard]] accept_result	accept(config) const;
	/// @brief Accept a new connection and retrieve the peer's address.
	[[nodiscard]] accept_result	accept(address_t&, config) const;
private:
	acceptor(Base&&) noexcept;

	bool	listen(int);
}; // class acceptor<D, T>

/// @brief Create a new acceptor socket.
template<domain::domain D, type::type T>
[[nodiscard]] std::optional<acceptor<D, T>>
make_acceptor(address<D> const&, config, int) noexcept;

}; // namespace bsf::net::socket

# include "./acceptor.ipp"

#endif // BSF_NET_SOCKET_ACCEPTOR_HPP
