#ifndef NETPP_STATIC_BUFFER_HPP
# define NETPP_STATIC_BUFFER_HPP

# include "network.hpp"
# include "BaseBuffer.hpp"

# include <array>
# include <istream>
# include <ostream>
# include <string>

namespace network {

template<size_t SIZE> // should be like std::array::size_type
class StaticBufferContainer: private std::array<uint8_t, SIZE> {
public:
	using super = std::array<uint8_t, SIZE>;
	using super::value_type;
	using super::size_type;

	size_type	size() const noexcept;
	size_type	capacity() const noexcept;
	using Ctr::begin;
	using Ctr::cbegin;
	using Ctr::end;
	using Ctr::cend;

	void	clear() noexcept;
	bool	push_back(value_type) noexcept;
private:
	size_type	_size;
}; // class StaticBufferContainer

template<size_t SIZE>
using StaticBuffer = BaseBuffer<StaticBufferContainer<SIZE>>;

}; // namespace network

# include "network/StaticBufferContainer.tpp"

#endif // NETPP_STATIC_BUFFER_HPP
