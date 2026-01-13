#ifndef BSF_NET_SOCKET_BUFFER_HPP
# define BSF_NET_SOCKET_BUFFER_HPP

# include <cstddef> // std::size_t

namespace bsf::net::socket {

using streamsize = long;

template<std::size_t N>
class stream_buffer {
public:
	/// @brief Construct an empty stream buffer.
	stream_buffer();

	/// @brief Get a pointer to the beginning of this buffer's data store.
	[[nodiscard]] unsigned char const*	begin() const noexcept;
	[[nodiscard]] unsigned char*		begin() noexcept;
	/// @brief Get this buffer's `next` pointer.
	[[nodiscard]] unsigned char const*	next() const noexcept;
	[[nodiscard]] unsigned char*		next() noexcept;
	/// @brief Get this buffer's `end` pointer.
	[[nodiscard]] unsigned char const*	end() const noexcept;
	/// @brief Get the number of bytes held.
	[[nodiscard]] std::size_t			available() const noexcept;
	/// @brief Get the total buffer capacity.
	[[nodiscard]] constexpr std::size_t	total_capacity() const noexcept;
	/// @brief Get the remaining buffer capacity.
	[[nodiscard]] std::size_t			capacity() const noexcept;

	/// @brief Discard this buffer's contents.
	void	clear() noexcept;

	/// @brief Read a byte.
	void		peek(unsigned char&) const noexcept;
	/// @brief Read a range of bytes.
	void		peek(unsigned char*, std::size_t) const noexcept;
	/// @brief Attempt to read a byte.
	std::size_t	peek_try(unsigned char&) const noexcept;
	/// @brief Attempt to read a range of bytes.
	std::size_t	peek_try(unsigned char*, std::size_t) const noexcept;

	/// @brief Read a byte and advance the `next` pointer.
	void		get(unsigned char&) noexcept;
	/// @brief Read a range of bytes and advance the `next` pointer.
	void		get(unsigned char*, std::size_t) noexcept;
	/// @brief Attempt to read a byte and advance the `next` pointer.
	std::size_t	get_try(unsigned char&) noexcept;
	/// @brief Attempt to read a range of bytes and advance the `next` pointer.
	std::size_t	get_try(unsigned char*, std::size_t) noexcept;
	/// @brief Decrement the `next` pointer.
	std::size_t	unget(std::size_t = 1) noexcept;
	
	/// @brief Append a byte.
	void		put(unsigned char) noexcept;
	/// @brief Append a range of bytes.
	void		put(unsigned char const*, std::size_t) noexcept;
	/// @brief Attempt to append a byte.
	std::size_t	put_try(unsigned char) noexcept;
	/// @brief Attempt to append a range of bytes.
	std::size_t	put_try(unsigned char const*, std::size_t) noexcept;

	/// @brief Set the `next` pointer.
	void	seek(unsigned char*) noexcept;
	/// @brief Offset the `next` pointer.
	void	seek(streamsize) noexcept;
	/// @brief Offset the `end` pointer.
	void	set_end(std::size_t) noexcept;
private:
	/// @brief Get a pointer to past the end of the buffer data store.
	unsigned char const*	absolute_end() const noexcept;

	unsigned char			_data[N];
	unsigned char*			_next;
	unsigned char const*	_end;
}; // class stream_buffer<N>

}; // namespace bsf::net::socket

# include "./stream_buffer.ipp"

#endif // BSF_NET_SOCKET_BUFFER_HPP