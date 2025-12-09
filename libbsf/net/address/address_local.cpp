#include <libbsf/net/address/address.hpp>

using Address = bsf::net::address::address<bsf::net::domain::local>;

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