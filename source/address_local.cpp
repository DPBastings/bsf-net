#include "address_local.hpp"

using network::address;
using network::socket_domain;

address<socket_domain::local>::address(char const* path):
	_addr {
		.sun_family = static_cast<sa_family_t>(socket_domain::local),
	},
	_size(0) {

}

address<socket_domain::local>::host_type
address<socket_domain::local>::host() const noexcept {
	return ("");
}

address<socket_domain::local>::subtype
address<socket_domain::local>::subclass() const noexcept {
	return (subtype::path);
}