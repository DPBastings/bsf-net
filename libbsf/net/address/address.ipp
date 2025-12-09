#ifndef BSF_NET_ADDRESS_IPP
# define BSF_NET_ADDRESS_IPP

extern "C" {
# include <sys/socket.h> // ::socketpair
}

namespace bsf::net::address {

// Accessors

template<domain::domain Domain>
sockaddr*
address<Domain>::raw_ptr() noexcept {
	return (reinterpret_cast<sockaddr*>(&_raw));
}

template<domain::domain Domain>
sockaddr const*
address<Domain>::raw_ptr() const noexcept {
	return (reinterpret_cast<sockaddr const*>(&_raw));
}

template<domain::domain Domain>
socklen_t*
address<Domain>::size_ptr() noexcept {
	return (&_size);
}

template<domain::domain Domain>
socklen_t
address<Domain>::size() const noexcept {
	return (_size);
}



template<domain::domain Domain>
std::string
to_string(address<Domain> const& that) {
	return (that.to_string());
}

}; // namespace bsf::net::address

#endif // BSF_NET_ADDRESS_IPP