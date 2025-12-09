#ifndef BSF_NET_BASE_BUFFER_TPP
# define BSF_NET_BASE_BUFFER_TPP

# ifndef BSF_NET_BASE_BUFFER_HPP
#  error "include in socketbuf.hpp"
# endif // BSF_NET_BASE_BUFFER_HPP

namespace bsf::net {

template<contiguous_byte_container Ctr>
socketbuf<Ctr>::size_type
socketbuf<Ctr>::remaining_capacity() const noexcept {
	return (capacity() - size());
}

template<contiguous_byte_container Ctr>
bool
socketbuf<Ctr>::append(std::u8string_view bytes) {
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
socketbuf<Ctr>::append(socketbuf<Dtr> const& that) {
	if (that.length() > remaining_capacity()) {
		return (false);
	}
	for (auto const& byte: that) {
		push_back(byte);
	}
	return (true);
}

}; // namespace bsf::net

#endif // BSF_NET_BASE_BUFFER_TPP