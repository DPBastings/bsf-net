#ifndef BSF_NET_DOMAIN_HPP
# define BSF_NET_DOMAIN_HPP

extern "C" {
# include <sys/socket.h> // domain defines
}

namespace bsf::net::domain {

enum domain {
	local = AF_LOCAL,
	ipv4 = AF_INET,
	ipv6 = AF_INET6,
};

template<bool IsInternet, bool IsPairable>
struct traits_base {
	static constexpr bool	is_internet = IsInternet;
	static constexpr bool	is_pairable = IsPairable;
};

template<domain Domain>
struct traits;

template<>
struct traits<domain::local>:
	traits_base<false, true> {};

template<>
struct traits<domain::ipv4>:
	traits_base<true, false> {};

template<>
struct traits<domain::ipv6>:
	traits_base<true, false> {};

}; // namespace bsf::net::domain

#endif // BSF_NET_DOMAIN_HPP