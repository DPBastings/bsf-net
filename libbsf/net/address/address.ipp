#ifndef BSF_NET_ADDRESS_IPP
# define BSF_NET_ADDRESS_IPP

extern "C" {
# include <sys/socket.h> // ::socketpair
}

namespace bsf::net {

template<domain::domain D>
std::string
to_string(address<D> const& that) {
	return (that.to_string());
}

}; // namespace bsf::net

#endif // BSF_NET_ADDRESS_IPP