#ifndef BSF_NET_BASE_BUFFER_HPP
# define BSF_NET_BASE_BUFFER_HPP

# include <concepts>
# include <iostream>
# include <string>

namespace bsf::net {
	
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
class socketbuf: private Ctr {
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
	void		append(socketbuf<Dtr> const&);

	size_type	get_from(std::istream&);
	size_type	put_into(std::ostream&) const;
}; // class socketbuf

template<contiguous_byte_container Ctr>
std::istream&	operator>>(std::istream&, socketbuf<Ctr>&);
template<contiguous_byte_container Ctr>
std::ostream&	operator<<(std::ostream&, socketbuf<Ctr> const&);

}; // namespace bsf::net

#include "socketbuf.tpp"

#endif // BSF_NET_BASE_BUFFER_HPP