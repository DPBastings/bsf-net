#ifndef BSF_NET_ADDRESS_UNIX_HPP
# define BSF_NET_ADDRESS_UNIX_HPP

# include <libbsf/net/address/address.hpp> // net::address_storage
# include <optional> // std::optional
extern "C" {
# include <sys/un.h> // Unix
}

namespace bsf::net {

template<>
class address<domain::unix>: public address_base<sockaddr_un> {
public:
	static constexpr auto	min_size = sizeof(sa_family_t);
	static constexpr auto	max_size = sizeof(sockaddr_un);
	static constexpr auto	max_path_length = max_size - min_size;
	static constexpr auto	domain = domain::unix;
	using host_t = char const*;

	/// @brief Construct an empty (unnamed) Unix address.
	address() noexcept;

	/// @brief Create a pathname-based Unix address.
	[[nodiscard]] static std::optional<address>	make_path(host_t) noexcept;
	/// @brief Create an abstract Unix address.
	[[nodiscard]] static std::optional<address>	make_abstract(host_t, socklen_t) noexcept;

	/// @brief Convert this address to a string representation.
	[[nodiscard]] std::string	to_string() const;

	/// @brief Query whether this Unix address is pathname-based.
	bool	is_path() const noexcept;
	/// @brief Query whether this Unix address is abstract.
	bool	is_abstract() const noexcept;

	/// @brief Get this address' host name.
	[[nodiscard]] host_t	host() const noexcept;
private:
	using Base = address_base<sockaddr_un>;
}; // class address

}; // namespace bsf::net

#endif // BSF_NET_ADDRESS_UNIX_HPP