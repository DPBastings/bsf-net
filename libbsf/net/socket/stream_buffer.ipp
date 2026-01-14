#ifndef BSF_NET_SOCKET_BUFFER_IPP
# define BSF_NET_SOCKET_BUFFER_IPP

# include <algorithm> // std::min
# include <cstring> // std::memcpy

namespace bsf::net::socket {

template<std::size_t N>
stream_buffer<N>::stream_buffer():
	_data{},
	_next{ _data },
	_end{ _data } {}


template<std::size_t N>
unsigned char const*
stream_buffer<N>::begin() const noexcept {
	return (_data);
}

template<std::size_t N>
unsigned char*
stream_buffer<N>::begin() noexcept {
	return (_data);
}

template<std::size_t N>
unsigned char*
stream_buffer<N>::next() noexcept {
	return (_next);
}

template<std::size_t N>
unsigned char const*
stream_buffer<N>::next() const noexcept {
	return (_next);
}

template<std::size_t N>
unsigned char const*
stream_buffer<N>::end() const noexcept {
	return (_end);
}

template<std::size_t N>
unsigned char const*
stream_buffer<N>::absolute_end() const noexcept {
	return (begin() + total_capacity());
}

template<std::size_t N>
std::size_t
stream_buffer<N>::available() const noexcept {
	return (_end - _next);
}

template<std::size_t N>
constexpr std::size_t
stream_buffer<N>::total_capacity() const noexcept {
	return (N);
}

template<std::size_t N>
std::size_t
stream_buffer<N>::capacity() const noexcept {
	return (absolute_end() - end());
}

/**
 * @param offset The new `next` pointer.
 */
template<std::size_t N>
void
stream_buffer<N>::seek(unsigned char* p) noexcept {
	_next = p;
}

/**
 * @param offset The offset of the `next` pointer, relative to its current
 * position.
 */
template<std::size_t N>
void
stream_buffer<N>::seek(streamsize offset) noexcept {
	_next += offset;
}

/**
 * @param offset The offset of the `end` pointer, relative to `next()`.
 */
template<std::size_t N>
void
stream_buffer<N>::set_end(std::size_t offset) noexcept {
	assert(offset >= static_cast<std::size_t>(_next - _data));
	assert(offset <= total_capacity());
	_end = _data + offset;
}

/**
 */
template<std::size_t N>
void
stream_buffer<N>::clear() noexcept {
	seek(_data);
	set_end(0);
}

/**
 * @param b Reference to the byte to be inspected.
 * @warning If there is no read position available, the behaviour is undefined.
 */
template<std::size_t N>
void
stream_buffer<N>::peek(unsigned char& b) const noexcept {
	b = *_next;
}

/**
 * @param dst Pointer to an array of bytes.
 * @param n Number of bytes to extract.
 * @warning The behaviour is undefined if the number of read positions is less
 * than `n`, or if `n` is greater than the area of memory pointed at by `dst`.
 */
template<std::size_t N>
void
stream_buffer<N>::peek(unsigned char* dst, std::size_t n) const noexcept {
	std::memcpy(dst, _next, n);
}

/**
 * @param b Reference to the byte to be inspected.
 * @return The number of bytes that were actually inspected (either 0 or 1).
 * @note If this method is called without a read position available, the value
 * of `b` will not be changed (possibly leaving it uninitialized if it wasn't
 * initialized prior to the `peek_try()` call).
 */
template<std::size_t N>
std::size_t
stream_buffer<N>::peek_try(unsigned char& b) const noexcept {
	if (available() >= 1) {
		peek(b);
		return (1);
	}
	return (0);
}

/**
 * @param dst Pointer to an array of bytes.
 * @param n Number of bytes to inspect.
 * @return The number of bytes that were actually inspected.
 */
template<std::size_t N>
std::size_t
stream_buffer<N>::peek_try(unsigned char* dst, std::size_t n) const noexcept {
	std::size_t const	to_peek = std::min(n, available());
	peek(dst, to_peek);
	return (to_peek);
}

/**
 * @param b Reference to the byte to be extracted.
 * @warning If there is no read position available, the behaviour is undefined.
 */
template<std::size_t N>
void
stream_buffer<N>::get(unsigned char& b) noexcept {
	b = *_next++;
}

/**
 * @param dst Pointer to an array of bytes.
 * @param n Number of bytes to extract.
 * @warning The behaviour is undefined if the number of read positions is less
 * than `n`, or if `n` is greater than the area of memory pointed at by `dst`.
 */
template<std::size_t N>
void
stream_buffer<N>::get(unsigned char* dst, std::size_t n) noexcept {
	peek(dst, n);
	_next += n;
}

/**
 * @param b Reference to the byte to be extracted.
 * @return The number of bytes that were actually extracted (either 0 or 1).
 * @note If this method is called without a read position available, the value
 * of `b` will not be changed (possibly leaving it uninitialized if it wasn't
 * initialized prior to the `get_try()` call).
 */
template<std::size_t N>
std::size_t
stream_buffer<N>::get_try(unsigned char& b) noexcept {
	if (available) {
		get(b);
		return (1);
	}
	return (0);
}

/**
 * @param dst Pointer to an array of bytes.
 * @param n Number of bytes to extract.
 * @return The number of bytes that were actually extracted.
 */
template<std::size_t N>
std::size_t
stream_buffer<N>::get_try(unsigned char* dst, std::size_t n) noexcept {
	std::size_t const	to_get = std::min(n, available());
	get(dst, to_get);
	return (to_get);
}

/**
 * @param n Number of bytes to unget.
 * @return The number of bytes that could be ungotten. This will be equal to the
 * lesser of `n` and the difference of the `next` pointer and the beginning of
 * the buffer's data store (i.e. the number of putback positions).
 */
template<std::size_t N>
std::size_t
stream_buffer<N>::unget(std::size_t n) noexcept {
	std::size_t const	to_unget = std::min(n, _next - _data);
	_next -= to_unget;
	return (to_unget);
}

/**
 * @param b The byte to put into the buffer.
 */
template<std::size_t N>
void
stream_buffer<N>::put(unsigned char b) noexcept {
	*_end++ = b;
}

/**
 * @param src A pointer to a block of bytes to be put into the buffer.
 * @param n The number of bytes that should be put into the buffer.
 */
template<std::size_t N>
void
stream_buffer<N>::put(unsigned char const* src, std::size_t n) noexcept {
	std::memcpy(_data, src, n);
	_end += n;
}

/**
 * @param b The byte to put into the buffer.
 * @return The number of bytes that have been successfully put into the buffer
 * (naturally, only values of 0 or 1 will ever be returned).
 */
template<std::size_t N>
std::size_t
stream_buffer<N>::put_try(unsigned char b) noexcept {
	if (capacity() >= 1) {
		put(b);
		return (1);
	}
	return (0);
}

/**
 * @param src A pointer to a block of bytes to be put into the buffer.
 * @param n The number of bytes that should be put into the buffer.
 * @return The number of bytes that have been successfully put into the buffer.
 */
template<std::size_t N>
std::size_t
stream_buffer<N>::put_try(unsigned char const* src, std::size_t n) noexcept {
	auto const	to_put = std::min(n, capacity());

	put(src, to_put);
	return (to_put);
}

}; // namespace bsf::net::socket

#endif // BSF_NET_SOCKET_BUFFER_IPP