#ifndef NETPP_STATIC_BUFFER_CONTAINER_TPP
# define NETPP_STATIC_BUFFER_CONTAINER_TPP

# ifndef NETPP_STATIC_BUFFER_HPP
#  error "include in StaticBuffer.hpp"
# endif // NETPP_STATIC_BUFFER_HPP

# include <sstream>

namespace network {
template<size_t SIZE>
StatioBufferContainer<SIZE>::Buffer():
	super(), _len(0) {}

template<size_t SIZE>
StatioBufferContainer<SIZE>::operator std::string() const {
	return (std::string(this->begin(), this->end()));
}

template<size_t SIZE>
void
StatioBufferContainer<SIZE>::empty() noexcept {
	_len = 0;
}

template<size_t SIZE>
size_t
StatioBufferContainer<SIZE>::len() const noexcept {
	return (_len);
}

template<size_t SIZE>
constexpr size_t
StatioBufferContainer<SIZE>::capacity() {
	return (SIZE);
}

template<size_t SIZE>
bool
StatioBufferContainer<SIZE>::push_back(char c) noexcept {
	if (len() >= capacity())
		return (false);
	this->data()[_len++] = c;
	return (true);
}

template<size_t SIZE>
size_t
StatioBufferContainer<SIZE>::push_back(std::string const& str) noexcept {
	if (str.size() > capacity() - len())
		return (0);
	for (auto const& c: str)
		this->data()[_len++] = c;
	return (str.size());
}

template<size_t SIZE>
template<size_t TSIZE>
size_t
StatioBufferContainer<SIZE>::push_back(StatioBufferContainer<TSIZE> const& that) noexcept {
	if (that.len() > capacity() - len())
		return (0);
	for (auto const& c: that)
		this->data()[_len++] = c;
	return (that.len());
}

template<size_t SIZE>
typename StatioBufferContainer<SIZE>::iterator
StatioBufferContainer<SIZE>::begin() noexcept {
	return (super::begin());
}

template<size_t SIZE>
typename StatioBufferContainer<SIZE>::const_iterator
StatioBufferContainer<SIZE>::begin() const noexcept {
	return (super::begin());
}

template<size_t SIZE>
typename StatioBufferContainer<SIZE>::const_iterator
StatioBufferContainer<SIZE>::cbegin() const noexcept {
	return (super::begin());
}

template<size_t SIZE>
typename StatioBufferContainer<SIZE>::iterator
StatioBufferContainer<SIZE>::end() noexcept {
	return (begin() + _len);
}
template<size_t SIZE>
typename StatioBufferContainer<SIZE>::const_iterator
StatioBufferContainer<SIZE>::end() const noexcept {
	return (begin() + _len);
}
template<size_t SIZE>
typename StatioBufferContainer<SIZE>::const_iterator
StatioBufferContainer<SIZE>::cend() const noexcept {
	return (cbegin() + _len);
}

template<size_t SIZE>
size_t
StatioBufferContainer<SIZE>::get(std::istream& is) {
	is.read(this->data(), SIZE); // signedness mismatch!
	_len = is.gcount();
	return (_len);
}

template<size_t SIZE>
size_t
StatioBufferContainer<SIZE>::put(std::ostream& os) const {
	os.write(this->data(), _len); // signedness mismatch!
	return (os.is_open() ? _len : 0);
}

template<size_t SIZE>
std::istream&
operator>>(std::istream& is, StatioBufferContainer<SIZE>& that) {
	while (!is.eof() && that._len < SIZE)
		that[that._len++] = is.get();
	return (is);
}

template<size_t SIZE>
std::ostream&
operator<<(std::ostream& os, StatioBufferContainer<SIZE> const& that) {
	for (auto const& byte: that)
		os << byte;
	return (os);
}

}; // namespace network

#endif // NETPP_STATIC_BUFFER_CONTAINER_TPP
