#include <libbsf/net/address/unix.hpp>

using Address = bsf::net::address<bsf::net::domain::unix>;

template<>
Address::address(std::string const&):
	_raw {
		.sun_family = static_cast<sa_family_t>(domain),
	} {}

template<>
Address::host_type
Address::host() const noexcept {
	return (""); // todo
}

template<>
Address::to_string() const {
	return ("local socket"); // todo
}