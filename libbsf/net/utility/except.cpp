#include <libbsf/net/utility/except.hpp>
#include <cerrno> // errno

extern int errno;

using bsf::net::exception;

exception::exception() noexcept:
	exception{ "BSF/Net exception" } {}

exception::exception(char const* msg) noexcept:
	_msg{ msg },
	_errno{ errno } {}



char const*
exception::what() const noexcept {
	return (_msg);
}

int
exception::error() const noexcept {
	return (_errno);
}