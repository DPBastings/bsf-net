#ifndef BSF_NET_SOCKET_STREAM_IPP
# define BSF_NET_SOCKET_STREAM_IPP

namespace bsf::net::socket {

template<domain::domain D, std::size_t IC, std::size_t OC>
stream<D, IC, OC>::stream(socket_t&& socket) noexcept:
	_socket{ std::forward<socket_t>(socket) } {}

/**
 * @param addr The remote address to connect the new socket to.
 * @param conf Configuration parameters for the new socket.
 * @return On success, an open socket, connected to the supplied address.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
std::optional<stream<D, IC, OC>>
stream<D, IC, OC>::make(address_t const& addr, config conf) noexcept {
	auto	sock = socket_t::make(conf);
	if (!sock) return (std::nullopt);

	if (sock->connect(addr)) {
		return (stream{ std::move(sock) });
	} else {
		return (std::nullopt);
	}
}

template<domain::domain D, std::size_t IC, std::size_t OC>
void
stream<D, IC, OC>::close() noexcept {
	_in.clear();
	_out.clear();
	_socket.close();
}


template<domain::domain D, std::size_t IC, std::size_t OC>
std::size_t
stream<D, IC, OC>::in_avail() const noexcept {
	return (_in.available());
}

/**
 * @param b Reference to the byte to be inspected.
 * @warning If then input buffer has no read position available, the behaviour 
 * is undefined.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
void
stream<D, IC, OC>::peek(unsigned char& b) const noexcept {
	_in.peek(b);
}

/**
 * @param dst Pointer to an array of bytes.
 * @param n Number of bytes to extract.
 * @warning The behaviour is undefined if the number of read positions is less
 * than `n`, or if `n` is greater than the area of memory pointed at by `dst`.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
void
stream<D, IC, OC>::peek(unsigned char* dst, std::size_t n) const noexcept {
	_in.peek(dst, n);
}

/**
 * @param b Reference to the byte to be inspected.
 * @return The number of bytes that were actually inspected (either 0 or 1).
 * @note If this method is called without a read position available on the
 * input buffer, the value of `b` will not be changed (possibly leaving it 
 * uninitialized if it wasn't initialized prior to the `peek_try()` call).
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
std::size_t
stream<D, IC, OC>::peek_try(unsigned char& b) const noexcept {
	return (_in.peek_try(b));
}

/**
 * @param dst Pointer to an array of bytes.
 * @param n Number of bytes to inspect.
 * @return The number of bytes that were actually inspected.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
std::size_t
stream<D, IC, OC>::peek_try(unsigned char* dst, std::size_t n) const noexcept {
	return (_in.peek_try(dst, n));
}

/**
 * @param b Reference to the byte to be extracted.
 * @warning If there is no read position available, the behaviour is undefined.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
void
stream<D, IC, OC>::get(unsigned char& b) noexcept {
	_in.get(b);
}

/**
 * @param dst Pointer to an array of bytes.
 * @param n Number of bytes to extract.
 * @warning The behaviour is undefined if the number of read positions is less
 * than `n`, or if `n` is greater than the area of memory pointed at by `dst`.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
void
stream<D, IC, OC>::get(unsigned char* dst, std::size_t n) noexcept {
	_in.get(dst, n);
}

/**
 * @param b Reference to the byte to be extracted.
 * @return The number of bytes that were actually extracted (either 0 or 1).
 * @note If this method is called without a read position available, the value
 * of `b` will not be changed (possibly leaving it uninitialized if it wasn't
 * initialized prior to the `get_try()` call).
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
std::size_t
stream<D, IC, OC>::get_try(unsigned char& b) noexcept {
	return (_in.get_try(b));
}

/**
 * @param dst Pointer to an array of bytes.
 * @param n Number of bytes to extract.
 * @return The number of bytes that were actually extracted.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
std::size_t
stream<D, IC, OC>::get_try(unsigned char* dst, std::size_t n) noexcept {
	return (_in.get_try(dst, n));
}

/**
 * @param n Number of bytes to unget.
 * @return The number of bytes that could be ungotten. This will be equal to the
 * lesser of `n` and the difference of the `next` pointer and the beginning of
 * the buffer's data store (i.e. the number of putback positions).
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
std::size_t
stream<D, IC, OC>::unget(std::size_t n) noexcept {
	return (_in.unget(n));
}

/**
 * @param b The byte to put into the buffer.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
void
stream<D, IC, OC>::put(unsigned char b) noexcept {
	_out.put(b);
}

/**
 * @param src A pointer to a block of bytes to be put into the buffer.
 * @param n The number of bytes that should be put into the buffer.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
void
stream<D, IC, OC>::put(unsigned char const* src, std::size_t n) noexcept {
	_out.put(src, n);
}

/**
 * @param b The byte to put into the buffer.
 * @return The number of bytes that have been successfully put into the buffer
 * (naturally, only values of 0 or 1 will ever be returned).
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
std::size_t
stream<D, IC, OC>::put_try(unsigned char b) noexcept {
	return (_out.put_try(b));
}

/**
 * @param src A pointer to a block of bytes to be put into the buffer.
 * @param n The number of bytes that should be put into the buffer.
 * @return The number of bytes that have been successfully put into the buffer.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
std::size_t
stream<D, IC, OC>::put_try(unsigned char const* src, std::size_t n) noexcept {
	return (_out.put_try(src, n));
}

/**
 * This method will attempt to receive as many bytes as possible in one call.
 * @post If the input buffer held no unextracted bytes prior to the `recv` call,
 * no putback positions will be available afterward, even if no data could be
 * received.
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
recv_result
stream<D, IC, OC>::recv(recv_flag flag) noexcept {
	if (_in.available() == 0) {
		_in.clear(); // Discard putbacks in order to use the entire buffer.
	}

	auto	res = _socket.recv(_in.next(), _in.capacity(), flag);

	if (res) { // System call was successful.
		_in.set_end(*res); // Mark all received bytes as available.
	}
	return (res);
}

/**
 */
template<domain::domain D, std::size_t IC, std::size_t OC>
send_result
stream<D, IC, OC>::send(send_flag flag) noexcept {
	auto	res = _socket.send(_out.begin(), _out.available(), flag);

	if (res) { // System call was successful.
		if (*res == _out.available()) { // All bytes were sent...
			_out.clear(); // therefore clear the buffer.
		} else { // Some bytes were not sent...
			_out.seek(*res); // ...therefore seek to the first unsent byte.
		}
	}
	return (res);
}

}; // bsf::net::socket

#endif // BSF_NET_SOCKET_STREAM_IPP