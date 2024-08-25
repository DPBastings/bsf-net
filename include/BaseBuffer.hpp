#ifndef NETWORK_BASE_BUFFER_HPP
# define NETWORK_BASE_BUFFER_HPP

# include <concepts>
# include <iostream>
# include <string>

namespace network {

template<typename T>
concept byte_container = requires {
	typename T::value_type;
	requires std::same_as<typename T::value_type, uint8_t>;
};

template<byte_container Ctr>
class BaseBuffer {
public:
	using value_type = uint8_t;
	using size_type = Ctr::size_type;
	using iterator = Ctr::iterator;
	using const_iterator = Ctr::const_iterator;

	BaseBuffer();

	size_type	size() const noexcept;

	iterator		begin() noexcept;
	const_iterator	begin() const noexcept;
	const_iterator	cbegin() const noexcept;
	iterator		end() noexcept;
	const_iterator	end() const noexcept;
	const_iterator	cend() const noexcept;

	void		clear() noexcept;
	size_type	get_from(std::istream&);
	size_type	put_into(std::ostream&) const;
	bool		push_back(value_type);
	bool		push_back(std::u8string const&);
	bool		push_back(std::u8string_view);
	bool		push_back(BaseBuffer const&);

private:
	Ctr	_ctr;
}; // class BaseBuffer

}; // namespace network

#endif // NETWORK_BASE_BUFFER_HPP