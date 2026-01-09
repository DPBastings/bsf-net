#ifndef BSF_NET_DOMAIN_HPP
# define BSF_NET_DOMAIN_HPP

extern "C" {
# include <sys/socket.h> // domain defines
}

namespace bsf::net::domain {

enum domain {
	unix = AF_UNIX,
	ipv4 = AF_INET,
	ipv6 = AF_INET6,
};

template<bool IsInternet>
struct traits_base {
	static constexpr bool	is_internet = IsInternet;
};

template<domain D>
struct traits;

template<>
struct traits<domain::unix>:
	traits_base<false> {};

template<>
struct traits<domain::ipv4>:
	traits_base<true> {};

template<>
struct traits<domain::ipv6>:
	traits_base<true> {};

}; // namespace bsf::net::domain

#endif // BSF_NET_DOMAIN_HPP