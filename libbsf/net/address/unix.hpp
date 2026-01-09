#ifndef BSF_NET_ADDRESS_UNIX_HPP
# define BSF_NET_ADDRESS_UNIX_HPP

# include <libbsf/net/address/address.hpp> // net::address_storage
extern "C" {
# include <sys/un.h> // Unix
}

namespace bsf::net {

template<>
class address<domain::unix> {
public:
	static constexpr auto	domain = domain::unix;
	using host_t = char const*;

	address() noexcept = default;
	address(host_t) noexcept;

	[[nodiscard]] static std::optional<address>	from_string(char const*) noexcept;
	[[nodiscard]] std::string					to_string() const;

	[[nodiscard]] host_t	host() const noexcept;
private:
	address_storage<sockaddr_un>	_storage;
}; // class address

}; // namespace bsf::net

#endif // BSF_NET_ADDRESS_UNIX_HPP