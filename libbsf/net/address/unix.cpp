#include <libbsf/net/address/unix.hpp>
#include <cstring> // std::strlen, std::memcpy

namespace bsf::net {

using Address = address<domain::unix>;

Address::address() noexcept:
	Base{
		sockaddr_un{
			.sun_family = static_cast<sa_family_t>(domain),
			.sun_path{}
		},
		sizeof(sa_family_t)
	} {}

/**
 * @param path The path representing the address' host name. Must be a zero-
 * terminated string, not greater than `max_path_length` in length.
 * @warning If `host` doesn't point to a string, the behaviour is undefined.
 * @return On success, a pathname-based Unix address.
 * If the string represented by `path` was too great in length, `std::nullopt`.
 */
std::optional<Address>
Address::make_path(host_t path) noexcept {
	auto const	size = std::strlen(path);
	if (size >= max_path_length) return (std::nullopt);

	Address	addr;
	std::memcpy(addr._data.sun_path, path, size);
	addr._size = min_size + size;
	return (addr);
}

/**
 * @param path The address' host name. Must point to a character sequence no
 * more than `max_path_length - 1` in length.
 * @param size The length of the host name character sequence.
 * @warning If `host` equals `nullptr` or points to an inappropriately-sized
 * block of memory, the behaviour is undefined.
 * @return On success, an abstract Unix address.
 * If `size` was not of an appropriate value: `std::nullopt`.
 */
std::optional<Address>
Address::make_abstract(host_t host, socklen_t size) noexcept {
	if (size + 1 >= max_path_length) return (std::nullopt);

	Address	addr;
	addr._data.sun_path[0] = '\0';
	std::memcpy(addr._data.sun_path + 1, host, size);
	addr._size = min_size + size + 1;
	return (addr);
}

std::string
Address::to_string() const {
	if (is_path()) {
		return (std::string(host()));
	}
	return ("<abstract Unix socket>");
}

bool
Address::is_path() const noexcept {
	return (_size > min_size && *host() != '\0');
}

bool
Address::is_abstract() const noexcept {
	return (_size > min_size && *host() == '\0');
}

Address::host_t
Address::host() const noexcept {
	return (_data.sun_path);
}

}; // namespace bsf::net
