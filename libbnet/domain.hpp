#ifndef BSF_NET_DOMAIN_HPP
# define BSF_NET_DOMAIN_HPP

namespace bsf::net::domain {

struct local {};
struct ipv4 {};
struct ipv6 {};


template<bool IsInternet, bool IsPairable>
struct traits_base {
	static constexpr bool	is_internet = IsInternet;
	static constexpr bool	is_pairable = IsPairable;
};

template<typename T>
struct traits;

template<>
struct traits<local>: traits_base<false> {};

template<>
struct traits<ipv4>: traits_base<true> {};

template<>
struct traits<ipv6>: traits_base<true> {};

}; // namespace bsf::net::domain

#endif // BSF_NET_DOMAIN_HPP