#ifndef BSF_NET_SOCKET_STREAM_HPP
# define BSF_NET_SOCKET_STREAM_HPP

# include <libbsf/net/utility/expected.hpp> // bsf::expected
# include <libbsf/net/domain.hpp> // net::domain::*
# include <libbsf/net/address/address.hpp> // net::address::*
# include <libbsf/net/socket/socket_base.hpp> // net::socket::socket_base
# include <libbsf/net/socket/stream_buffer.hpp> // net::socket::stream_buffer

namespace bsf::net::socket {

template<domain::domain D, std::size_t InCap, std::size_t OutCap = InCap>
class stream {
public:
	using socket_t = socket_base<D, type::stream>;
	using address_t = address<D>;

	/// @brief Construct a stream from a previously-configured socket.
	stream(socket_t&&) noexcept;

	/// @brief Create a stream connected to a remote address.
	static std::optional<stream>	make(address_t const&, config) noexcept;

	/// @brief Read a byte.
	void		peek(unsigned char&) const noexcept;
	/// @brief Read a range of bytes.
	void		peek(unsigned char*, std::size_t) const noexcept;
	/// @brief Attempt to read a byte.
	std::size_t	peek_try(unsigned char&) const noexcept;
	/// @brief Attempt to read a range of bytes.
	std::size_t	peek_try(unsigned char*, std::size_t) const noexcept;

	/// @brief Extract a byte.
	void		get(unsigned char&) noexcept;
	/// @brief Extract a range of bytes.
	void		get(unsigned char*, std::size_t) noexcept;
	/// @brief Extract a byte.
	std::size_t	get_try(unsigned char&) noexcept;
	/// @brief Extract a range of bytes.
	std::size_t	get_try(unsigned char*, std::size_t) noexcept;
	/// @brief Unextract a byte, if possible.
	std::size_t	unget(std::size_t = 1) noexcept;
	
	/// @brief Append a byte.
	void		put(unsigned char) noexcept;
	/// @brief Append a range of bytes.
	void		put(unsigned char const*, std::size_t) noexcept;
	/// @brief Attempt to append a byte.
	std::size_t	put_try(unsigned char) noexcept;
	/// @brief Attempt to append a range of bytes.
	std::size_t	put_try(unsigned char const*, std::size_t) noexcept;

	/// @brief Receive data into this stream's input buffer.
	recv_result	recv(recv_flag = recv_flag::none) noexcept;
	/// @brief Send the contents of this stream's output buffer.
	send_result	send(send_flag = send_flag::none) noexcept;
private:
	socket_t				_socket;
	stream_buffer<InCap>	_in;
	stream_buffer<OutCap>	_out;
}; // class stream<D>

}; // namespace bsf::net::socket

# include "./stream.ipp"

#endif // BSF_NET_SOCKET_STREAM_HPP