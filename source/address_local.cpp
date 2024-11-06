#include "address.hpp"

using address = network::basic_address<network::socket_domain::local>;

template<>
address::basic_address(std::string const&):
	_raw {
		.sun_family = static_cast<sa_family_t>(domain),
	},
	_size(0) {

}

template<>
address::host_type
address::host() const noexcept {
	return (""); // todo
}

template<>
address::subclass_type
address::subclass() const noexcept
	requires (network::has_subclass<address::domain>) {
	return (subclass_type::path); // todo
}