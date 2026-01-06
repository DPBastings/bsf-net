#ifndef BSF_NET_ADDRESS_HPP
# define BSF_NET_ADDRESS_HPP

# include <libbsf/net/domain.hpp> // net::domain::*
# include <libbsf/net/utility/utility.hpp> // net::void_t
# include <concepts> // std::same_as
# include <cstdint> // *int*_t
# include <optional> // std::optional
# include <string> // std::string

namespace bsf::net {

template<typename S>
class address_base {
public:
	static constexpr socklen_t	max_size = sizeof(S);

	address_base() noexcept = default;
	address_base(S const& data):
		_data{ data } {}

	[[nodiscard]] sockaddr const*	data() const noexcept {
		return (reinterpret_cast<sockaddr const*>(&_data));
	}
	[[nodiscard]] sockaddr*			data() noexcept {
		return (reinterpret_cast<sockaddr*>(&_data));
	}
	[[nodiscard]] socklen_t			size() const noexcept {
		return (_size);
	}
	[[nodiscard]] socklen_t*		size() noexcept {
		return (&_size);
	}
protected:
	S			_data{};
	socklen_t	_size = sizeof(S);
}; // struct address_base<S>

template<domain::domain D>
class address;

/// @brief Convert an address to a text representation.
template<domain::domain D>
[[nodiscard]] std::string
to_string(address<D> const& that) {
	return (that.to_string());
}

}; // namespace bsf::net

#endif // BSF_NET_ADDRESS_HPP
