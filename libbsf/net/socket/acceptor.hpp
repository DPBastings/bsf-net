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

template<domain::domain D, type::type T>
	requires (type::traits<T>::is_connection_based)
class acceptor: public socket_base<D, T> {
	using Base = socket_base<D, T>;
public:
	using socket_t = socket_base<D, T>;
	using typename Base::address_t;

	acceptor() = default;

	[[nodiscard]] static std::optional<acceptor>	make(address_t const&, config, int) noexcept;

	using accept_result = bsf::expected<socket_t, accept_error>;
	[[nodiscard]] accept_result	accept(config) const;
	[[nodiscard]] accept_result	accept(address_t&, config) const;
private:
	acceptor(Base&&) noexcept;
}; // class acceptor<D, T>

template<domain::domain D, type::type T>
[[nodiscard]] std::optional<acceptor<D, T>>
make_acceptor(address::address<D> const&, config, int) noexcept;

}; // namespace bsf::net::socket

# include "./acceptor.ipp"

#endif // BSF_NET_SOCKET_ACCEPTOR_HPP
