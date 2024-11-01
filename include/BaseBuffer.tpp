#ifndef NETPP_BASE_BUFFER_TPP
# define NETPP_BASE_BUFFER_TPP

# ifndef NETPP_BASE_BUFFER_HPP
#  error "include in basic_socketbuf.hpp"
# endif // NETPP_BASE_BUFFER_HPP

namespace network {

template<contiguous_byte_container Ctr>
basic_socketbuf<Ctr>::size_type
basic_socketbuf<Ctr>::remaining_capacity() const noexcept {
	return (capacity() - size());
}

template<contiguous_byte_container Ctr>
bool
basic_socketbuf<Ctr>::append(std::u8string_view bytes) {
	if (bytes.length() > remaining_capacity()) {
		return (false);
	}
	for (auto const& byte: bytes) {
		push_back(byte);
	}
	return (true);
}

template<contiguous_byte_container Ctr>
template<contiguous_byte_container Dtr>
bool
basic_socketbuf<Ctr>::append(basic_socketbuf<Dtr> const& that) {
	if (that.length() > remaining_capacity()) {
		return (false);
	}
	for (auto const& byte: that) {
		push_back(byte);
	}
	return (true);
}

}; // namespace network

#endif // NETPP_BASE_BUFFER_TPP