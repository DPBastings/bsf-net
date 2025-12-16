#ifndef BSF_NET_ADDRESS_IPP
# define BSF_NET_ADDRESS_IPP

extern "C" {
# include <sys/socket.h> // ::socketpair
}

namespace bsf::net::address {

// Accessors

template<domain::domain D>
sockaddr*
address<D>::raw_ptr() noexcept {
	return (reinterpret_cast<sockaddr*>(&_raw));
}

template<domain::domain D>
sockaddr const*
address<D>::raw_ptr() const noexcept {
	return (reinterpret_cast<sockaddr const*>(&_raw));
}

template<domain::domain D>
socklen_t*
address<D>::size_ptr() noexcept {
	return (&_size);
}

template<domain::domain D>
socklen_t
address<D>::size() const noexcept {
	return (_size);
}



template<domain::domain D>
std::string
to_string(address<D> const& that) {
	return (that.to_string());
}

}; // namespace bsf::net::address

#endif // BSF_NET_ADDRESS_IPP