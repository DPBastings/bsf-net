#ifndef NETPP_BASE_BUFFER_HPP
# define NETPP_BASE_BUFFER_HPP

# include <concepts>
# include <iostream>
# include <string>

namespace network {
template<typename T>
concept byte_container = requires {
	typename T::value_type;
	requires std::same_as<typename T::value_type, uint8_t>; 
};

template<typename T>
concept contiguous_container = requires(T t) {
	{ t.data() } -> std::same_as<uint8_t*>;
}

template<typename T>
concept contiguous_byte_container = byte_container<T> && contiguous_container<T>;

template<contiguous_byte_container Ctr>
class BaseBuffer: private Ctr {
public:
	using Ctr::value_type; // == uint8_t
	using Ctr::size_type;
	using Ctr::iterator;
	using Ctr::const_iterator;

	using Ctr::size;
	using Ctr::capacity;
	size_type	remaining_capacity() const noexcept;
	using Ctr::begin;
	using Ctr::cbegin;
	using Ctr::end;
	using Ctr::cend;

	using Ctr::clear;
	using Ctr::push_back;
	void		append(std::u8string_view);
	template<contiguous_byte_container Dtr>
	void		append(BaseBuffer<Dtr> const&);

	size_type	get_from(std::istream&);
	size_type	put_into(std::ostream&) const;
}; // class BaseBuffer

template<contiguous_byte_container Ctr>
std::istream&	operator>>(std::istream&, BaseBuffer<Ctr>&);
template<contiguous_byte_container Ctr>
std::ostream&	operator<<(std::ostream&, BaseBuffer<Ctr> const&);
}; // namespace network

#include "BaseBuffer.tpp"

#endif // NETPP_BASE_BUFFER_HPP